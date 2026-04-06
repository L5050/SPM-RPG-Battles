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
using namespace spm::icondrv;

namespace mod
{

#define RINGMENU_MAX_ICONS 16

typedef struct
{
    // icon list
    IconEntry* icons[RINGMENU_MAX_ICONS];
    s32 iconCount;

    // center of ring on screen/world
    f32 centerX;
    f32 centerY;
    f32 centerZ;

    // ring shape
    f32 radiusX;
    f32 radiusY;
    f32 depthRadius;

    // current rotation of ring
    f32 ringAngle;

    // where ring wants to rotate to
    f32 targetRingAngle;

    // currently selected logical icon
    s32 selectedIndex;

    // icon easing speed
    u32 iconMoveFrames;
    u32 ringRotateFrames;

    // front-facing angle (PI/2 = bottom/front if using sin)
    f32 frontAngle;

    // optional cosmetic frame counter
    u32 frameCounter;

    // animation state
    bool isScrolling;

    // whether menu is active
    bool active;

    // whether the selected icon is in free cam or not
    bool isSelectFree;
} RingMenu;

EVT_DECLARE_USER_FUNC(ring_init_battle, 0) 
EVT_DECLARE_USER_FUNC(ring_display_battle, 0)
EVT_DECLARE_USER_FUNC(ring_battle_main, 6)

void ring_menu_main();

}