#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "koopa.h"

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


NPCTribeAnimDef animsGreenKoopaTroopa1[] = {
  {0, "NKT_S_1"},
  {1, "NKT_W_1"},
  {2, "NKT_R_1"},
  {3, "NKT_T_1"},
  {4, "NKT_D_1"},
  {5, "NKT_S_3"},
  {6, "NKT_R_2"},
  {7, "NKT_S_2"},
  {10, "NKT_R_1"},
  {11, "NKT_Z_1"},
  {14, "NKT_N_1"},
  {-1, nullptr}
};

NPCTribeAnimDef animsGreenParaTroopa1[] = {
  {0, "PTP_S_1"},
  {1, "PTP_W_1"},
  {2, "PTP_R_1"},
  {3, "PTP_T_1"},
  {4, "PTP_D_1"},
  {5, "PTP_S_3"},
  {6, "PTP_R_1"},
  {7, "PTP_S_2"},
  {10, "PTP_R_1"},
  {11, "PTP_Z_1"},
  {14, "PTP_N_1"},
  {-1, nullptr}
};

NPCTribeAnimDef animsGreenKoopaTroopa2[] = {
  {0, "NKT_S_2"},
  {1, "NKT_W_1"},
  {2, "NKT_R_1"},
  {3, "NKT_T_1"},
  {4, "NKT_S_3"},
  {5, "NKT_D_1"},
  {6, "NKT_R_2"},
  {7, "NKT_S_1"},
  {10, "NKT_R_1"},
  {11, "NKT_Z_1"},
  {14, "NKT_N_1"},
  {-1, nullptr}
};

namespace mod {

  EVT_BEGIN(koopa_onhit)
    USER_FUNC(evt_npc_get_unitwork, LW(15), 3, LW(0))
    IF_EQUAL(LW(0), 0)
      INLINE_EVT()
      SET(LW(10), 0)
      LBL(1)
        IF_SMALL(LW(10), 20)
          USER_FUNC(check_pressed_2_ac, LW(11))
          IF_EQUAL(LW(11), 1)
            GOTO(2)
          END_IF()
        ELSE()
          USER_FUNC(check_pressed_2_ac, LW(11))
          IF_EQUAL(LW(11), 1)
            USER_FUNC(ac_success_toggle)
            GOTO(2)
          END_IF()
        END_IF()
        WAIT_FRM(1)
        ADD(LW(10), 1)
        IF_EQUAL(LW(10), 30)
          GOTO(2)
        END_IF()
      GOTO(1)
      LBL(2)
      END_INLINE()
      USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 500)
      USER_FUNC(check_ac_success, LW(11))
      USER_FUNC(spm::an2_08::evt_rpg_calc_damage_to_enemy, LW(2), 0, LW(10))
      IF_EQUAL(LW(11), 1)
        USER_FUNC(evt_npc_set_unitwork, LW(15), 3, 1)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_BUNSIN_ATTACK1"))
        USER_FUNC(evt_npc_set_property, LW(15), 14, (s32)animsGreenKoopaTroopa2)
        USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
        USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 80, 700)
        USER_FUNC(displayDamage, LW(5), LW(6), LW(7), LW(10))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
        USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, LW(2), LW(10), 0, LW(0))
        RUN_EVT(damageAnims)
      ELSE()
        USER_FUNC(evt_npc_set_unitwork, LW(15), 3, 1)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_BUNSIN_ATTACK1"))
        USER_FUNC(evt_npc_set_property, LW(15), 14, (s32)animsGreenKoopaTroopa2)
        USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
        RUN_EVT(damageAnims)
      END_IF()
      RETURN()
    ELSE()
      RUN_CHILD_EVT(rpg_jump)
    END_IF()
    RETURN()
  EVT_END()

  EVT_BEGIN(para_koopa_onhit)
    USER_FUNC(evt_npc_get_unitwork, LW(15), 3, LW(0))
    IF_EQUAL(LW(0), 0)
      INLINE_EVT()
      SET(LW(10), 0)
      LBL(1)
        IF_SMALL(LW(10), 20)
          USER_FUNC(check_pressed_2_ac, LW(11))
          IF_EQUAL(LW(11), 1)
            GOTO(2)
          END_IF()
        ELSE()
          USER_FUNC(check_pressed_2_ac, LW(11))
          IF_EQUAL(LW(11), 1)
            USER_FUNC(ac_success_toggle)
            GOTO(2)
          END_IF()
        END_IF()
        WAIT_FRM(1)
        ADD(LW(10), 1)
        IF_EQUAL(LW(10), 30)
          GOTO(2)
        END_IF()
      GOTO(1)
      LBL(2)
      END_INLINE()
      USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 500)
      USER_FUNC(spm::an2_08::evt_rpg_calc_damage_to_enemy, LW(2), 0, LW(10))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_BUNSIN_ATTACK1"))
      USER_FUNC(evt_npc_set_property, LW(15), 14, (s32)animsGreenKoopaTroopa1)
      USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
      RUN_EVT(damageAnims)
      USER_FUNC(setRpgTribeID, UW(0), 11)
      RETURN()
    ELSE()
      RUN_CHILD_EVT(rpg_jump)
    END_IF()
    RETURN()
  EVT_END()

  EVT_BEGIN(koopa_attack)
    USER_FUNC(evt_npc_get_unitwork, LW(15), 3, LW(0))
    IF_EQUAL(LW(0), 0)
      SET(LW(0), 0)
      SET(LW(4), 0)
          USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
          USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
          SET(LW(5), LW(0))
          ADD(LW(5), 75)
          INLINE_EVT()
            SET(LW(13), UW(3))
            SUB(LW(13), 25)
            USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
          END_INLINE()
          USER_FUNC(evt_npc_set_anim, LW(15), 6, 1)
          USER_FUNC(evt_npc_walk_to, LW(15), LW(5), 0, LW(7), FLOAT(250.0), 0, 0, 0)
          INLINE_EVT()
            SET(LW(10), 0)
            LBL(1)
              IF_SMALL(LW(10), 2)
                USER_FUNC(check_pressed_b_ac, LW(11))
                IF_EQUAL(LW(11), 1)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
                  WAIT_FRM(3)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
                  GOTO(2)
                END_IF()
              ELSE()
                USER_FUNC(check_pressed_b_ac, LW(11))
                IF_EQUAL(LW(11), 1)
                  USER_FUNC(ac_success_toggle)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
                  IF_LARGE_EQUAL(LW(10), 7)
                    USER_FUNC(superguard_toggle)
                    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
                  END_IF()
                  GOTO(2)
                END_IF()
              END_IF()
              WAIT_FRM(1)
              ADD(LW(10), 1)
              IF_EQUAL(LW(10), 13)
                GOTO(2)
              END_IF()
            GOTO(1)
            LBL(2)
          END_INLINE()
          USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(6), LW(7), FLOAT(250.0), 0, 0, 0)
          USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
          USER_FUNC(check_ac_success, LW(11))
          IF_EQUAL(LW(11), 1)
            USER_FUNC(check_superguard_success, LW(11))
            IF_EQUAL(LW(11), 1)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
            USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
            USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(6), LW(7), 20, 300)
            BROTHER_EVT_ID(LW(4))
              RUN_CHILD_EVT(superguard_stylish)
            END_BROTHER()
            WAIT_MSEC(500)
            ELSE()
              SET(LW(11), 1)
              USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
              SUB(LW(10), 2)
              IF_SMALL(LW(10), 0)
                SET(LW(10), 0)
              END_IF()
              RUN_CHILD_EVT(mod::marioRPGtakeDamage)
              USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
            END_IF()
          ELSE()
            USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
            RUN_CHILD_EVT(mod::marioRPGtakeDamage)
            USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          END_IF()
    USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
    ADD(LW(5), 150)
    INLINE_EVT()
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
    USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(1), LW(2), FLOAT(240.0), 0, 0, 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    USER_FUNC(ac_success_reset)
    LBL(5)
    CHK_EVT(LW(4), LW(0))
    IF_EQUAL(LW(0), 0)
      RETURN()
    END_IF()
    WAIT_FRM(1)
    GOTO(5)
    ELSE()
      USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
      WAIT_FRM(20)
      USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
      RETURN()
    END_IF()
  EVT_END()

  EVT_BEGIN(para_koopa_attack)
    USER_FUNC(evt_npc_get_unitwork, LW(15), 3, LW(0))
    IF_EQUAL(LW(0), 0)
      SET(LW(0), 0)
      SET(LW(4), 0)
          USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
          USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
          SET(LW(5), LW(0))
          ADD(LW(5), 100)
          INLINE_EVT()
            SET(LW(13), UW(3))
            SUB(LW(13), 25)
            USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
          END_INLINE()
          ADD(LW(6), 75)
          USER_FUNC(evt_npc_glide_to, LW(15), LW(5), LW(6), LW(7), 0, FLOAT(180.0), 0, 0, 0, 0)
          SUB(LW(6), 75)
          USER_FUNC(evt_npc_set_anim, LW(15), 6, 1)
          INLINE_EVT()
            SET(LW(10), 0)
            LBL(1)
              IF_SMALL(LW(10), 7)
                USER_FUNC(check_pressed_b_ac, LW(11))
                IF_EQUAL(LW(11), 1)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
                  WAIT_FRM(3)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
                  GOTO(2)
                END_IF()
              ELSE()
                USER_FUNC(check_pressed_b_ac, LW(11))
                IF_EQUAL(LW(11), 1)
                  USER_FUNC(ac_success_toggle)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
                  IF_LARGE_EQUAL(LW(10), 17)
                    USER_FUNC(superguard_toggle)
                    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
                  END_IF()
                  GOTO(2)
                END_IF()
              END_IF()
              WAIT_FRM(1)
              ADD(LW(10), 1)
              IF_EQUAL(LW(10), 21)
                GOTO(2)
              END_IF()
            GOTO(1)
            LBL(2)
          END_INLINE()
          USER_FUNC(evt_npc_glide_to, LW(15), LW(0), LW(6), LW(7), 0, FLOAT(380.0), 0, 0, 0, 0)
          USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
          USER_FUNC(check_ac_success, LW(11))
          IF_EQUAL(LW(11), 1)
            USER_FUNC(check_superguard_success, LW(11))
            IF_EQUAL(LW(11), 1)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
            USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
            USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(6), LW(7), 20, 300)
            BROTHER_EVT_ID(LW(4))
              RUN_CHILD_EVT(superguard_stylish)
            END_BROTHER()
            WAIT_MSEC(500)
            ELSE()
              SET(LW(11), 1)
              USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
              SUB(LW(10), 2)
              IF_SMALL(LW(10), 0)
                SET(LW(10), 0)
              END_IF()
              RUN_CHILD_EVT(mod::marioRPGtakeDamage)
              USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
            END_IF()
          ELSE()
            USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
            RUN_CHILD_EVT(mod::marioRPGtakeDamage)
            USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          END_IF()
    USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
    INLINE_EVT()
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
    //USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(1), LW(2), FLOAT(240.0), 0, 0, 0)
    USER_FUNC(evt_npc_glide_to, LW(15), LW(0), LW(1), LW(2), 0, FLOAT(500.0), 0, 0, 0, 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    USER_FUNC(ac_success_reset)
    LBL(5)
    CHK_EVT(LW(4), LW(0))
    IF_EQUAL(LW(0), 0)
      RETURN()
    END_IF()
    WAIT_FRM(1)
    GOTO(5)
    ELSE()
      USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
      WAIT_FRM(20)
      USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
      RETURN()
    END_IF()
  EVT_END()

  NPCTribeAnimDef * getKoopaAnims()
  {
    return animsGreenKoopaTroopa1;
  }

  NPCTribeAnimDef * getParaKoopaAnims()
  {
    return animsGreenParaTroopa1;
  }

  void koopa_main()
  {
    npcTribes[11].maxHp = 3;
    npcTribes[14].attackStrength = 3;
  }

}
