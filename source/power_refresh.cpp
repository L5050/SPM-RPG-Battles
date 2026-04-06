#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"
#include "power_refresh.h"

#include <msl/string.h>
#include <spm/rel/an.h>
#include <spm/acdrv.h>
#include <spm/evt_ac.h>
#include <spm/evtmgr.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/seqdef.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/system.h>
#include <wii/os/OSError.h>
#include <cmath>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod
{

Vec3 iconPositions[9] = {
  {-100.0, 100.0, 0.0},
  {0.0, 100.0, 0.0},
  {100.0, 100.0, 0.0},
  {-100.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
  {100.0, 0.0, 0.0},
  {-100.0, -100.0, 0.0},
  {0.0, -100.0, 0.0},
  {100.0, -100.0, 0.0},
};

enum Direction { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

void iconGlideTo(spm::icondrv::IconEntry * icon, u32 frames, f32 destX, f32 destY, f32 destZ)
{
  f32 absV = fabs(destX - icon->position.x);
  if (absV != 0.0)
  {
    if (destX > icon->position.x)
    {
      icon->position.x += (absV / frames);
    }
    else
    {
      icon->position.x -= (absV / frames);
    }
  }
  absV = fabs(destY - icon->position.y);
  if (absV != 0.0)
  {
    if (destY > icon->position.y)
    {
      icon->position.y += (absV / frames);
    }
    else
    {
      icon->position.y -= (absV / frames);
    }
  }
  absV = fabs(destZ - icon->position.z);
  if (absV != 0.0)
  {
    if (destZ > icon->position.z)
    {
      icon->position.z += (absV / frames);
    }
    else
    {
      icon->position.z -= (absV / frames);
    }
  }
  return;
}

void iconRotateTo(spm::icondrv::IconEntry * icon, u32 frames, f32 destX, f32 destY, f32 destZ)
{
  f32 absV = fabs(destX - icon->rotation.x);
  if (absV != 0.0)
  {
    if (destX > icon->rotation.x)
    {
      icon->rotation.x += (absV / frames);
    }
    else
    {
      icon->rotation.x -= (absV / frames);
    }
  }
  absV = fabs(destY - icon->rotation.y);
  if (absV != 0.0)
  {
    if (destY > icon->rotation.y)
    {
      icon->rotation.y += (absV / frames);
    }
    else
    {
      icon->rotation.y -= (absV / frames);
    }
  }
  absV = fabs(destZ - icon->rotation.z);
  if (absV != 0.0)
  {
    if (destZ > icon->rotation.z)
    {
      icon->rotation.z += (absV / frames);
    }
    else
    {
      icon->rotation.z -= (absV / frames);
    }
  }
  return;
}


void iconFadeTo(spm::icondrv::IconEntry * icon, u32 frames, u8 alpha)
{
  s32 absV = abs((s32)alpha - (s32)icon->colour.a);
  if (absV != 0)
  {
    if (alpha > icon->colour.a)
    {
      icon->colour.a += (u8)(absV / frames);
    }
    else
    {
      icon->colour.a -= (u8)(absV / frames);
    }
  }
  return;
}

void iconZoomTo(spm::icondrv::IconEntry * icon, u32 frames, f32 scale)
{
  s32 absV = fabs(scale - icon->scale);
  if (absV != 0)
  {
    if (scale > icon->scale)
    {
      icon->scale += (absV / (f32)frames);
    }
    else
    {
      icon->scale -= (absV / (f32)frames);
    }
  }
  return;
}

void moveCursor(PowerRefreshWork * wp)
{
  if (wp->cursorFlags == 0x0)
    return;
  f32 destX = wp->panels[wp->cursorDestIndex].entry->position.x;
  f32 destY = wp->panels[wp->cursorDestIndex].entry->position.y - 15.0;
  f32 destZ = wp->panels[wp->cursorDestIndex].entry->position.z;
  if (wp->cursorEntry->position.x == destX && wp->cursorEntry->position.y == destY)
  {
    wp->cursorFlags = 0x0;
    return;
  }
  iconGlideTo(wp->cursorEntry, wp->cursorMoveFrames, destX, destY, destZ);
}

s32 moveInGrid(s32 index, Direction dir)
{
  s32 ret = index;
  switch (dir)
  {
  case UP:
    ret -= 3;
    if (ret < 0)
    {
      ret += 9;
    }
    break;
  case DOWN:
    ret += 3;
    if (ret > 8)
    {
      ret -= 9;
    }
    break;
  case LEFT:
    ret -= 1;
    if (ret < 0)
    {
      ret += 9;
    }
    break;
  case RIGHT:
    ret += 1;
    if (ret > 8)
    {
      ret -= 9;
    }
    break;
  }

  return ret;
}

void initFunc(spm::acdrv::AcEntry *entry)
{
  spm::spmario::SpmarioGlobals * gp = spm::spmario::gp;
  
  // Allocate space, memset so theres no garbage data
  //entry->flags = entry->flags | 4;
  PowerRefreshWork *wp = (PowerRefreshWork *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(PowerRefreshWork));
  msl::string::memset(wp, 0, sizeof(PowerRefreshWork));
  entry->acDefEntry = (void *)wp;

  // Set the timers, first is how long until the AC starts, second is how long the AC lasts
  // gp->fps needs to be converted to u32 because all timers need to be compared against gp->frameCounter which is unsigned
  wp->startTimer = 1 * (u32)gp->fps;
  wp->timer = 12 * (u32)gp->fps;
  wii::os::OSReport("turtle wow %u!\n", wp->startTimer);
  wii::os::OSReport("turtle wow %u!\n", wp->timer);

  for (s32 i = 0; i < 9; i++)
  {
    s32 rand = spm::system::irand(3);
    switch (rand)
    {
    case 0:
      wp->panels[i].iconId = 1600; // FP
      break;
    case 1:
      wp->panels[i].iconId = 1601; // HP
      break;
    case 2:
      wp->panels[i].iconId = 1602; // Poison
      break;
    case 3:
      wp->panels[i].iconId = 1602; // Poison
      break;
    }
    if (i == 4)
    {
      wp->panels[4].iconId = 1601; // Guaruntee a HP panel for the center panel
    }

    // Create and associate iconEntries
    spm::icondrv::IconEntry * iconEntry = iconEntryAutoname(wp->panels[i].iconId);
    spm::icondrv::iconFlagOn(iconEntry->name, 2);
    iconEntry->colour.a = 0;
    iconEntry->rotation.y = 360.0;
    iconEntry->position = iconPositions[i];
    iconEntry->position.z += 25.0;
    iconEntry->scale = 1.0;
    wp->panels[i].entry = iconEntry;
    wp->panels[i].flags = 0x3;
    wp->panels[i].fadeIn = 1 * (u32)gp->fps;
    spm::icondrv::IconEntry * backgroundEntry = iconEntryAutoname(1603);
    spm::icondrv::iconFlagOn(backgroundEntry->name, 2);
    //backgroundEntry->colour.a = 0;
    backgroundEntry->position = iconPositions[i];
    backgroundEntry->position.z -= 3.0;
    wp->panels[i].background = backgroundEntry;
  }

  // Create cursor icon
  spm::icondrv::IconEntry * cursorEntry = iconEntryAutoname(1604);
  spm::icondrv::iconFlagOn(cursorEntry->name, 2);
  cursorEntry->alpha = 0;
  cursorEntry->position = wp->panels[4].entry->position;
  wp->cursorDestIndex = 4;
  cursorEntry->position.y -= 15.0;
  wp->cursorEntry = cursorEntry;
  wp->cursorEntry->scale = 1.3;
  
  // Update frame counter
  wp->frameCounter = gp->frameCounter;
  return;
}

void mainFunc(spm::acdrv::AcEntry *entry)
{
  spm::spmario::SpmarioGlobals *gp = spm::spmario::gp;
  PowerRefreshWork *wp = (PowerRefreshWork *)entry->acDefEntry;

  // First run of mainFunc, sync the frameCounter
  if (wp->flags == 0)
  {
    wp->frameCounter = gp->frameCounter;
    wp->flags = 0x1;
  }

  // Frame counters are identical, wait until a frame passes in order to do the rest of the function logic
  if (gp->frameCounter == wp->frameCounter)
  {
    return;
  }
  else // Subtract the amount of passed time from the various timers
  {
    u32 absV = abs((s32)gp->frameCounter - (s32)wp->frameCounter);

    wp->frameCounter += 1;
    
    if (wp->startTimer > 0)
    {
      wp->startTimer -= absV;
    }
    else if (wp->timer > 0)
    {
      wp->timer -= absV;
    }

    if (wp->cursorMoveFrames > 0)
    {
      wp->cursorMoveFrames -= absV;
    }

    if (wp->poisonFrames > 0)
    {
      wp->poisonFrames -= absV;
    }

    // Subtract from panel timers
    for (u8 i = 0; i < 9; i++)
    {
      if (wp->panels[i].fadeIn > 0)
      {
        wp->panels[i].fadeIn -= absV;
      }
      if (wp->panels[i].fadeOut > 0)
      {
        wp->panels[i].fadeOut -= absV;
      }
      if (wp->panels[i].waitTimer > 0)
      {
        wp->panels[i].waitTimer -= absV;
      }
    }
  }

  // Start fading in and spinning the icons
  if (wp->startTimer > 0)
  {
    for (s32 i = 0; i < 9; i++)
    {
      //iconFadeTo(wp->panels[i].entry, wp->startTimer, 255);
      iconFadeTo(wp->panels[i].background, wp->startTimer, 255);
      //iconRotateTo(wp->panels[i].entry, wp->startTimer, 0.0, 0.0, 0.0);
      //iconZoomTo(wp->panels[i].entry, wp->startTimer, 1.5);
    }
    iconFadeTo(wp->cursorEntry, wp->startTimer, 255);
  }

  if (wp->timer == 0 && wp->flags == 0x1)
  {
    if (wp->hp < 3)
    {
      wp->hp = 3;
    }
    wp->flags = 0x2;
    for (u8 i = 0; i < 9; i++)
    {
      if (wp->panels[i].fadeOut == 0)
      {
        wp->panels[i].fadeOut = 30;
        wp->panels[i].flags = 0x2;
      }
    }
  }

  if (wp->flags == 0x2)
  {
    u8 finishedCount = 0;
    for (u8 i = 0; i < 9; i++)
    {
      if (wp->panels[i].fadeOut == 0)
      {
        finishedCount += 1;
      }
    }
    if (finishedCount == 9)
    {
      entry->results = wp->hp;
      s32 fp = _getFP();
      s32 maxFp = _getMaxFP();
      s32 newFp = fp + wp->fp;
      if (newFp > maxFp)
      {
        newFp = maxFp;
      }
      _setFP(newFp);
      wp->flags = 0x3;
    }
  }

    // Animate and spin icons when clicked/fading in
    for (u8 i = 0; i < 9; i++)
    {
      if (wp->panels[i].fadeIn > 0)
      {
        iconFadeTo(wp->panels[i].entry, wp->panels[i].fadeIn, 255);
        iconRotateTo(wp->panels[i].entry, wp->panels[i].fadeIn, 0.0, 0.0, 0.0);
      }
      if (wp->panels[i].fadeOut > 0)
      {
        iconFadeTo(wp->panels[i].entry, wp->panels[i].fadeOut, 0);
        iconRotateTo(wp->panels[i].entry, wp->panels[i].fadeOut, 0.0, 360.0, 0.0);
      }
      if (wp->panels[i].waitTimer == 0 && wp->panels[i].flags == 0x0)
      {
        s32 rand = spm::system::irand(2);
        switch (rand)
        {
        case 0:
          wp->panels[i].iconId = 1600; // FP
          wp->panels[i].entry->iconId = 1600; // FP
          break;
        case 1:
          wp->panels[i].iconId = 1601; // HP
          wp->panels[i].entry->iconId = 1601; // HP
          break;
        case 2:
          wp->panels[i].iconId = 1602; // Poison
          wp->panels[i].entry->iconId = 1602; // Poison
          break;
        }
        wp->panels[i].flags = 0x3;
        wp->panels[i].fadeIn = 30;
      }
      if (wp->panels[i].flags == 0x2 && wp->panels[i].fadeOut == 0)
      {
        wp->panels[i].waitTimer = 2 * (u32)gp->fps;
        wp->panels[i].flags = 0x0;
      }
      if (wp->panels[i].flags == 0x3 && wp->panels[i].fadeIn == 0)
      {
        wp->panels[i].waitTimer = 2 * (u32)gp->fps + spm::system::irand(90);
        wp->panels[i].flags = 0x1;
      }
      if (wp->panels[i].flags == 0x1 && wp->panels[i].waitTimer == 0)
      {
        wp->panels[i].fadeOut = 1 * (u32)gp->fps;
        wp->panels[i].fadeOut = wp->panels[i].fadeOut / 2;
        wp->panels[i].flags = 0x2;
      }
    }

  moveCursor(wp);
  
  if (wp->cursorFlags == 0x0 && wp->flags == 0x1)
  {
    u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
    switch (pressed)
    {
    case 0x1:
      wp->cursorDestIndex = moveInGrid(wp->cursorDestIndex, DOWN);
      wp->cursorFlags = 0x1;
      wp->cursorMoveFrames = 10;
      break;
    case 0x2:
      wp->cursorDestIndex = moveInGrid(wp->cursorDestIndex, UP);
      wp->cursorFlags = 0x1;
      wp->cursorMoveFrames = 10;
      break;
    case 0x4:
      wp->cursorDestIndex = moveInGrid(wp->cursorDestIndex, RIGHT);
      wp->cursorFlags = 0x1;
      wp->cursorMoveFrames = 10;
      break;
    case 0x8:
      wp->cursorDestIndex = moveInGrid(wp->cursorDestIndex, LEFT);
      wp->cursorFlags = 0x1;
      wp->cursorMoveFrames = 10;
      break;
    case 0x100:
      if (wp->panels[wp->cursorDestIndex].flags == 0x1 && wp->panels[wp->cursorDestIndex].iconId != 1602)
      {
        spm::spmario_snd::spsndSFXOn("SFX_I_AC_BUTTON_OK1");
        if (wp->panels[wp->cursorDestIndex].iconId == 1600)
        {
          wp->fp += 1;
        }
        if (wp->panels[wp->cursorDestIndex].iconId == 1601)
        {
          wp->hp += 1;
        }
      }
      else if (wp->panels[wp->cursorDestIndex].flags == 0x1 && wp->panels[wp->cursorDestIndex].iconId == 1602)
      {
        spm::spmario_snd::spsndSFXOn("SFX_I_AC_BUTTON_NG1");
        wp->poisoned = true;
        wp->poisonFrames = 100;
      }
      if (wp->panels[wp->cursorDestIndex].flags == 0x1)
      {
        wp->panels[wp->cursorDestIndex].fadeOut = 30;
        wp->panels[wp->cursorDestIndex].flags = 0x2;
      }
    break;
    default:
      break;
    }
    if (wp->cursorMoveFrames > 0 && wp->poisoned == true)
    {
      wp->cursorMoveFrames *= 4;
    }
  }
  if (wp->poisonFrames == 0)
  {
    wp->poisoned = false;
  }
  return;
}

void dispFunc(s8 camId, spm::acdrv::AcEntry *entry)
{
  spm::spmario::SpmarioGlobals *gp = spm::spmario::gp;
  PowerRefreshWork *wp = (PowerRefreshWork *)entry->acDefEntry;
  if (wp->flags != 0x3)
  {
    for (s32 i = 0; i < 9; i++)
    {
      if (wp->panels[i].flags != 0x0)
      {
        spm::icondrv::iconDisp(camId, wp->panels[i].entry);
      }
      if (wp->flags == 0x1)
      {
        spm::icondrv::iconDisp(camId, wp->panels[i].background);
      }
    }
    spm::icondrv::iconDisp(camId, wp->cursorEntry);
  }
  return;
}

void deleteFunc(spm::acdrv::AcEntry *entry)
{
  PowerRefreshWork *wp = (PowerRefreshWork *)entry->acDefEntry;
  for (s32 i = 0; i < 9; i++)
  {
    spm::icondrv::iconDelete(wp->panels[i].entry->name);
    spm::icondrv::iconDelete(wp->panels[i].background->name);
  }
  spm::icondrv::iconDelete(wp->cursorEntry->name);
  spm::memory::__memFree(spm::memory::Heap::HEAP_MAP, wp);
}

void power_refresh_main(){
  spm::acdrv::acdrv_acDefs[14].initFunc = initFunc;
  spm::acdrv::acdrv_acDefs[14].mainFunc = mainFunc;
  spm::acdrv::acdrv_acDefs[14].dispFunc = dispFunc;
  spm::acdrv::acdrv_acDefs[14].deleteFunc = deleteFunc;
  spm::acdrv::acdrv_acDefs[14].msgName = nullptr;
}

}