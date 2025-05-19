#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"

#include <patch.h>
#include <spm/rel/an.h>
#include <spm/evtmgr.h>
#include <spm/evt_ac.h>
#include <spm/evt_msg.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_paper.h>
#include <spm/evt_img.h>
#include <spm/evt_env.h>
#include <spm/evt_eff.h>
#include <spm/evt_snd.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_npc.h>
#include <spm/evt_door.h>
#include <spm/evt_case.h>
#include <spm/evt_pouch.h>
#include <spm/evt_seq.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/sp4_13.h>
#include <spm/rel/an.h>
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

#define NPC_TABLE_MAX 13

namespace mod {

typedef struct
{
  s32 tribeId;
  NPCTribeAnimDef* animDefs;
  const EvtScriptCode* attackScript;
  const EvtScriptCode* onHitScript;
  const EvtScriptCode* deathScript;
  const EvtScriptCode* onSpawnScript = nullptr;
} npc_rpg_data;

extern NPCTribeAnimDef animsKuribo[];

EVT_DECLARE_USER_FUNC(get_rpg_enemy_max_hp, 2)
EVT_DECLARE_USER_FUNC(get_rpg_enemy_attack_script, 2)
EVT_DECLARE_USER_FUNC(get_rpg_enemy_damage_script, 2)
EVT_DECLARE_USER_FUNC(get_rpg_enemy_death_script, 2)
EVT_DECLARE_USER_FUNC(get_rpg_enemy_info, 4)
EVT_DECLARE_USER_FUNC(kill_rpg_npc, 1)
EVT_DECLARE_USER_FUNC(evt_rpg_revive_enemy, 2)
EVT_DECLARE_USER_FUNC(set_npc_as_me, 1)
EVT_DECLARE_USER_FUNC(mobjChangeAnimPoseName, 2)
EVT_DECLARE(doopliss_attack)
EVT_DECLARE(doopliss_death)
EVT_DECLARE(superguard_stylish)
EVT_DECLARE(increase_stylish)

void npc_rpgdrv_main();

}
