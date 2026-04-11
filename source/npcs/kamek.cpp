#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "kamek.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "ip.h"
#include "ip_badges.h"
#include "tplpatch.h"
#include "ip_badgepouch.h"

#include <spm/rel/an.h>
#include <spm/evtmgr.h>
#include <spm/animdrv.h>
#include <spm/evt_ac.h>
#include <spm/evt_msg.h>
#include <spm/evt_mario.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_paper.h>
#include <spm/evt_img.h>
#include <spm/evt_env.h>
#include <spm/evt_eff.h>
#include <spm/evt_snd.h>
#include <spm/rel/mi4.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_npc.h>
#include <spm/evt_door.h>
#include <spm/evt_case.h>
#include <spm/evt_pouch.h>
#include <spm/evt_mobj.h>
#include <spm/evt_seq.h>
#include <spm/rel/an2_08.h>
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
using namespace spm;

namespace mod {

  const char * btl_kamek_dialogue_1 = "<dq>\n"
  "<p>\n"
  "*Kamek overheals the\n"
  "bald clefts!\n"
  "<o>\n";

  const char * btl_kamek_dialogue_2 = "<dq>\n"
  "<p>\n"
  "*Kamek ran out\n"
  "of bald clefts.\n"
  "<o>\n";

  const char * lucky_start_text = "<system><p>\n"
  "You got Lucky Start!\n"
  "<k>\n"
  "<p>\n"
  "Lucky Start gives you a\n"
  "chance of fully restoring HP and\n"
  "FP when entering combat.\n"
  "<k>\n"
  "<p>\n"
  "It costs 4 BP to wear.\n"
  "<k>\n";

  EVT_BEGIN(getLuckyStart)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(spm::evt_mobj::evt_mobj_delete, PTR("luckyStart"))
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(lucky_start_text), 0, 0)
    USER_FUNC(ip::evt_pouch_add_badge, 3)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
  EVT_END()

  EVT_BEGIN(addLuckyStart)
    USER_FUNC(ip::evt_pouch_check_for_badge, 3, LW(15))
    IF_EQUAL(LW(15), 0)
      USER_FUNC(evt_mobj_sui_new, 0x9, PTR("luckyStart"), LW(5), LW(6), LW(7), (s32)getLuckyStart, 0, EVT_NULLPTR)
    END_IF()
  RETURN()
  EVT_END()
  
  EVT_BEGIN(kamek_death)
    //IF_EQUAL(UW(0), 1)
        USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(15), 4, 1)
        BROTHER_EVT_ID(LW(10))
          SET(LW(9), FLOAT(0.0))
          DO(0)
          ADDF(LW(9), FLOAT(9.0))
          USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, LW(9), EVT_NULLPTR)
          WAIT_FRM(1)
          WHILE()
        END_BROTHER()
        WAIT_MSEC(1000)
        DELETE_EVT(LW(10))
        USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
        IF_EQUAL(LF(2), 0)
          USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(5), LW(6), LW(7), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
          USER_FUNC(spm::evt_pouch::evt_pouch_increment_enemies_defeated)
        ELSE()
          USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 13, LW(5), LW(6), LW(7), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
        END_IF()
        USER_FUNC(spm::evt_npc::evt_npc_delete, LW(15))
        USER_FUNC(npc_change_name, LW(15), PTR("deadNPC"))
        USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 0, LW(14))
        USER_FUNC(spm::evt_mobj::evt_mobj_delete, LW(14))
        RUN_CHILD_EVT(addLuckyStart)
    //END_IF()
  RETURN()
  EVT_END()

  EVT_BEGIN(kamek_spell)
    USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_MAGIC_SING1"))
    USER_FUNC(evt_npc_set_anim, LW(15), 6, 1)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_MAGIC_SING1"))
    USER_FUNC(evt_npc_set_anim, LW(15), 7, 1)
    USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 8, 1)
    WAIT_MSEC(500)
    USER_FUNC(evt_npc_set_anim, LW(15), 9, 1)
    USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_AGERU1"))
    USER_FUNC(evt_npc_set_anim, LW(15), 10, 1)
    RETURN()
  EVT_END()

  EVT_BEGIN(check_for_the_bois)
    SET(LW(1), 0)
    USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 0, 0, 0, LW(0))
    IF_FLAG(LW(0), 0x8000)
      SET(LW(1), 1)
    END_IF()
    USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 2, 0, 0, LW(0))
    IF_FLAG(LW(0), 0x8000)
      ADD(LW(1), 1)
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 15, LW(0))
    IF_SMALL(LW(0), 3)
      ADD(LW(1), 1)
    ELSE()
      USER_FUNC(rpg_set_dialogue, PTR(btl_kamek_dialogue_2))
    END_IF()
    IF_EQUAL(LW(1), 3)
      ADD(LW(0), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 15, LW(0))
      RUN_CHILD_EVT(kamek_spell)
      USER_FUNC(evt_npc_get_position, LW(15), LW(0), LW(1), LW(2))
      SUB(LW(0), 50)
      USER_FUNC(setRpgActivity, 0, 1)
      USER_FUNC(get_rpg_enemy_info, 0, LW(3), LW(4), LW(5), LW(6))
      IF_EQUAL(LW(3), -1)
        RETURN()
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("npc1"), LW(4), 0)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("npc1"), LW(0), LW(1), LW(2))
      USER_FUNC(spm::evt_mobj::evt_mobj_blk, 4, PTR("mobj1"), 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_mobj::evt_mobj_set_position, PTR("mobj1"), LW(0), 0, LW(2))
      USER_FUNC(mobjChangeAnimPoseName, PTR("mobj1"), PTR("MOBJ_su_blank"))
      USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 0, PTR("mobj1"))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("npc1"), 0, LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("npc1"), 1, LW(1))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("npc1"), 2, LW(2))
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("npc1"), 0xe, LW(3))
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("npc1"), 0xd, LW(6))
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("npc1"), 0, 1)
      USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("npc1"), 1, 201326596)
      USER_FUNC(spm::an2_08::evt_rpg_status_remove, 0, 0, 0x8000)
      USER_FUNC(spm::an2_08::evt_rpg_status_remove, 0, 0, 0x3)
      USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 0, -2, 0, EVT_NULLPTR)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(evt_npc_get_position, LW(15), LW(0), LW(1), LW(2))
      ADD(LW(0), 50)
      USER_FUNC(setRpgActivity, 2, 1)
      USER_FUNC(get_rpg_enemy_info, 2, LW(3), LW(4), LW(5), LW(6))
      IF_EQUAL(LW(3), -1)
        RETURN()
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("npc3"), LW(4), 0)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("npc3"), LW(0), LW(1), LW(2))
      USER_FUNC(spm::evt_mobj::evt_mobj_blk, 4, PTR("mobj3"), 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_mobj::evt_mobj_set_position, PTR("mobj3"), LW(0), 0, LW(2))
      USER_FUNC(mobjChangeAnimPoseName, PTR("mobj3"), PTR("MOBJ_su_blank"))
      USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 0, PTR("mobj3"))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("npc3"), 0, LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("npc3"), 1, LW(1))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("npc3"), 2, LW(2))
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("npc3"), 0xe, LW(3))
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("npc3"), 0xd, LW(6))
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("npc3"), 0, 1)
      USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("npc3"), 1, 201326596)
      USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 2, -2, 0, EVT_NULLPTR)
      USER_FUNC(spm::an2_08::evt_rpg_status_remove, 0, 2, 0x8000)
      USER_FUNC(spm::an2_08::evt_rpg_status_remove, 0, 2, 0x3)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
      SET(LW(0), 1)
    ELSE()
      SET(LW(0), 0)
    END_IF()
  RETURN()
  EVT_END()

  EVT_BEGIN(kamek_onspawn)
    USER_FUNC(evt_npc_delete, PTR("peach"))
    USER_FUNC(evt_npc_delete, PTR("kamek"))
  RETURN()
  EVT_END()

  EVT_BEGIN(kamek_attack)
    SET(LW(0), 0)
    INLINE_EVT()
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE()
    RUN_CHILD_EVT(check_for_the_bois)
    IF_EQUAL(LW(0), 0)
      USER_FUNC(spm::an2_08::evt_rpg_enemy_death_check, 0, LW(1))
      IF_NOT_FLAG(LW(1), 0x3)
        IF_NOT_FLAG(LW(1), 0x8000)
          SET(LW(0), 2)
        END_IF()
      END_IF()
      USER_FUNC(spm::an2_08::evt_rpg_enemy_death_check, 2, LW(1))
      IF_NOT_FLAG(LW(1), 0x3)
        IF_NOT_FLAG(LW(1), 0x8000)
          SET(LW(0), 2)
        END_IF()
      END_IF()
    END_IF()
    SWITCH(LW(0))
      CASE_EQUAL(0)
        USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_MAGIC_SING1"))
        USER_FUNC(evt_npc_set_anim, LW(15), 6, 1)
        WAIT_MSEC(500)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_MAGIC_SING1"))
        USER_FUNC(evt_npc_set_anim, LW(15), 7, 1)
        USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
        USER_FUNC(evt_npc_set_anim, LW(15), 8, 1)
        WAIT_MSEC(500)
        USER_FUNC(evt_npc_set_anim, LW(15), 9, 1)
        USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_AGERU1"))
        USER_FUNC(evt_npc_set_anim, LW(15), 10, 1)
        WAIT_FRM(11)
        USER_FUNC(check_guards, 3, 11, LW(11))
        IF_EQUAL(LW(11), 2)
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
          USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(6), LW(7), 20, 300)
          BROTHER_EVT_ID(LW(4))
          RUN_CHILD_EVT(superguard_stylish)
          END_BROTHER()
        END_IF()
        IF_EQUAL(LW(11), 1)
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
            USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
            SUB(LW(10), 1)
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
    CASE_EQUAL(2)
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 14, LW(0))
      IF_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 14, 1)
        USER_FUNC(rpg_set_dialogue, PTR(btl_kamek_dialogue_1))
      END_IF()
      RUN_CHILD_EVT(kamek_spell)
      USER_FUNC(spm::an2_08::evt_rpg_enemy_death_check, 0, LW(1))
      IF_NOT_FLAG(LW(1), 0x3)
        IF_NOT_FLAG(LW(1), 0x8000)
          USER_FUNC(spm::an2_08::evt_rpg_status_remove, 1, 0, 0x3)
          USER_FUNC(spm::evt_npc::evt_npc_set_animpose_disp_callback, PTR("npc1"), PTR(spm::mi4::mi4MimiHolographicEffect), 0)
          USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 0, -2, 0, EVT_NULLPTR)
        END_IF()
      END_IF()
      USER_FUNC(spm::an2_08::evt_rpg_enemy_death_check, 2, LW(1))
      IF_NOT_FLAG(LW(1), 0x3)
        IF_NOT_FLAG(LW(1), 0x8000)
          USER_FUNC(spm::an2_08::evt_rpg_status_remove, 1, 2, 0x3)
          USER_FUNC(spm::evt_npc::evt_npc_set_animpose_disp_callback, PTR("npc3"), PTR(spm::mi4::mi4MimiHolographicEffect), 0)
          USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 2, -2, 0, EVT_NULLPTR)
        END_IF()
      END_IF()
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_POWERUP1"))
    END_SWITCH()
  WAIT_FRM(60)
  USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
  INLINE_EVT()
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
  END_INLINE()
  USER_FUNC(ac_success_reset)
  LBL(5)
    CHK_EVT(LW(4), LW(0))
    IF_EQUAL(LW(0), 0)
      SET(LW(0), 0)
      RETURN()
    END_IF()
    WAIT_FRM(1)
  GOTO(5)
  EVT_END()

  void kamek_main()
  {
    npcTribes[63].maxHp = 55;
    npcTribes[63].attackStrength = 4;
  }

}
