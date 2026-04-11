#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"
#include "ring_menu.h"

#define __GNU_C__

#include <msl/string.h>
#include <msl/math.h>
#include <spm/rel/an.h>
#include <spm/acdrv.h>
#include <spm/evt_ac.h>
#include <spm/evtmgr.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/seqdef.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/system.h>
#include <wii/os/OSError.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;
using namespace spm::icondrv;
using namespace msl::math;

namespace mod
{

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

RingMenu battle_menu;
RingMenu partner_menu;

// ============================================================
// Icon animation functions from power refresh
// Power refresh didn't need quite as precise math
// But since circles are complicated we probably need it to be fairly precise?
// Idk lmao i dont understand this shit
// ============================================================

static void iconGlideTo(IconEntry* icon, u32 frames, f32 destX, f32 destY, f32 destZ)
{
    if (!icon || frames == 0)
        return;

    f32 diffX = destX - icon->position.x;
    if (fabsf(diffX) > 0.0001f)
        icon->position.x += diffX / (f32)frames;
    else
        icon->position.x = destX;

    f32 diffY = destY - icon->position.y;
    if (fabsf(diffY) > 0.0001f)
        icon->position.y += diffY / (f32)frames;
    else
        icon->position.y = destY;

    f32 diffZ = destZ - icon->position.z;
    if (fabsf(diffZ) > 0.0001f)
        icon->position.z += diffZ / (f32)frames;
    else
        icon->position.z = destZ;
}

static void iconRotateTo(IconEntry* icon, u32 frames, f32 destX, f32 destY, f32 destZ)
{
    if (!icon || frames == 0)
        return;

    f32 diffX = destX - icon->rotation.x;
    if (fabsf(diffX) > 0.0001f)
        icon->rotation.x += diffX / (f32)frames;
    else
        icon->rotation.x = destX;

    f32 diffY = destY - icon->rotation.y;
    if (fabsf(diffY) > 0.0001f)
        icon->rotation.y += diffY / (f32)frames;
    else
        icon->rotation.y = destY;

    f32 diffZ = destZ - icon->rotation.z;
    if (fabsf(diffZ) > 0.0001f)
        icon->rotation.z += diffZ / (f32)frames;
    else
        icon->rotation.z = destZ;
}

static void iconFadeTo(spm::icondrv::IconEntry * icon, u32 frames, u8 alpha)
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


static void iconZoomTo(IconEntry* icon, u32 frames, f32 scale)
{
    if (!icon || frames == 0)
        return;

    f32 diff = scale - icon->scale;

    if (fabsf(diff) > 0.0001f)
        icon->scale += diff / (f32)frames;
    else
        icon->scale = scale;
}

// ============================================================
// Meth helpers
// ============================================================

static f32 ringMenuWrapAngle(f32 angle)
{
    while (angle < 0.0f)
        angle += (2.0f * (f32)M_PI);

    while (angle >= (2.0f * (f32)M_PI))
        angle -= (2.0f * (f32)M_PI);

    return angle;
}

static f32 ringMenuAngleDiff(f32 target, f32 current)
{
    f32 diff = target - current;

    while (diff > (f32)M_PI)
        diff -= 2.0f * (f32)M_PI;

    while (diff < -(f32)M_PI)
        diff += 2.0f * (f32)M_PI;

    return diff;
}

static f32 ringMenuLerp(f32 a, f32 b, f32 t)
{
    return a + ((b - a) * t);
}

static f32 ringMenuGetAngleStep(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 0)
        return 0.0f;

    return (2.0f * (f32)M_PI) / (f32)menu->iconCount;
}

static f32 ringMenuGetTargetAngleForIndex(RingMenu* menu, s32 index)
{
    f32 angleStep = ringMenuGetAngleStep(menu);
    return menu->frontAngle - ((f32)index * angleStep);
}

// ============================================================
// Setup / Init
// ============================================================

void ringMenuInit(RingMenu* menu, f32 centerX, f32 centerY, f32 centerZ, f32 radiusX, f32 radiusY)
{
    if (!menu)
        return;

    msl::string::memset(menu, 0, sizeof(RingMenu));

    menu->centerX = centerX;
    menu->centerY = centerY;
    menu->centerZ = centerZ;

    menu->radiusX = radiusX;
    menu->radiusY = radiusY;
    menu->depthRadius = 0.0f;

    menu->ringAngle = 0.0f;
    menu->targetRingAngle = 0.0f;
    menu->selectedIndex = 0;

    menu->isScrolling = 0;

    menu->iconMoveFrames = 4;    // icon easing
    menu->ringRotateFrames = 4;  // ring easing

    menu->frontAngle = (f32)M_PI * 0.5f;

    menu->frameCounter = 0;
    menu->active = 1;
}

void ringMenuClear(RingMenu* menu)
{
    if (!menu)
        return;

    for (s32 i = 0; i < menu->iconCount; i++)
    {
      spm::icondrv::iconDelete(menu->icons[i]->name);
    }
    
    menu->iconCount = 0;
    menu->selectedIndex = 0;
    menu->ringAngle = 0.0f;
    menu->targetRingAngle = 0.0f;
    menu->isScrolling = 0;
}

void ringMenuAddIcon(RingMenu* menu, IconEntry* icon)
{
    if (!menu || !icon)
        return;

    if (menu->iconCount >= RINGMENU_MAX_ICONS)
        return;

    spm::icondrv::iconFlagOn(icon->name, 2);
    icon->position.x = menu->centerX;
    icon->position.y = menu->centerY;
    icon->position.z = menu->centerZ;
    menu->icons[menu->iconCount] = icon;
    menu->iconCount++;
    menu->radiusX += 1.0f;
    menu->radiusY += 1.0f;
    return;
}

void ringMenuFinalize(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 0)
        return;

    menu->selectedIndex = 0;
    menu->ringAngle = ringMenuGetTargetAngleForIndex(menu, menu->selectedIndex);
    menu->targetRingAngle = menu->ringAngle;
    menu->ringAngle = ringMenuWrapAngle(menu->ringAngle);
    menu->targetRingAngle = ringMenuWrapAngle(menu->targetRingAngle);
}

// ============================================================
// Selection helpers
// ============================================================

IconEntry* ringMenuGetSelectedIcon(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 0)
        return NULL;

    if (menu->selectedIndex < 0 || menu->selectedIndex >= menu->iconCount)
        return NULL;

    return menu->icons[menu->selectedIndex];
}

void ringMenuScrollRight(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 1)
        return;

    menu->selectedIndex++;
    if (menu->selectedIndex >= menu->iconCount)
        menu->selectedIndex = 0;

    menu->targetRingAngle = ringMenuGetTargetAngleForIndex(menu, menu->selectedIndex);
    menu->targetRingAngle = ringMenuWrapAngle(menu->targetRingAngle);
    menu->isScrolling = 1;
    return;
}

void ringMenuScrollLeft(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 1)
        return;

    menu->selectedIndex--;
    if (menu->selectedIndex < 0)
        menu->selectedIndex = menu->iconCount - 1;

    menu->targetRingAngle = ringMenuGetTargetAngleForIndex(menu, menu->selectedIndex);
    menu->targetRingAngle = ringMenuWrapAngle(menu->targetRingAngle);
    menu->isScrolling = 1;
    return;
}

// ============================================================
// Internal update
// ============================================================

static void ringMenuUpdateRotation(RingMenu* menu)
{
    if (!menu || !menu->isScrolling)
        return;

    f32 diff = ringMenuAngleDiff(menu->targetRingAngle, menu->ringAngle);

    if (fabsf(diff) < 0.001f)
    {
        menu->ringAngle = menu->targetRingAngle;
        menu->ringAngle = ringMenuWrapAngle(menu->ringAngle);
        menu->isScrolling = 0;
        return;
    }

    menu->ringAngle += diff / (f32)menu->ringRotateFrames;
    menu->ringAngle = ringMenuWrapAngle(menu->ringAngle);
}

static void ringMenuUpdateIcons(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 0)
        return;

    f32 angleStep = ringMenuGetAngleStep(menu);

    for (s32 i = 0; i < menu->iconCount; i++)
    {
        IconEntry* icon = menu->icons[i];
        if (!icon)
            continue;

        // This icon's position on the ring
        f32 iconAngle = menu->ringAngle + ((f32)i * angleStep);
        f32 cosA = cosf(iconAngle);
        f32 sinA = sinf(iconAngle);

        f32 targetX = menu->centerX + (cosA * menu->radiusX);
        f32 targetY = menu->centerY + (sinA * menu->radiusY);
        f32 targetZ = menu->centerZ;

        // --------------------------------------------------------
        // Front/back amount
        // 0.0 = back, 1.0 = front
        // --------------------------------------------------------
        f32 depthT = (sinA + 1.0f) * 0.5f;

        // --------------------------------------------------------
        // Visual tuning
        // --------------------------------------------------------
        f32 targetScale = ringMenuLerp(0.70f, 1.10f, depthT);
        u8 targetAlpha = (u8)(ringMenuLerp(110.0f, 255.0f, depthT));

        // --------------------------------------------------------
        // Selected/front icon emphasis
        // --------------------------------------------------------
        if (i == menu->selectedIndex)
        {
            f32 pulse = sinf((f32)menu->frameCounter * 0.18f) * 0.03f;
            targetScale += 0.18f + pulse;
            targetY -= 3.0f;
        } else {
          targetAlpha /= 2;
        }

        // --------------------------------------------------------
        // Animate toward targets
        // --------------------------------------------------------
        iconZoomTo(icon, menu->iconMoveFrames, targetScale);
        iconFadeTo(icon, menu->iconMoveFrames, targetAlpha);
        spm::icondrv::iconDisp(11, icon);
        if (menu->isSelectFree && i == menu->selectedIndex)
        {
          
        } else {
          iconGlideTo(icon, menu->iconMoveFrames, targetX, targetY, targetZ);
        }
    }
}

// Call once every frame
void ringMenuDisplay(RingMenu* menu)
{
    if (!menu || !menu->active)
        return;

    if (menu->iconCount <= 0)
        return;

    menu->frameCounter++;

    ringMenuUpdateRotation(menu);

    ringMenuUpdateIcons(menu); 

}

void ringMenuSnapToCurrentState(RingMenu* menu)
{
    if (!menu || menu->iconCount <= 0)
        return;

    f32 angleStep = ringMenuGetAngleStep(menu);

    for (s32 i = 0; i < menu->iconCount; i++)
    {
        IconEntry* icon = menu->icons[i];
        if (!icon)
            continue;

        f32 iconAngle = menu->ringAngle + ((f32)i * angleStep);

        f32 cosA = cosf(iconAngle);
        f32 sinA = sinf(iconAngle);

        f32 x = menu->centerX + (cosA * menu->radiusX);
        f32 y = menu->centerY + (sinA * menu->radiusY);
        f32 z = menu->centerZ;

        f32 depthT = (sinA + 1.0f) * 0.5f;

        f32 scale = ringMenuLerp(0.70f, 1.10f, depthT);
        u8 alpha = (u8)(ringMenuLerp(110.0f, 255.0f, depthT));

        if (i == menu->selectedIndex)
        {
            scale += 0.18f;
            y -= 6.0f;
            alpha = 255;
        }

        icon->position.x = x;
        icon->position.y = y;
        icon->position.z = z;

        icon->scale = scale;
        icon->colour.a = alpha;
        icon->alpha = alpha;
    }
}

s32 ring_delete_battle(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  ringMenuClear(&battle_menu);
  return 2;
}

s32 ring_init_battle(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  spm::mario::MarioWork * mario = spm::mario::marioGetPtr();
  ringMenuInit(&battle_menu, -115.0f, -30.0f, 0.0f, 31.0f, 34.0f);
  ringMenuAddIcon(&battle_menu, iconEntryAutoname(1605)); // Attack
  ringMenuAddIcon(&battle_menu, iconEntryAutoname(0x31)); // Special Moves

  u32 pixlCount = 0;
  for (u32 i = 0; i < 16; i++)
  {
    spm::mario_pouch::PouchCharOrPixlInfo * pixls = spm::mario_pouch::pouchGetPixlInfo(i);
    if (pixls != nullptr && pixls->selectable)
    {
      pixlCount += 1;
    }
  }
  if(pixlCount > 1)
  {
    ringMenuAddIcon(&battle_menu, iconEntryAutoname(0x8C)); // Pixls
  }
  ringMenuAddIcon(&battle_menu, iconEntryAutoname(0x7C)); // Items

  u32 charCount = 0;
  for (u32 i = 0; i < 4; i++)
  {
    spm::mario_pouch::PouchCharOrPixlInfo * characters = spm::mario_pouch::pouchGetCharInfo(i);
    if (characters != nullptr && characters->selectable)
    {
      charCount += 1;
    }
  }
  if(charCount > 1)
  {
    ringMenuAddIcon(&battle_menu, iconEntryAutoname(1607)); // Swap Characters
  }

  ringMenuAddIcon(&battle_menu, iconEntryAutoname(1606)); // Run Away

  ringMenuFinalize(&battle_menu);
  //ringMenuSnapToCurrentState(&battle_menu);
  return 2;
}

s32 jumpFrames = 0;

void battle_select(s32 i)
{
  spm::npcdrv::NPCEntry *npc = nullptr;
    // Check NPC is not dead
    if ((spm::an2_08::rpgdrv_wp->rpgNpcInfo[i].flags & 0x8000) == 0)
    {
      switch (i)
      {
      case 0:
        npc = spm::npcdrv::npcNameToPtr_NoAssert("npc1");
        break;
      case 1:
        npc = spm::npcdrv::npcNameToPtr_NoAssert("npc2");
        break;
      case 2:
        npc = spm::npcdrv::npcNameToPtr_NoAssert("npc3");
        break;
      }

      if (npc != nullptr)
      {
        // Get screen position
        wii::mtx::Vec3 pos;
        wii::mtx::Vec3 npcPos = npc->position;
        
        if (getRpgTribeID(i) == 313)
          npcPos.y = 0;
        if (getRpgTribeID(i) == 450 && i == 0)
          npcPos.x -= 50.0;
        if (getRpgTribeID(i) == 450 && i == 1)
          npcPos.x += 50.0;
        spm::camdrv::getScreenPoint(&npcPos, &pos);
        pos.y += 60.0f;
        iconGlideTo(battle_menu.icons[battle_menu.selectedIndex], jumpFrames, pos.x, pos.y, pos.z);
        if (jumpFrames > 0)
        {
          jumpFrames -= 1;
        }
      }
      return;
    }
    return;
  }

  static s32 validateIndex(s32 index)
  {
    spm::npcdrv::NPCEntry *npc = nullptr;
    // Check NPC is not dead, reset to correct index if it is
    bool indexFiller[3];
    for (s32 i = 0; i < 3; i++)
    {
      if ((spm::an2_08::rpgdrv_wp->rpgNpcInfo[i].flags & 0x8000) == 0)
      {
        indexFiller[i] = true;
      } else {
        indexFiller[i] = false;
      }
    }
    if (!indexFiller[index])
    {
      index = -1;
      for (s32 i = 0; i < 3; i++)
      {
        if (indexFiller[i] && i != index)
        {
          index = i;
          break;
        }
      }
    }
    return index;
  }

  static s32 jumpAction(spm::evtmgr::EvtEntry *evtEntry, bool firstRun, u32 pressed)
  {
    spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 id = spm::evtmgr_cmd::evtGetValue(evtEntry, args[2]);
    s32 indexTest = validateIndex(id);
    if (indexTest > 0 && indexTest != id)
    {
      id = indexTest;
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], id);
    }
    battle_select(id);
    if (pressed & 0x8)
    {
      spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE2");
      id -= 1;
      if (id < 0)
      {
        id = 2;
      }
      if ((spm::an2_08::rpgdrv_wp->rpgNpcInfo[id].flags & 0x8000) != 0)
      {
        do
        {
          id -= 1;
          if (id < 0)
          {
            id = 2;
          }
        } while ((spm::an2_08::rpgdrv_wp->rpgNpcInfo[id].flags & 0x8000) != 0);
      }
      jumpFrames = 15;
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], id);
      return 2;
    }
    if (pressed & 0x4)
    {
      spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE2");
      id += 1;
      if (id > 2)
      {
        id = 0;
      }
      if ((spm::an2_08::rpgdrv_wp->rpgNpcInfo[id].flags & 0x8000) != 0)
      {
        do
        {
          id += 1;
          if (id > 2)
          {
            id = 0;
          }
        } while ((spm::an2_08::rpgdrv_wp->rpgNpcInfo[id].flags & 0x8000) != 0);
      }
      jumpFrames = 15;
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], id);
      return 2;
    }
    if (pressed & 0x100)
    {
      spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_DESIDE1");
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], id);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
      return 2;
    }
    if (pressed & 0x200)
    {
      battle_menu.isSelectFree = false;
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], 0);
      spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CANCEL1");
      return 2;
    }
    return 2;
  }

s32 ring_battle_main(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  if (battle_menu.iconMoveFrames >= 15)
  {
    return 2;
  }
  u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);

  if (battle_menu.isSelectFree)
  {
    switch (battle_menu.icons[battle_menu.selectedIndex]->iconId)
    {
    case 1605:
      return jumpAction(evtEntry, firstRun, pressed);
      break;
    }
    return 2;
  }

  if (pressed & 0x100) {
    spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_DESIDE1");
    battle_menu.isSelectFree = true;
    if (battle_menu.icons[battle_menu.selectedIndex]->iconId == 1605)
    {
      jumpFrames = 15;
      u8 npcIndex = 0;
      for (u8 i = 0; i < 3; i++)
      {
        spm::npcdrv::NPCEntry *npc = nullptr;
        switch (i)
        {
        case 0:
          npc = spm::npcdrv::npcNameToPtr_NoAssert("npc1");
          break;
        case 1:
          npc = spm::npcdrv::npcNameToPtr_NoAssert("npc2");
          break;
        case 2:
          npc = spm::npcdrv::npcNameToPtr_NoAssert("npc3");
          break;
        }
        if (npc)
        {
          npcIndex = i;
          break;
        }
      }
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], npcIndex);  
    }
    if (battle_menu.icons[battle_menu.selectedIndex]->iconId == 0x31)
    {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
    }
    if (battle_menu.icons[battle_menu.selectedIndex]->iconId == 0x8C)
    {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 2);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
    }
    if (battle_menu.icons[battle_menu.selectedIndex]->iconId == 0x7C)
    {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 3);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
    }
    if (battle_menu.icons[battle_menu.selectedIndex]->iconId == 1607)
    {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 4);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
    }
    if (battle_menu.icons[battle_menu.selectedIndex]->iconId == 1606)
    {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 5);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
    }
    return 2;
  }

  if (pressed & 0x8) {
    ringMenuScrollRight(&battle_menu);
    spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE2");
    return 2;
  }
  if (pressed & 0x4) {
    ringMenuScrollLeft(&battle_menu);
    spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE2");
    return 2;
  }
  return 2;
}

s32 ring_battle_pixl_main(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);

  if (battle_menu.isSelectFree)
  {
    return jumpAction(evtEntry, firstRun, pressed);
  } else {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], -1);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[5], 1);
  }
  return 2;
}

s32 ring_init_battle_pixl(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  s32 id = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);

  spm::mario::MarioWork * mario = spm::mario::marioGetPtr();
  ringMenuInit(&battle_menu, -115.0f, -30.0f, 0.0f, 31.0f, 34.0f);
  ringMenuAddIcon(&battle_menu, iconEntryAutoname(id));

  ringMenuFinalize(&battle_menu);

  battle_menu.isSelectFree = true;

  jumpFrames = 15;
  u8 npcIndex = 0;
  for (u8 i = 0; i < 3; i++)
  {
    spm::npcdrv::NPCEntry *npc = nullptr;
    switch (i)
    {
    case 0:
      npc = spm::npcdrv::npcNameToPtr_NoAssert("npc1");
      break;
    case 1:
      npc = spm::npcdrv::npcNameToPtr_NoAssert("npc2");
      break;
    case 2:
      npc = spm::npcdrv::npcNameToPtr_NoAssert("npc3");
      break;
    }
    if (npc)
    {
      npcIndex = i;
      break;
    }
  }
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], npcIndex);

  return 2;
}

void battle_menu_display(s32 camid, RingMenu * menu)
{
  return ringMenuDisplay(&battle_menu);
}

s32 ring_display_battle(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  //ringMenuDisplay(&battle_menu);
  spm::dispdrv::dispEntry(spm::camdrv::CAM_ID_2D, 11, 0, battle_menu_display, (void *)&battle_menu);
  return 2;
}

void ring_menu_main()
{
  spm::acdrv::acdrv_acDefs[3].msgName = spm::acdrv::acdrv_acDefs[2].msgName;
}

}