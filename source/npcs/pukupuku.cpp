#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "pukupuku.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"

#include <spm/rel/an.h>
#include <spm/evtmgr.h>
#include <spm/evt_ac.h>
#include <spm/evt_msg.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_paper.h>
#include <spm/evt_mobj.h>
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
#include <spm/wpadmgr.h>
#include <spm/map_data.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>
#include <patch.h>
#include <string>

using namespace spm::npcdrv;
using namespace spm::evt_npc;

namespace mod {

NPCTribeAnimDef animsPuku[] = {
    {0, "pukupuku_S_1"},
    {1, "pukupuku_W_1"},
    {2, "pukupuku_W_2"},
    {3, "pukupuku_T_1"},
    {4, "pukupuku_D_1"},
    {6, "pukupuku_D_1"},
    {7, "pukupuku_D_1"},
    {10, "pukupuku_A_1a"},
    {11, "pukupuku_A_1b"},
    {12, "pukupuku_A_1c"},
    {14, "pukupuku_N_1"},
    {-1, nullptr}
  };

  EVT_BEGIN(pukupuku_onspawn)
    SUB(LW(2), 25)
    SUB(LW(1), 50)
    ADD(LW(0), 125)
    USER_FUNC(evt_npc_set_position, LW(15), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 1, LW(1))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 2, LW(2))
    USER_FUNC(spm::evt_mobj::evt_mobj_set_position, LW(14), LW(0), LW(1), LW(2))
  RETURN()
  EVT_END()

  EVT_BEGIN(pukupuku_attack)
    SET(LW(0), 0)
    SET(LW(4), 0)
    SWITCH(LW(0))
      CASE_EQUAL(0)
        USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(8))
        ADDF(LW(1), LW(8))
        SUBF(LW(1), FLOAT(15.0))
        ADDF(LW(6), FLOAT(175.0))
        USER_FUNC(evt_npc_set_anim, LW(15), 2, 1)
        USER_FUNC(evt_npc_jump_to, LW(15), LW(5), LW(6), LW(7), 25, FLOAT(1000.0))
        USER_FUNC(evt_npc_set_anim, LW(15), 10, 1)
        BROTHER_EVT()
          SET(LW(8), 0)
          SET(LW(9), FLOAT(0.0))
          LBL(59)
          ADD(LW(8), 1)
          ADDF(LW(9), FLOAT(5.0))
          USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, EVT_NULLPTR, LW(9))
          WAIT_FRM(1)
          IF_EQUAL(LW(8), 5)
            RETURN()
          END_IF()
          GOTO(59)
        END_BROTHER()
        USER_FUNC(evt_npc_set_anim, LW(15), 11, 1)
        USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
        USER_FUNC(evt_npc_glide_to, LW(15), LW(0), LW(1), LW(7), 0, FLOAT(680.0), 0, 0, 0, 0)
        USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, EVT_NULLPTR, FLOAT(0.0))
        USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
        USER_FUNC(check_ac_success, LW(11))
        USER_FUNC(check_guards, 3, 11, LW(11))
        IF_EQUAL(LW(11), 2)
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
          USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
          USER_FUNC(spm::evt_mario::evt_mario_get_pos, EVT_NULLPTR, LW(1), LW(2))
          USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(1), LW(2), 20, 300)
          BROTHER_EVT_ID(LW(4))
            RUN_CHILD_EVT(superguard_stylish)
          END_BROTHER()
        END_IF()
        IF_EQUAL(LW(11), 1)
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
            USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
            SUB(LW(10), 2)
            IF_SMALL(LW(10), 0)
              SET(LW(10), 0)
            END_IF()
            RUN_EVT(mod::marioRPGtakeDamage)
            USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
            USER_FUNC(evt_npc_set_anim, LW(15), 12, 1)
        END_IF()
        IF_EQUAL(LW(11), 0)
          USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
          RUN_EVT(mod::marioRPGtakeDamage)
          USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          USER_FUNC(evt_npc_set_anim, LW(15), 12, 1)
        END_IF()
    END_SWITCH()
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(0), 50)
  USER_FUNC(evt_npc_jump_to, LW(15), LW(0), LW(6), LW(7), 75, FLOAT(500.0))
  USER_FUNC(evt_npc_set_anim, LW(15), 2, 1)
  ADD(LW(5), 150)
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
  USER_FUNC(evt_npc_jump_to, LW(15), LW(0), LW(1), LW(2), 75, FLOAT(500.0))
  //USER_FUNC(evt_npc_glide_to, LW(15), LW(0), LW(1), LW(2), 0, FLOAT(240.0), 0, 0, 0, 0)
  USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
  USER_FUNC(ac_success_reset)
  LBL(5)
  CHK_EVT(LW(4), LW(0))
  IF_EQUAL(LW(0), 0)
    RETURN()
  END_IF()
  WAIT_FRM(1)
  GOTO(5)
  EVT_END()

  NPCTribeAnimDef * getPukuAnims()
  {
    return animsPuku;
  }

void pukupuku_main()
{
  npcTribes[88].maxHp = 20;
  npcTribes[88].attackStrength = 6;
  npcTribes[89].maxHp = 4;
  npcTribes[89].attackStrength = 4;
}

}
