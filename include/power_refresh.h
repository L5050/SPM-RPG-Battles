#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"

#include <patch.h>
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
#include <spm/icondrv.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod
{

  typedef struct Panel
  {
    u32 flags; // TBD
    s32 iconId;  // Type of panel, sorted by Icon Id
    Vec3 pos;    // Position of the center of the panel
    u32 fadeIn;  // How much time in frames that the icon is spinning and fading in
    u32 fadeOut; // How much time in frames that the icon is spinning and fading out
    u32 waitTimer; // How much time in frames until the panel is refreshed
    spm::icondrv::IconEntry * entry; // Self explanatory
    spm::icondrv::IconEntry * background; // Background of the panel thats displayed when the panel is inactive
  };

  typedef struct PowerRefreshWork
  {
    u32 flags; // TBD
    u32 startTimer; // How much time in frames until the action command starts
    u32 timer; // How much time in frames until the action command ends
    u32 frameCounter;   // Current frame counter, compared against gp->frameCounter
    Panel panels[9];
    Vec3 pos; // Position of the center of the grid, should be the same as panels[4].pos
    spm::icondrv::IconEntry * cursorEntry; // Self explanatory
    s32 cursorDestIndex; // The panel that the cursor is moving towards
    u32 cursorFlags; // TBD
    u32 cursorMoveFrames; // Amount of frames until the cursor stops moving
    s32 hp;   // HP gained from the action command
    s32 fp;   // FP gained from the action command
    u32 poisonFrames; // Amount of frames the player is poisoned for
    bool poisoned; // Whether the player has clicked a poisoned panel recently or not
    // Note, we can just throw the hp and fp logic in the panel work function, or we can have it handled in the evtscript post action command, either works but I may prefer the former
  };

void power_refresh_main();

}