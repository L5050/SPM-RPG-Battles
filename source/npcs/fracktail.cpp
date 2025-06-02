#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "oChunks.h"
#include "sndpatch.h"
#include "fracktail.h"

#include <spm/evtmgr.h>
#include <spm/evt_ac.h>
#include <spm/evt_msg.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_mobj.h>
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
#include <spm/wpadmgr.h>
#include <spm/map_data.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/an.h>
#include <spm/rel/mi4.h>
#include <wii/os/OSError.h>
#include <wii/os/OSError.h>
#include <nw4r/snd.hpp>
#include <patch.h>

using namespace spm::npcdrv;
using namespace spm::evt_npc;

namespace mod {
spm::evtmgr::EvtScriptCode* fracktail_evt = nullptr;
NPCTribeAnimDef animsFracktail[] = {
    {0, "zun_all_S_1"},
    {1, "zun_all_S_1B"},
    {2, "zun_all_S_2"},
    {3, "zun_all_T_1"},
    {4, "zun_all_D_1"},
    {8, "zun_all_I_1"},
    {9, "zun_all_M_1"},
    {10, "zun_all_S_3"},
    {11, "zun_all_S_4A"},
    {12, "zun_all_S_4B"},
    {13, "zun_all_S_4C"},
    {14, "zun_all_S_4D"},
    {24, "zun_all_T_3"},
    {-1, nullptr}
  };

NPCTribeAnimDef animsFracktailHead[] = {
    {0, "zun_hed_S_1A"},
    {1, "zun_hed_S_1B"},
    {2, "zun_hed_S_2A"},
    {3, "zun_hed_T_1"},
    {4, "zun_hed_D_1"},
    {5, "zun_hed_S_2B"},
    {10, "zun_hed_A_1"},
    {11, "zun_hed_H_1"},
    {-1, nullptr}
  };

  const char * fracktail_onhit_msg = "<dq><p>\n"
  "Fracktail is too high\n"
  "up to jump on!\n"
  "<k>\n";

  const char * fracktail_onbirth_msg = "<dq><p>\n"
  "Fracktail gave birth!\n"
  "<k>\n";

  NPCTribeAnimDef * getFracktailAnims()
  {
    return animsFracktail;
  }

  EVT_BEGIN(good_camera)
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    SET(LW(3), UW(2))
    SET(LW(4), UW(3))
    SUB(LW(3), 100)
    SUB(LW(4), 100)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(0), EVT_NULLPTR, UW(3), LW(0), EVT_NULLPTR, -500, 1000, 11)
  RETURN()
  EVT_END()

  EVT_BEGIN(fracktail_on_spawn)
    USER_FUNC(evt_npc_set_position, PTR("zun"), 0, -500, 0)
    USER_FUNC(evt_npc_animflag_onoff, PTR("zun"), 1, 128)
    //USER_FUNC(evt_npc_set_animpose_disp_callback, LW(15), PTR(spm::mi4::mi4MimiHolographicEffect), 0)
    USER_FUNC(evt_npc_set_scale, LW(15), FLOAT(4.0), FLOAT(4.0), FLOAT(4.0))
    ADD(LW(0), 500)
    USER_FUNC(evt_npc_set_position, LW(15), LW(0), -1050, LW(2))
    //USER_FUNC(evt_npc_flag8_onoff, LW(15), 1, 1073741824)
    USER_FUNC(evt_npc_flag8_onoff, LW(15), 1, 65536)
    USER_FUNC(evt_npc_flag8_onoff, LW(15), 1, 205520900)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 1, -1050)
    SUB(UW(3), 300)
    USER_FUNC(spm::evt_mobj::evt_mobj_set_scale, LW(14), FLOAT(5.0), FLOAT(3.0), FLOAT(5.0))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 300, 0, 11)
    ADD(LW(1), 275)
    SUB(LW(0), 375)
    ADD(LW(2), 50)
    USER_FUNC(spm::evt_mobj::evt_mobj_set_position, LW(14), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_mobj::evt_mobj_set_rotation, LW(14), FLOAT(0.0), FLOAT(0.0), FLOAT(15.0))
    USER_FUNC(spm::evt_mobj::evt_mobj_blk, 4, PTR("mobj4"), 0, 0, 0, 0, 0, 0)
    ADD(LW(1), 260)
    ADD(LW(0), 40)
    USER_FUNC(spm::evt_mobj::evt_mobj_set_position, PTR("mobj4"), LW(0), LW(1), LW(2))
    USER_FUNC(mobjChangeAnimPoseName, PTR("mobj4"), PTR("MOBJ_su_blank"))
  RETURN()
  EVT_END()

  EVT_BEGIN(fracktail_start_fight)
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_STG1"))
    WAIT_FRM(15)
    USER_FUNC(start_boss_fight, 313)
  RETURN()
  EVT_END()

  EVT_BEGIN(fracktail_fix)
    DELETE_EVT(LW(10))
  RETURN_FROM_CALL()

  EVT_BEGIN(fracktail_fix2)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 750, 275, 2100, 750, 275, 250, 5000, 11)
    INLINE_EVT()
        USER_FUNC(spm::evt_cam::evt_cam_shake, 5, FLOAT(1.19921875), FLOAT(1.19921875), FLOAT(0.0), 5500, 0)
    END_INLINE()
  RETURN_FROM_CALL()

  EVT_BEGIN(fracktail_fix3)
    INLINE_EVT()
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        ADD(LW(0), -125)
        DO(20)
            USER_FUNC(spm::evt_sub::evt_sub_random, 200, LW(1))
            ADD(LW(1), LW(0))
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, 200, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 205, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
        WHILE()
        WAIT_MSEC(100)
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 24, LW(0), 0, 200, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
    END_INLINE()
    INLINE_EVT()
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        ADD(LW(0), -325)
        DO(20)
            USER_FUNC(spm::evt_sub::evt_sub_random, 200, LW(1))
            ADD(LW(1), LW(0))
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, 200, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 205, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
        WHILE()
    END_INLINE()
  RETURN_FROM_CALL()
  
EVT_BEGIN(frackle_attack)
    SET(LW(0), 0)
    SET(LW(4), 0)
        RUN_CHILD_EVT(good_camera)
        USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        SET(LW(8), LW(5))
        SET(LW(5), LW(0))
        ADD(LW(5), 75)
        USER_FUNC(evt_npc_jump_to, LW(15), LW(8), LW(6), LW(7), 100, FLOAT(200.0))
        USER_FUNC(evt_npc_jump_to, LW(15), LW(8), LW(6), LW(7), 100, FLOAT(200.0))
        WAIT_FRM(30)
        USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
        INLINE_EVT()
          SET(LW(10), 0)
          LBL(1)
            IF_SMALL(LW(10), 5)
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
                IF_LARGE_EQUAL(LW(10), 15)
                  USER_FUNC(superguard_toggle)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
                END_IF()
                GOTO(2)
              END_IF()
            END_IF()
            WAIT_FRM(1)
            ADD(LW(10), 1)
            IF_EQUAL(LW(10), 19)
              GOTO(2)
            END_IF()
          GOTO(1)
          LBL(2)
        END_INLINE()
        USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(6), LW(7), FLOAT(800.0), 0, 0, 0)
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
RETURN()
EVT_END()

  EVT_BEGIN(fracktail_onhit)
    USER_FUNC(evt_npc_get_unitwork, LW(15), 3, LW(0))
    IF_EQUAL(LW(0), 0)
      USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(fracktail_onhit_msg), 0, 0)
      SET(LF(1), 1)
    ELSE()
      RUN_CHILD_EVT(rpg_jump)
    END_IF()
    RETURN()
  EVT_END()

  EVT_BEGIN(superguard_stylish_fracktail)
  USER_FUNC(ac_success_reset)
  INLINE_EVT()
    SET(LW(10), 0)
    USER_FUNC(enable_disable_stylish, 1)
    LBL(1)
        USER_FUNC(check_pressed_a_ac, LW(11))
        IF_EQUAL(LW(11), 1)
          USER_FUNC(ac_success_toggle)
          GOTO(2)
        END_IF()
      WAIT_FRM(1)
      ADD(LW(10), 1)
      IF_EQUAL(LW(10), 10)
        GOTO(2)
      END_IF()
    GOTO(1)
    LBL(2)
  USER_FUNC(enable_disable_stylish, 0)
  END_INLINE()
  WAIT_FRM(10)
  USER_FUNC(check_ac_success, LW(11))
  IF_EQUAL(LW(11), 1)
      USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(5))
      ADD(LW(5), 50)
      USER_FUNC(spm::an2_08::evt_rpg_add_xp, LW(5))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU1"))
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), -250, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)  //@TODO Figure out good hardcoded 2D offsets
      RUN_CHILD_EVT(increase_stylish)
      INLINE_EVT()
          SET(LW(11), 0)
          SETF(LW(10), FLOAT(0.0))
          DO(0)
            USER_FUNC(mario_rotate, LW(10))
            ADD(LW(11), 1)
            ADDF(LW(10), FLOAT(50.0))
            WAIT_FRM(1)
            IF_EQUAL(LW(11), 10)
              DO_BREAK()
            END_IF()
          WHILE()
          SETF(LW(10), FLOAT(0.0))
          USER_FUNC(mario_rotate, LW(10))
      END_INLINE()
      INLINE_EVT()
        USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_3"), 0)
      END_INLINE()
  END_IF()
  WAIT_FRM(3)
  USER_FUNC(ac_success_reset)
  RETURN()
EVT_END()

EVT_BEGIN(marioRPGtakeDamage_fracktail)
    ADDF(LW(1), FLOAT(25.0))
    USER_FUNC(displayDamage, LW(0), LW(1), LW(2), LW(10))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    INLINE_EVT()
      WAIT_FRM(5)
      USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("D_3"), 0)
    END_INLINE()
    USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(1), LW(2), 20, 300)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_DAMAGE1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
    RETURN()
EVT_END()

EVT_BEGIN(fracktail_damage_evt)
        USER_FUNC(check_ac_success, LW(11))
        IF_EQUAL(LW(11), 1)
          USER_FUNC(check_superguard_success, LW(11))
          IF_EQUAL(LW(11), 1)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
            USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
            USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(1), LW(2), 20, 300)
            RUN_EVT(superguard_stylish_fracktail)
          ELSE()
            USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, UW(0), LW(10))
            RUN_EVT(mod::marioRPGtakeDamage_fracktail)
            USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_BS_ZNB_BITE1"))
          END_IF()
        ELSE()
          USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, UW(0), LW(10))
          RUN_EVT(mod::marioRPGtakeDamage_fracktail)
          USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_BS_ZNB_BITE1"))
        END_IF()
RETURN()
EVT_END()

EVT_BEGIN(fracktail_head_attack)
  RUN_CHILD_EVT(good_camera)
  USER_FUNC(evt_npc_entry_autoname, PTR("e_zun_hed"), 0, LW(14))
  USER_FUNC(evt_npc_set_property, LW(14), 14, (s32)animsFracktailHead)
  USER_FUNC(evt_npc_set_anim, LW(14), 0, 1)
  USER_FUNC(evt_npc_flag8_onoff, LW(14), 1, 65536)
  USER_FUNC(evt_npc_flag8_onoff, LW(14), 1, 205520900)
  USER_FUNC(evt_npc_set_scale, LW(14), FLOAT(3.0), FLOAT(3.0), FLOAT(3.0))
  SET(LW(6), -500)
  ADD(LW(0), 300)
  USER_FUNC(evt_npc_set_position, LW(14), LW(0), LW(6), LW(2))
  INLINE_EVT()
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    ADD(LW(0), 200)
    SET(LW(4), 0)
    DO(10)
      SET(LW(4), 0)
      USER_FUNC(spm::evt_sub::evt_sub_random, 200, LW(1))
      ADD(LW(1), LW(0))
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, LW(4), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, LW(4), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      WAIT_MSEC(50)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, LW(4), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, LW(4), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      WAIT_MSEC(50)
    WHILE()
  END_INLINE()
  INLINE_EVT()
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    ADD(LW(0), 200)
    DO(10)
      SET(LW(4), 0)
      USER_FUNC(spm::evt_sub::evt_sub_random, 200, LW(1))
      ADD(LW(1), LW(0))
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, LW(4), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, LW(4), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      WAIT_MSEC(50)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, LW(4), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, LW(4), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
      ADD(LW(4), 5)
      WAIT_MSEC(50)
    WHILE()
  END_INLINE()
  USER_FUNC(evt_npc_get_position, LW(14), LW(5), LW(6), LW(7))
  USER_FUNC(evt_npc_glide_to, LW(14), LW(5), 150, LW(7), 0, FLOAT(250.0), 0, 0, 0, 0)
  USER_FUNC(spm::evt_sub::evt_sub_random, 0, LW(0))
  IF_EQUAL(LW(0), 0)
    SUB(LW(5), 50)
    USER_FUNC(evt_npc_glide_to, LW(14), LW(5), 150, LW(7), 0, FLOAT(250.0), 0, 0, 0, 0)
    USER_FUNC(evt_npc_set_anim, LW(14), 10, 1)
    DO(3)
      INLINE_EVT()
        SET(LW(10), 0)
        LBL(1)
        IF_SMALL(LW(10), 5)
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
            IF_LARGE_EQUAL(LW(10), 16)
              USER_FUNC(ac_success_toggle)
              USER_FUNC(superguard_toggle)
              USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
            END_IF()
          GOTO(2)
          END_IF()
        END_IF()
        WAIT_FRM(1)
        ADD(LW(10), 1)
        IF_EQUAL(LW(10), 20)
        GOTO(2)
        END_IF()
        GOTO(1)
        LBL(2)
      END_INLINE()
      WAIT_FRM(20)
      RUN_EVT(fracktail_damage_evt)
    WHILE()
    USER_FUNC(evt_npc_set_anim, LW(14), 0, 1)
    INLINE_EVT()
      USER_FUNC(evt_npc_glide_to, LW(14), LW(5), -500, LW(7), 0, FLOAT(250.0), 0, 0, 0, 0)
      WAIT_FRM(1)
      USER_FUNC(evt_npc_delete, LW(14))
    END_INLINE()
  ELSE()

  END_IF()
  RETURN()
EVT_END()

EVT_BEGIN(fracktail_attack)
  USER_FUNC(evt_npc_get_unitwork, LW(15), 4, LW(0))
  IF_EQUAL(LW(0), 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_BS_ZNB_DAMAGE1"))
    WAIT_MSEC(2000) 
    USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(fracktail_onbirth_msg), 0, 0)
    USER_FUNC(evt_npc_set_unitwork, LW(15), 4, 1)
    RUN_CHILD_EVT(fracktail_head_attack)
  ELSE()
    RUN_CHILD_EVT(fracktail_head_attack)
  END_IF()
  RETURN()
EVT_END()

  void fracktail_main()
  {
    npcTribes[313].attackStrength = 1;
    npcTribes[313].maxHp = 30;
    spm::map_data::MapData * he4_10_md = spm::map_data::mapDataPtr("he4_10");
    spm::evtmgr::EvtScriptCode* fracktail_fight_setup_evt = getInstructionEvtArg(he4_10_md->initScript, 24, 0);
    fracktail_evt = getInstructionEvtArg(fracktail_fight_setup_evt, 34, 0);
    evtpatch::hookEvtReplace(fracktail_evt, 366, (spm::evtmgr::EvtScriptCode*)fracktail_start_fight);
    evtpatch::hookEvtReplaceBlock(fracktail_evt, 340, (spm::evtmgr::EvtScriptCode*)insertNop, 355);
    //evtpatch::hookEvt(fracktail_evt, 335, (spm::evtmgr::EvtScriptCode*)fracktail_fix);
    evtpatch::hookEvtReplace(fracktail_evt, 330, (spm::evtmgr::EvtScriptCode*)insertNop);
    evtpatch::hookEvtReplaceBlock(fracktail_evt, 291, (spm::evtmgr::EvtScriptCode*)fracktail_fix3, 335);
    
  }

}
