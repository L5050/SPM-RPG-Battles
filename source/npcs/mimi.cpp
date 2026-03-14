#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "mimi.h"

#include <spm/rel/an.h>
#include <spm/evtmgr.h>
#include <spm/evt_ac.h>
#include <spm/evt_msg.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_fairy.h>
#include <spm/evt_img.h>
#include <spm/evt_env.h>
#include <spm/evt_eff.h>
#include <spm/evt_snd.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_npc.h>
#include <spm/evt_door.h>
#include <spm/evt_case.h>
#include <spm/evt_mobj.h>
#include <spm/evt_seq.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/mi4.h>
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

NPCTribeAnimDef animsMimi[] = {
    {0, "3D_manera_S_1"},
    {1, "3D_manera_W_2"},
    {2, "3D_manera_W_3"},
    {3, "3D_manera_T_1"},
    {4, "3D_manera_D_1"},
    {5, "3D_manera_A_1"},
    {14, "3D_manera_N_1"},
    {15, "3D_manera_K_1"},
    {16, "3D_manera_K_2"},
    {17, "3D_manera_K_3"},
    {19, "3D_manera_D_2a"},
    {20, "3D_manera_D_2b"},
    {21, "3D_manera_D_2c"},
    {22, "3D_manera_D_2d"},
    {23, "3D_manera_D_2e"},
    {24, "3D_manera_D_3"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsRubies[] = {
    {0, "manera_ruby_Z_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef * getMimiAnims()
  {
    return animsMimi;
  }

  EVT_BEGIN(mimi_onspawn)
    USER_FUNC(evt_npc_set_property, LW(15), 14, (s32)animsMimi)
    USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 281)
    ADD(LW(1), 70)
    USER_FUNC(spm::evt_mobj::evt_mobj_set_position, LW(14), LW(0), LW(1), LW(2))
    RETURN()
  EVT_END()

  EVT_BEGIN(mimi_attack)
    RUN_CHILD_EVT(mimi_ruby_attack)
    RETURN()
  EVT_END()

  EVT_BEGIN(mimi_ruby_attack)
    SET(LW(0), 0)
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
          SET(LW(14), LW(15))
          USER_FUNC(evt_npc_entry_autoname, PTR("e_3D_manera_ruby"), 0, LW(15))
          USER_FUNC(spm::an2_08::evt_rpg_enemy_death_check, UW(0), LW(4))
          IF_FLAG(LW(4), 0x2)
            USER_FUNC(evt_npc_set_animpose_disp_callback, LW(15), PTR(spm::mi4::mi4MimiHolographicEffect), 0)
          END_IF()
          USER_FUNC(evt_npc_set_property, LW(15), 14, (s32)animsRubies) 
          USER_FUNC(evt_npc_get_position, LW(14), LW(5), LW(6), LW(7))
          ADD(LW(6), 75)
          SUB(LW(7), 25)
          USER_FUNC(evt_npc_set_position, LW(15), LW(5), LW(6), LW(7))
          USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
          WAIT_MSEC(200)
          USER_FUNC(evt_npc_set_anim, LW(14), 5, 1)
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_BS_MONELA_MONEY_BORN1"))
          USER_FUNC(evt_npc_wait_anim_end, LW(14), 1)
          USER_FUNC(evt_npc_set_anim, LW(14), 0, 1)
          SUB(LW(5), 10)
          USER_FUNC(evt_npc_flag8_onoff, LW(15), 1, 201326596)
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_BS_MONELA_MONEY_FLY1"))
          BROTHER_EVT_ID(LW(10))
            SET(LW(9), FLOAT(0.0))
            LBL(59)
            ADDF(LW(9), FLOAT(5.0))
            USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, EVT_NULLPTR, LW(9))
            WAIT_FRM(1)
            GOTO(59)
          END_BROTHER()
          USER_FUNC(evt_npc_jump_to, LW(15), LW(0), LW(1), LW(7), 75, FLOAT(500.0))
          DELETE_EVT(LW(10))
          USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, EVT_NULLPTR, FLOAT(0.0))
          USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
          USER_FUNC(check_guards, 3, 11, LW(11))
          IF_EQUAL(LW(11), 2)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
            USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
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
          END_IF()
          IF_EQUAL(LW(11), 0)
            USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
            RUN_EVT(mod::marioRPGtakeDamage)
            USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          END_IF()
    INLINE_EVT()
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE() 
    USER_FUNC(ac_success_reset)
    LBL(5)
    CHK_EVT(LW(4), LW(0))
    IF_EQUAL(LW(0), 0)
      WAIT_FRM(1)
      USER_FUNC(evt_npc_delete, LW(15))
      RETURN()
    END_IF()
    WAIT_FRM(1)
    GOTO(5)
    END_IF()
  EVT_END()

  EVT_BEGIN(mimi_onhit)
    USER_FUNC(ac_success_reset)
    SWITCH(UW(0))
      CASE_EQUAL(0)
        SET(LW(14), PTR("mobj1"))
      CASE_EQUAL(1)
        SET(LW(14), PTR("mobj2"))
      CASE_EQUAL(2)
        SET(LW(14), PTR("mobj3"))
    END_SWITCH()
    USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 1, LW(14))
    INLINE_EVT()
    SET(LW(10), 0)
    LBL(1)
      IF_SMALL(LW(10), 15)
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
    ADD(LW(6), 70)
    USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 500)
    USER_FUNC(check_ac_success, LW(12))
    IF_EQUAL(LW(12), 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
      USER_FUNC(displayDamage, LW(5), LW(6), LW(7), 1)
      INLINE_EVT()
        WAIT_FRM(14)
        USER_FUNC(enable_disable_stylish, 1)
        WAIT_FRM(9)
        USER_FUNC(check_stylish, 9, LW(11))
        IF_EQUAL(LW(11), 1)
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("AC_1"), 0)
          USER_FUNC(spm::evt_sub::evt_sub_random, 50, LW(11))
          ADD(LW(11), 50)
          USER_FUNC(spm::an2_08::evt_rpg_add_xp, LW(11))
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU1"))
          USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), 150, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0) 
          RUN_EVT(increase_stylish)
        END_IF()
      END_INLINE()
      USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 700)
      USER_FUNC(displayDamage, LW(5), LW(6), LW(7), 1)
      USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, UW(0), 1, 0, LW(0))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
      RUN_EVT(damageAnims)
    ELSE()
      USER_FUNC(displayDamage, LW(5), LW(6), LW(7), 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
      RUN_EVT(damageAnims)
    END_IF()
    USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, UW(0), 0, 0, LW(0))
    USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 0, LW(14))
    //USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(15), 19, 1)
    //USER_FUNC(evt_npc_wait_anim_end, LW(15), 1)
    //WAIT_FRM(1)
    //USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    RETURN()
  EVT_END()

  EVT_BEGIN(mimi_throw_script)
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
    ADD(LW(6), 20)
    USER_FUNC(spm::evt_fairy::evt_fairy_all_enter_run_mode1)
    USER_FUNC(spm::evt_fairy::evt_fairy_fly_to, 0, LW(5), LW(6), LW(7), 500)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_1"))
    USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_NAGE_FIRLING1"))
    USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_2B"))
    INLINE_EVT()
      ADD(LW(6), 30)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(15), 4, 1)
      USER_FUNC(spm::evt_npc::evt_npc_glide_to, LW(15), LW(5), LW(6), LW(7), 0, FLOAT(380.0), 0, 0, 0, 0)
    END_INLINE()
    SET(LW(9), FLOAT(0.0))
    DO(0)
      ADDF(LW(9), FLOAT(30.0))
      IF_LARGE(LW(9), 180)
        SET(LW(9), 180)
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, EVT_NULLPTR, LW(9))
      WAIT_FRM(1)
      IF_EQUAL(LW(9), 180)
        DO_BREAK()
      END_IF()
    WHILE()
    ADD(LW(4), 1)
    WAIT_FRM(10)
    LBL(33)
    USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_3C"))
    INLINE_EVT()
      ADD(LW(6), 30)
      USER_FUNC(spm::evt_fairy::evt_fairy_fly_to, 0, LW(5), LW(6), LW(7), 100)
    END_INLINE()
    BROTHER_EVT_ID(LW(12))
      USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
      USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_4A"))
      USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
      USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("S_1"))
      USER_FUNC(ac_success_reset)
      USER_FUNC(enable_disable_stylish, 1)
      WAIT_FRM(10)
      USER_FUNC(check_stylish, 10, LW(11))
      IF_EQUAL(LW(11), 1)
        USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(5))
        ADD(LW(5), 50)
        USER_FUNC(spm::an2_08::evt_rpg_add_xp, LW(5))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU1"))
        USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), 150, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
        RUN_CHILD_EVT(increase_stylish)
        SET(LW(5), UW(4))
        USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_4A"))
        USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
        USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("S_1"))
      END_IF()
    END_BROTHER()
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_THROW1"))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
    ADD(LW(1), 20)
    USER_FUNC(spm::evt_npc::evt_npc_jump_to, LW(15), LW(0), LW(1), LW(2), 75, FLOAT(500.0))
    SET(LW(4), UW(0))
    SET(LW(2), LW(4))
    WAIT_FRM(15)
    INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
    SUB(LW(1), 20)
    USER_FUNC(spm::evt_npc::evt_npc_jump_to, LW(15), LW(0), LW(1), LW(2), 25, FLOAT(500.0))
    USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(15), LW(0), LW(1), LW(2))
    END_INLINE()
    WAIT_FRM(4)
    DO(0)
      SUBF(LW(9), FLOAT(30.0))
      IF_SMALL(LW(9), 0)
        SET(LW(9), 0)
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, EVT_NULLPTR, LW(9))
      WAIT_FRM(1)
      IF_EQUAL(LW(9), 0)
        DO_BREAK()
      END_IF()
    WHILE()
    USER_FUNC(spm::evt_fairy::evt_fairy_all_enter_run_mode2)
    WAIT_FRM(7)
    RETURN()
  EVT_END()

  void mimi_main()
  {
    npcTribes[280].killXp *= 2;
    npcTribes[280].attackStrength = 3;
  }

}
