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
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod
{

  typedef struct Panel
  {
    s32 iconId;  // Type of panel, sorted by Icon Id
    Vec3 pos;    // Position of the center of the panel
    f32 fadeIn;  // Some kind of variable that determines whether it needs to be spinning and fading in
    f32 fadeOut; // Some kind of variable that determines whether it needs to be spinning and fading out
    void *func;  // Logic for when the Panel is clicked, possibly pass the AcEntry through so that it can modify PowerRefreshWork
  };

  typedef struct PowerRefreshWork
  {
    f32 timer; // How much time until the action command ends
    Panel panels[9];
    Vec3 pos; // Position of the center of the grid, should be the same as panelWp[4].pos
    s32 hp;   // HP gained from the action command
    s32 fp;   // FP gained from the action command
    // Note, we can just throw the hp and fp logic in the panel work function, or we can have it handled in the evtscript post action command, either works but I may prefer the former
  };

}