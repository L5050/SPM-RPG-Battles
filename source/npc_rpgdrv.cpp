#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "doopliss.h"
#include "pukupuku.h"
#include "kuribo.h"
#include "octa.h"
#include "sproing.h"
#include "koopa.h"
#include "sinno.h"
#include "met.h"
#include "oChunks.h"
#include "cherbil.h"
#include "fracktail.h"

#include <spm/rel/an.h>
#include <spm/animdrv.h>
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
#include <spm/rel/mi4.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <msl/string.h>
#include <wii/os/OSError.h>
#include <patch.h>
#include <string>

using namespace spm::npcdrv;

namespace mod {

npc_rpg_data npcDataTable[NPC_TABLE_MAX];

NPCTribeAnimDef animsKuribo[] = {
    {0, "kuribo_S_1"},
    {1, "kuribo_W_1"},
    {2, "kuribo_R_1"},
    {3, "kuribo_T_1"},
    {4, "kuribo_D_1"},
    {7, "kuribo_D_1"},
    {6, "kuribo_D_1"},
    {10, "kuribo_Z_1"},
    {11, "kuribo_Z_1"},
    {14, "kuribo_N_1"},
    {-1, nullptr}
  };

NPCTribeAnimDef animsParaKuribo[] = {
    {0, "pata_kuribo_S_1"},
    {1, "pata_kuribo_W_1"},
    {2, "pata_kuribo_R_1"},
    {3, "pata_kuribo_T_1"},
    {4, "pata_kuribo_D_1"},
    {7, "pata_kuribo_D_1"},
    {6, "pata_kuribo_D_1"},
    {10, "pata_kuribo_Z_1"},
    {11, "pata_kuribo_Z_1"},
    {14, "pata_kuribo_N_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsSinno[] = {
    {0, "SIS_S_1"},
    {1, "SIS_W_1"},
    {2, "SIS_R_1"},
    {3, "SIS_T_1"},
    {4, "SIS_D_1"},
    {7, "SIS_A_1a"},
    {6, "SIS_D_1"},
    {10, "SIS_Z_1"},
    {11, "SIS_Z_1"},
    {14, "SIS_N_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsOcta2[] = {
    {0, "S_1"},
    {1, "W_1"},
    {2, "R_1"},
    {3, "T_1"},
    {4, "D_1"},
    {7, "W_1a"},
    {6, "D_1"},
    {10, "Z_1"},
    {11, "Z_1"},
    {14, "N_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsOctar[] = {
    {0, "S_1"},
    {1, "W_1"},
    {2, "A_1"},
    {3, "T_1"},
    {4, "D_1"},
    {7, "W_1a"},
    {6, "D_1"},
    {10, "Z_1"},
    {11, "Z_1"},
    {14, "N_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsTecti[] = {
    {0, "S_1"},
    {1, "J_1"},
    {2, "J_2"},
    {3, "T_1"},
    {4, "D_1"},
    {7, "J_3"},
    {6, "J_4"},
    {14, "N_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsFrackle[] = {
    {0, "S_2"},
    {1, "W_2"},
    {4, "D_1"},
    {14, "N_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsCharge[] = {
    {0, "Z_1"},
    {1, "S_1"},
    {-1, nullptr}
  };

  static inline s32 getDataTableIndex(s32 tribeId)
  {
    for (s32 i = 0; i < NPC_TABLE_MAX; i++)
    {
      if (npcDataTable[i].tribeId == tribeId) {
        return i;
      }
    }
    wii::os::OSReport("Invalid TribeId Loaded, loading goomba instead\n");
    return 0;
  }

s32 mobjChangeAnimPoseName(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtScriptCode *args = evtEntry->pCurData;
        char *mobjName = (char *)spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        char *animPoseName = (char *)spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        spm::mobjdrv::MobjEntry *mobj = spm::mobjdrv::mobjNameToPtrNoAssert(mobjName);
        mobj->animPoseId = spm::animdrv::animPoseEntry(animPoseName, 0);
        return 2;
    }

  s32 get_rpg_enemy_attack_script(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    s32 index = getDataTableIndex(tribeId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)npcDataTable[index].attackScript);
    //wii::os::OSReport("Getting attack script\n");
    return 2;
  }

  s32 get_rpg_enemy_max_hp(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 index = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
    s32 tribe = getRpgTribeID(index);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], npcTribes[tribe].maxHp);
    return 2;
  }

  s32 set_npc_as_me(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    const char * name = (const char*)spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
    NPCEntry * entry = npcNameToPtr(name);
    evtEntry->ownerNPC = entry;
    return 2;
  }


  s32 get_rpg_enemy_damage_script(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    s32 index = getDataTableIndex(tribeId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)npcDataTable[index].onHitScript);
    //wii::os::OSReport("Getting onhit script\n");
    return 2;
  }

  s32 get_rpg_enemy_charge_odds(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    s32 index = getDataTableIndex(tribeId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)npcDataTable[index].chargeOdds);
    //wii::os::OSReport("Getting charge odds\n");
    return 2;
  }

  s32 get_rpg_enemy_throw_script(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    s32 index = getDataTableIndex(tribeId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)npcDataTable[index].onThrowScript);
    //wii::os::OSReport("Getting throw script\n");
    return 2;
  }

  s32 get_rpg_enemy_death_script(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    s32 index = getDataTableIndex(tribeId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)npcDataTable[index].deathScript);
    //wii::os::OSReport("Getting death script\n");
    return 2;
  }

  s32 evt_rpg_revive_enemy(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 index = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
    s32 newHealth = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
    spm::an2_08::rpgdrv_wp->rpgNpcInfo[index].maxHp = newHealth;
    return 2;
  }

  s32 get_rpg_enemy_info(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(args[0]);

    if (!IsNpcActive(args[0])) {
      s32 index = args[0];
      //debug only wii::os::OSReport("NPC is inactive, killing NPC\n");
      spm::an2_08::rpgdrv_wp->rpgNpcInfo[index].maxHp = -1;
      spm::an2_08::rpgdrv_wp->rpgNpcInfo[index].killXp = 0;
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], 1);
      spm::an2_08::evt_rpg_enemy_take_damage(evtEntry, firstRun);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], -1);
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], -1);
      return 2;
    }
    s32 index = getDataTableIndex(tribeId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)npcDataTable[index].animDefs);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], (s32)npcTribes[tribeId].animPoseName);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[3], (s32)npcDataTable[index].onSpawnScript);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[4], tribeId);

    if (tribeId == 313) {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], (s32)"e_zun_all");
    }

    if (firstRun == false) {}
    return 2;
  }

  s32 get_rpg_enemy_card_id(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], spm::npcdrv::npcTribes[tribeId].catchCardItemId);
    if (firstRun == false) {}
    return 2;
  }

  s32 get_rpg_enemy_card_count(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 tribeId = getRpgTribeID(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], spm::npcdrv::npcTribes[tribeId].catchCardItemId);
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId));
    if (firstRun == false) {}
    return 2;
  }

  s32 set_respawn_pos(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    f32 x = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[0]);
    f32 y = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[1]);
    f32 z = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[2]);
    spm::mario::marioGetPtr()->respawnPosition.x = x;
    spm::mario::marioGetPtr()->respawnPosition.y = y;
    spm::mario::marioGetPtr()->respawnPosition.z = z;
    return 2;
  }

  s32 calc_mario_position(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    const char *mapName = reinterpret_cast<const char *>(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
    wii::os::OSReport("%s name\n", mapName);

    const char * comparison = "he1_01";
    const char *result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], -970.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he1_02";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -1816.0;
      if (spm::mario::marioGetPtr()->position.x >= -1716.0)
      {
        x = 1100.0;
      }
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he1_04";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -1690.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he2_01";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -15.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he3_01";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -1350.0;
      if (spm::mario::marioGetPtr()->position.x >= -1000.0)
      {
        if (spm::mario::marioGetPtr()->position.x <= 0.0)
        {
          x = -135.0;
        } else {
          x = 2125.0;
        }
      }
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he3_03";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -1560.0;
      if (spm::mario::marioGetPtr()->position.x >= -1110.0)
      {
        if (spm::mario::marioGetPtr()->position.x <= 0.0)
        {
          x = -690.0;
        } else {
          x = 1225.0;
        }
      }
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he4_02";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -387.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he4_06";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -75.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "he4_07";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = 86.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "mi1_01";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -2605.0;
      if (spm::mario::marioGetPtr()->position.x >= -600.0)
      {
        x = -450.0;
      }
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "mi1_08";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -830.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "mi1_05";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -145.0;
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    comparison = "mi1_10";
    result = msl::string::strstr(mapName, comparison);
    if (result != 0)
    {
      f32 x = -1030.0;
      if (spm::mario::marioGetPtr()->position.x > 0.0)
      {
        x = 375.0;
      }
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[1], x);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[2], 0.0);
      spm::evtmgr_cmd::evtSetFloat(evtEntry, args[3], 0.0);
      return 2;
    }
    return 2;
  }

  s32 kill_rpg_npc(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;

    s32 index = args[0];
    //wii::os::OSReport("NPC is inside map bounding box, killing NPC\n");
    spm::an2_08::rpgdrv_wp->rpgNpcInfo[index].maxHp = 0;
    spm::an2_08::rpgdrv_wp->rpgNpcInfo[index].killXp = 0;
    spm::an2_08::rpgdrv_wp->rpgNpcInfo[index].flags = 0x8000;
    if (firstRun == false) {}
    return 2;
  }

  EVT_BEGIN(increase_stylish)
    IF_SMALL(UW(6), 4)
      ADD(UW(6), 1)
    END_IF()
  RETURN()
  EVT_END()

  EVT_BEGIN(superguard_stylish)
  USER_FUNC(ac_success_reset)
  USER_FUNC(enable_disable_stylish, 1)
  WAIT_FRM(10)
  INLINE_EVT()
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 180, 11)
  END_INLINE()
  USER_FUNC(check_stylish, 10, LW(11))
  IF_EQUAL(LW(11), 1)
      USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(5))
      ADD(LW(5), 50)
      USER_FUNC(spm::an2_08::evt_rpg_add_xp, LW(5))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU1"))
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), -250, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
      RUN_CHILD_EVT(increase_stylish)
      INLINE_EVT()
          SET(LW(11), 0)
          SETF(LW(10), FLOAT(0.0))
          DO(0)
            USER_FUNC(mario_rotate, LW(10))
            ADD(LW(11), 1)
            ADDF(LW(10), FLOAT(25.0))
            WAIT_FRM(1)
            IF_EQUAL(LW(11), 20)
              DO_BREAK()
            END_IF()
          WHILE()
      END_INLINE()
      INLINE_EVT()
        WAIT_FRM(4)
        USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_3"), 500)
      END_INLINE()
      INLINE_EVT()
        WAIT_FRM(22)
        USER_FUNC(enable_disable_stylish, 1)
      END_INLINE()
      USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 700)
      USER_FUNC(check_stylish, 20, LW(11))
      IF_EQUAL(LW(11), 1)
          USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(5))
          ADD(LW(5), 50)
          USER_FUNC(spm::an2_08::evt_rpg_add_xp, LW(5))
          USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU1"))
          USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), -250, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
          //RUN_CHILD_EVT(increase_stylish)
          SET(LW(5), UW(4))
          INLINE_EVT()
              SET(LW(11), 0)
              SETF(LW(10), FLOAT(0.0))
              DO(0)
              USER_FUNC(mario_rotate, LW(10))
              ADD(LW(11), 1)
              ADDF(LW(10), FLOAT(25.0))
              WAIT_FRM(1)
              IF_EQUAL(LW(11), 20)
                  DO_BREAK()
              END_IF()
              WHILE()
          END_INLINE()
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_1"), 500)
          USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 500)
          WAIT_FRM(2)
          USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(5))
          SWITCH(LW(5))
            CASE_EQUAL(2)
              USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_5"), 0)
            CASE_EQUAL(3)
              USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_6"), 0)
            CASE_ETC()
              USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_7"), 0)
          END_SWITCH()
      END_IF()
  ELSE()
    SET(LW(5), UW(4))
    USER_FUNC(spm::evt_mario::evt_mario_pos_change, LW(5), LW(7), FLOAT(180.0))
  END_IF()
  WAIT_FRM(3)
  USER_FUNC(spm::evt_mario::evt_mario_direction_face, 90, 200)
  RETURN()
EVT_END()

EVT_BEGIN(npc_charge)
  INLINE_EVT_ID(LW(10))
    SET(LW(9), FLOAT(0.0))
    DO(0)
    ADDF(LW(9), FLOAT(18.0))
    USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, LW(9), EVT_NULLPTR)
    WAIT_FRM(1)
    WHILE()
  END_INLINE()
  USER_FUNC(evt_npc_entry_autoname, PTR("EFF_batug_tue"), 0, LW(14))
  USER_FUNC(spm::evt_npc::evt_npc_set_property, LW(14), 14, PTR(animsCharge))
  USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
  USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(14), LW(5), LW(6), LW(7))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(14), 1, 1)
  USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, LW(14), 1)
  USER_FUNC(spm::evt_npc::evt_npc_delete, LW(14))
  DELETE_EVT(LW(10))
  USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, FLOAT(0.0), EVT_NULLPTR)
  USER_FUNC(spm::evt_npc::evt_npc_set_animpose_disp_callback, LW(15), PTR(spm::mi4::mi4MimiHolographicEffect), 0)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_POWERUP1"))
  USER_FUNC(spm::evt_npc::evt_npc_jump_to, LW(15), LW(5), LW(6), LW(7), 100, FLOAT(200.0))
  USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_EVT_HELWANWAN_POWERUP1"))
RETURN()
EVT_END()

  void npc_rpgdrv_main()
  {
    npcDataTable[0] = {0, animsKuribo, 10, kuribo_attack, nullptr, nullptr};
    kuribo_main();
    npcDataTable[1] = {125, animsOcta2, 10, octa_attack, nullptr, nullptr};
    doopliss_main();
    npcDataTable[2] = {529, getDooplissAnims(), 0, doopliss_attack, nullptr, doopliss_death}; // Doopliss
    koopa_main();
    npcDataTable[3] = {11, getKoopaAnims(), 10, koopa_attack, koopa_onhit, nullptr}; // Green Koopa Troopa
    sproing_main();
    npcDataTable[4] = {134, animsTecti, 50, sproing_oing_attack, nullptr, nullptr}; // Sproing-Oing
    npcDataTable[5] = {126, animsOctar, 10, octar_attack, nullptr, nullptr}; // Squig
    npcDataTable[6] = {20, getParaKoopaAnims(), 10, para_koopa_attack, para_koopa_onhit, nullptr}; // Parakoopa
    npcDataTable[7] = {14, getKoopaAnims(), 10, koopa_attack, koopa_onhit, nullptr}; // Red Koopa Troopa
    npcDataTable[8] = {22, getParaKoopaAnims(), 10, para_koopa_attack, para_koopa_onhit, nullptr}; // Parakoopa
    npcDataTable[9] = {99, animsSinno, 10, sinno_attack, nullptr, nullptr}; // Bald Cleft
    chunks_main();
    npcDataTable[10] = {270, getChunksAnims(), 0, chunks_attack, chunks_onhit, getChunksDeath(), chunks_on_spawn}; // O'Chunks
    //npcDataTable[11] = {440, getCherbilAnims(), cherbil_attack, nullptr, nullptr, nullptr}; // Cherbil
    npcDataTable[11] = {25, getMetAnims(), 20, met_attack, met_onhit, nullptr, met_onspawn}; // Buzzy Beetle
    met_main();
    fracktail_main();
    npcDataTable[12] = {313, getFracktailAnims(), 0, fracktail_attack, fracktail_onhit, fracktail_death, fracktail_on_spawn, fracktail_throw_script}; // Fracktail
    npcDataTable[13] = {450, animsFrackle, 50, frackle_attack, nullptr, nullptr, nullptr, frackle_throw_script}; // Frackle
    npcDataTable[14] = {89, getPukuAnims(), 25, pukupuku_attack, nullptr, nullptr, pukupuku_onspawn, nullptr}; // Cheep Cheep
    pukupuku_main();
    npcDataTable[15] = {7, animsParaKuribo, 25, para_koopa_attack, nullptr, nullptr, nullptr, nullptr}; // Paragoomba
  }

}
