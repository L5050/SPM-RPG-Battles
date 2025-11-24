#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "ip_badges.h"
#include <spm/evtmgr.h>
#include <spm/icondrv.h>
#include <spm/rel/an2_08.h>
#include <wii/tpl.h>
namespace mod {

#define MOD_VERSION "SPM-RPG-Battles"

extern "C" {

EVT_DECLARE(evt_memcard_save)

}

extern s32 *fp;
extern s32 *maxFp;
extern s32 *bp;
extern s32 *maxBp;
extern bool gIsDolphin;
extern bool gIsRiivolution;
extern bool gIsPatchedDisc;
extern bool gIs4_3;
extern bool succeededActionCommand;
extern bool superGuard;

s32 getRpgTribeID(s32 index);
bool IsNpcActive(s32 index);
extern s32( * rpgHandleMenu)(s32, spm::an2_08::RpgMenuOption*);
bool checkBadgeTechnique(ip::BadgeId id);
spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction);
void mobj_main();
s32 _getFP();
void _setFP(s32 count);
s32 _getMaxFP();
spm::icondrv::IconEntry *iconEntryAutoname(s32 iconId);
EVT_DECLARE_USER_FUNC(setRpgTribeID, 2)
EVT_DECLARE_USER_FUNC(compareStrings, 3)
EVT_DECLARE_USER_FUNC(mario_set_scale, 3)
EVT_DECLARE_USER_FUNC(rpg_set_dialogue, 1)
EVT_DECLARE_USER_FUNC(mario_reset_rotation, 0)
EVT_DECLARE_USER_FUNC(get_rpg_name_by_id, 2)
EVT_DECLARE_USER_FUNC(evt_npc_entry_autoname, 3)
EVT_DECLARE_USER_FUNC(evt_item_entry_autoname, -1)
EVT_DECLARE_USER_FUNC(get_attack_strength, 2)
EVT_DECLARE_USER_FUNC(msgSearchTribeToTattle, 2)
EVT_DECLARE_USER_FUNC(evt_offscreen_set_type, 2)
EVT_DECLARE_USER_FUNC(check_guards, 3)
EVT_DECLARE_USER_FUNC(mario_get_hitbox_width, 1)
EVT_DECLARE_USER_FUNC(addNpcToItemEvent, 1)
EVT_DECLARE_USER_FUNC(check_for_boss_fight, 1)
EVT_DECLARE_USER_FUNC(check_slim, 1)
EVT_DECLARE_USER_FUNC(slim_off_on, 1)
EVT_DECLARE_USER_FUNC(check_stylish, 2)
EVT_DECLARE_USER_FUNC(npc_get_bounding_box, 5)

EVT_DECLARE(evt_memcard_save)

  const char stg7_2_133_2_002[32] = "<dq>\n"
  "<p>\n"
  "What will you do?\n"
  "<o>\n";

  const char stg7_2_133_2_023[70]= "<dkey><wait 500></dkey>\n"
  "<p>\n"
  "Urg... %s is too \nheavy to lift!\n"
  "<k>\n"
  "<o>\n";

void savemgr_main();
void main();

}
