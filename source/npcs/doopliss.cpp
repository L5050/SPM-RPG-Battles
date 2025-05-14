#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
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

NPCTribeAnimDef animsDoopliss[] = {
    {0, "S_1"},
    {1, "W_1"},
    {2, "R_1"},
    {3, "T_1"},
    {4, "D_1"},
    {6, "A_1A"},
    {7, "A_1B"},
    {8, "A_2"},
    {9, "A_3A"},
    {10, "J_1A"},
    {11, "J_1B"},
    {12, "A_3B"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsDooplissMario[] = {
    {0, "mario_S_1"},
    {1, "mario_W_1"},
    {2, "mario_R_1"},
    {3, "mario_T_1"},
    {4, "mario_D_2"},
    {6, "mario_H_1A"},
    {7, "mario_H_1B"},
    {8, "mario_H_1C"},
    {9, "mario_T_8"},
    {10, "mario_J_1A"},
    {11, "mario_J_1B"},
    {12, "mario_J_1C"},
    {13, "mario_D_4"},
    {-1, nullptr}
  };

  NPCTribeAnimDef animsWhipGuy[] = {
    {0, "n_stg2_kansyuS_1"},
    {1, "n_stg2_kansyuW_1"},
    {2, "n_stg2_kansyuR_1"},
    {3, "n_stg2_kansyuT_1"},
    {-1, nullptr}
  };

  NPCTribeAnimDef * getDooplissAnims()
  {
    return animsDoopliss;
  }

  const char * tippi_wtf = "<p><fairy>\n"
  "Where... are we?\n"
  "How did we even get here?\n"
  "<k>\n";

  const char * kansyu_wtf = "<p>\n"
  "Hm... I recognize you.\n"
  "<wait 150>You're Mario. We've met\n"
  "in a previous adventure.\n"
  "<k>\n"
  "<p>\n"
  "Do you recognize me?\n"
  "<wait 150>Guess, whats my name?\n"
  "<wait 150>Hit me with your best shot!\n"
  "<k>\n";

  const char * kansyu_no_response = "<p>\n"
  "Speechless?\n"
  "<wait 150>Come on, whats my name?\n"
  "<wait 150>Hit me with your best shot!\n"
  "<k>\n";

  const char * kansyu_questioning = "<p>\n"
  "%s?\n"
  "<k>\n";

  const char * doopliss_nope = "<p>\n"
  "Wrongamundo little Mario!\n"
  "Who'd name their kid that?\n"
  "<k>\n"
  "<p>\n"
  "Long time no see, slick.\n"
  "<wait 150>I see you were tricked by\n"
  "Mimi as well.\n"
  "<k>\n"
  "<p>\n"
  "I must flee this place before she\n"
  "traps me again, but I wouldn't be\n"
  "against some trickery first...\n"
  "<k>\n";

  const char * doopliss_yup = "<p>\n"
  "Correct!\n"
  "Your eyes have gotten sharper\n"
  "since I saw you last.\n"
  "<k>\n"
  "<p>\n"
  "Long time no see, slick.\n"
  "<wait 150>I see you were tricked by\n"
  "Mimi as well.\n"
  "<k>\n"
  "<p>\n"
  "I must flee this place before she\n"
  "traps me again, but I wouldn't be\n"
  "against some trickery first...\n"
  "<k>\n";

  const char * doopliss_death_feign_1 = "<p>\n"
  "Not bad, slick!\n"
  "You've gotten strong.\n"
  "<scale 0.4>I haven't fought against pixls in centuries...\n"
  "<k>\n";

  const char * doopliss_death_feign_2 = "<p>\n"
  "But so have I!\n"
  "I've got something very special\n"
  "for you here...\n"
  "<k>\n";

  const char * doopliss_transform_system = "<p>\n"
  "Doopliss turned into Mario!"
  "<k>\n"
  "<o>\n";

  static const char * superguard = "<p>\n"
  "%s superguarded!"
  "<k>\n";

s32 fix_doopliss_being_annoying(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  char * name = (char *)spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  NPCEntry * npc = spm::npcdrv::npcNameToPtr(name);
  npc->name[0] = "e";
  return 2;
}

EVT_DECLARE_USER_FUNC(fix_doopliss_being_annoying, 1)

  EVT_BEGIN(doopliss_cutscene)
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
  WAIT_MSEC(2000)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(tippi_wtf), 0, PTR("__guide__"))
  INLINE_EVT()
  USER_FUNC(spm::evt_mario::evt_mario_walk_to, FLOAT(50.0), FLOAT(0.0), 2000)
  END_INLINE()
  USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 100, EVT_NULLPTR, EVT_NULLPTR, 100, EVT_NULLPTR, EVT_NULLPTR, 2000, 11)
  WAIT_MSEC(2500)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(kansyu_wtf), 0, PTR("kansyu"))
  LBL(1)
  USER_FUNC(spm::evt_fade::evt_fade_entry, 2, 300, 0, 0, 0, 255)
  USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
  USER_FUNC(spm::evt_map::evt_mapdisp_onoff, 0)
  USER_FUNC(spm::evt_sub::func_800d8700, LW(0))
  USER_FUNC(spm::evt_map::evt_mapdisp_onoff, 1)
  USER_FUNC(spm::evt_fade::evt_fade_entry, 1, 300, 0, 0, 0, 255)
  USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
  IF_EQUAL(LW(0), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(kansyu_no_response), 0, PTR("kansyu"))
    GOTO(1)
  END_IF()
  USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(kansyu_questioning), 0, PTR("kansyu"), LW(0))
  USER_FUNC(compareStrings, LW(0), PTR("Doopliss"), LW(0))
  USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 100, EVT_NULLPTR, EVT_NULLPTR, 100, EVT_NULLPTR, 175, 1000, 11)
  USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
  WAIT_MSEC(1000)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, 125, 0, 10, FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(evt_npc_set_position, PTR("kansyu"), 125, -100, 0)
  USER_FUNC(evt_npc_set_position, PTR("doopliss"), 125, -2, 0)
  USER_FUNC(evt_npc_set_property, PTR("doopliss"), 14, (s32)animsDoopliss)
  USER_FUNC(evt_npc_set_anim, PTR("doopliss"), 0, 1)
  IF_EQUAL(LW(0), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(doopliss_nope), 0, PTR("doopliss"))
  ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(doopliss_yup), 0, PTR("doopliss"))
  END_IF()
  USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_RELAXATION1"))
  USER_FUNC(start_boss_fight, 529)
  WAIT_MSEC(800)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("doopliss"), 0, -100, 0)
  RETURN()
  EVT_END()

  EVT_BEGIN(hampter_room_init)
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_STG2"))
    USER_FUNC(spm::evt_mario::evt_mario_set_character, 0)
    RUN_CHILD_EVT(spm::evt_door::door_init_evt)
    USER_FUNC(evt_npc_entry, PTR("kansyu"), PTR("n_stg2_kansyu"), 0)
    USER_FUNC(evt_npc_set_position, PTR("kansyu"), 125, 0, 0)
    USER_FUNC(evt_npc_set_property, PTR("kansyu"), 14, (s32)animsWhipGuy)
    USER_FUNC(evt_npc_set_anim, PTR("kansyu"), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("doopliss"), PTR("c_ranpel"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("doopliss"), 0, -100, 0)
    RUN_EVT(doopliss_cutscene)
  RETURN()
  EVT_END()

  EVT_BEGIN(doopliss_mario_attack)
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    ADD(LW(0), 40)
    INLINE_EVT()
      SET(LW(13), UW(3))
      SUB(LW(13), 25)
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(0), EVT_NULLPTR, LW(13), LW(0), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE()
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 2, 1)
    USER_FUNC(evt_npc_walk_to, PTR("npc2"), LW(0), 0, LW(2), FLOAT(180.0), 0, 0, 0)
    WAIT_FRM(1)
    INLINE_EVT()
      SET(LW(10), 0)
      LBL(1)
        IF_SMALL(LW(10), 20)
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
            IF_LARGE_EQUAL(LW(10), 27)
              USER_FUNC(superguard_toggle)
              USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
            END_IF()
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
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 6, 1)
    USER_FUNC(evt_npc_wait_anim_end, PTR("npc2"), 0)
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 8, 1)
    USER_FUNC(evt_npc_wait_anim_end, PTR("npc2"), 0)
    USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
    USER_FUNC(check_ac_success, LW(11))
    IF_EQUAL(LW(11), 1)
      USER_FUNC(check_superguard_success, LW(11))
      IF_EQUAL(LW(11), 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
      USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
      USER_FUNC(evt_npc_set_anim, PTR("npc2"), 4, 1)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(1), LW(2), 20, 300)
      BROTHER_EVT_ID(LW(4))
        USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 1, PTR(superguard), LW(3))
      END_BROTHER()
      WAIT_MSEC(500)
      ELSE()
        SET(LW(11), 1)
        USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
        ADD(LW(10), 2) // Bonus damage that Doopliss' hammer gets
        SUB(LW(10), 2) // Defense from guarding
        IF_SMALL(LW(10), 0)
          SET(LW(10), 0)
        END_IF()
        RUN_CHILD_EVT(mod::marioRPGtakeDamage)
        USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
        BROTHER_EVT_ID(LW(4))
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
          USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
          USER_FUNC(spm::evt_msg::evt_msg_continue)
        END_BROTHER()
      END_IF()
    ELSE()
      USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
      ADD(LW(10), 2) // Bonus damage that Doopliss' hammer gets
      RUN_CHILD_EVT(mod::marioRPGtakeDamage)
      USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
      BROTHER_EVT_ID(LW(4))
        USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
        USER_FUNC(spm::evt_msg::evt_msg_continue)
      END_BROTHER()
    END_IF()
    INLINE_EVT()
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("npc2"), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("npc2"), 1, LW(1))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("npc2"), 2, LW(2))
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 2, 1)
    USER_FUNC(evt_npc_walk_to, PTR("npc2"), LW(0), LW(1), LW(2), FLOAT(180.0), 0, 0, 0)
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 0, 1)
    USER_FUNC(ac_success_reset)
    LBL(5)
    CHK_EVT(LW(4), LW(0))
    IF_EQUAL(LW(0), 0)
      RETURN()
    END_IF()
    WAIT_FRM(1)
    GOTO(5)
  EVT_END()

  EVT_BEGIN(doopliss_attack)
    USER_FUNC(evt_npc_get_unitwork, PTR("npc2"), 3, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
      RUN_CHILD_EVT(doopliss_mario_attack)
      RETURN()
    END_IF()
    USER_FUNC(spm::evt_sub::evt_sub_random, 1, LW(0))
    USER_FUNC(evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
    SET(LW(15), LW(5))
    SWITCH(LW(0))
      CASE_EQUAL(0)
        USER_FUNC(evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
        ADD(LW(5), -150)
        SET(LW(12), 150)
        INLINE_EVT()
          SET(LW(13), UW(3))
          SUB(LW(13), 25)
          USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
        END_INLINE()
        USER_FUNC(evt_npc_set_anim, PTR("npc2"), 2, 1)
        USER_FUNC(evt_npc_walk_to, PTR("npc2"), LW(5), 0, LW(7), FLOAT(140.0), 0, 0, 0)
        USER_FUNC(evt_npc_set_anim, PTR("npc2"), 11, 1)
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        INLINE_EVT()
          SET(LW(10), 0)
          LBL(1)
            IF_SMALL(LW(10), 20)
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
                IF_LARGE_EQUAL(LW(10), 27)
                  USER_FUNC(superguard_toggle)
                  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
                END_IF()
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
        USER_FUNC(evt_npc_jump_to, PTR("npc2"), LW(0), LW(6), LW(7), 50, FLOAT(500.0))
        USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
        USER_FUNC(check_ac_success, LW(11))
        IF_EQUAL(LW(11), 1)
          USER_FUNC(check_superguard_success, LW(11))
          IF_EQUAL(LW(11), 1)
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
          USER_FUNC(evt_npc_set_anim, PTR("npc2"), 4, 1)
          USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(6), LW(7), 20, 300)
          BROTHER_EVT_ID(LW(4))
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
          USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 1, PTR(superguard), LW(3))
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
            BROTHER_EVT_ID(LW(4))
            USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
            USER_FUNC(spm::evt_msg::evt_msg_continue)
            END_BROTHER()
          END_IF()
        ELSE()
          USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
          RUN_CHILD_EVT(mod::marioRPGtakeDamage)
          USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          BROTHER_EVT_ID(LW(4))
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
          USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
          USER_FUNC(spm::evt_msg::evt_msg_continue)
          END_BROTHER()
        END_IF()
      CASE_EQUAL(1)
        USER_FUNC(evt_npc_set_anim, PTR("npc2"), 6, 1)
        USER_FUNC(evt_npc_wait_anim_end, PTR("npc2"), 1)
        USER_FUNC(evt_npc_set_anim, PTR("npc2"), 7, 1)
        WAIT_MSEC(500)
        USER_FUNC(evt_npc_set_anim, PTR("npc2"), 8, 1)
        USER_FUNC(evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
        ADD(LW(6), 50)
        USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, UW(3), LW(5), EVT_NULLPTR, 200, 2000, 11)
        USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("npc2"), LW(5), LW(6), LW(7), 0, FLOAT(40.0), 0, 11, 0, 0)
        SUB(LW(6), 50)
        ADD(LW(5), -150)
        SET(LW(12), 150)
        INLINE_EVT()
          SET(LW(13), UW(3))
          SUB(LW(13), 25)
          USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
        END_INLINE()
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        INLINE_EVT()
        SET(LW(10), 0)
        LBL(1)
          IF_SMALL(LW(10), 20)
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
              IF_LARGE_EQUAL(LW(10), 27)
                USER_FUNC(superguard_toggle)
                USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1A"), 0)
              END_IF()
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
        WAIT_FRM(10)
        USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("npc2"), LW(0), LW(6), LW(7), 0, FLOAT(700.0), 0, 0, 0, 0)
        //USER_FUNC(evt_npc_jump_to, PTR("npc2"), LW(0), LW(6), LW(7), 50, FLOAT(500.0))
        USER_FUNC(spm::an2_08::evt_rpg_char_get, LW(3))
        USER_FUNC(check_ac_success, LW(11))
        IF_EQUAL(LW(11), 1)
          USER_FUNC(check_superguard_success, LW(11))
          IF_EQUAL(LW(11), 1)
          USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
          USER_FUNC(evt_npc_set_anim, PTR("npc2"), 4, 1)
          USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(6), LW(7), 20, 300)
          BROTHER_EVT_ID(LW(4))
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
          USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 1, PTR(superguard), LW(3))
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
            BROTHER_EVT_ID(LW(4))
            USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
            USER_FUNC(spm::evt_msg::evt_msg_continue)
            END_BROTHER()
          END_IF()
        ELSE()
          USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
          RUN_CHILD_EVT(mod::marioRPGtakeDamage)
          USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
          BROTHER_EVT_ID(LW(4))
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
          USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
          USER_FUNC(spm::evt_msg::evt_msg_continue)
          END_BROTHER()
        END_IF()
    END_SWITCH()
  USER_FUNC(evt_npc_set_anim, PTR("npc2"), 2, 1)
  ADD(LW(5), 150)
  INLINE_EVT()
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
  END_INLINE()
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("npc2"), 0, LW(0))
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("npc2"), 1, LW(1))
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("npc2"), 2, LW(2))
  USER_FUNC(evt_npc_walk_to, PTR("npc2"), LW(0), LW(1), LW(2), FLOAT(140.0), 0, 0, 0)
  USER_FUNC(evt_npc_set_anim, PTR("npc2"), 0, 1)
  USER_FUNC(ac_success_reset)
  LBL(5)
  CHK_EVT(LW(4), LW(0))
  IF_EQUAL(LW(0), 0)
    RETURN()
  END_IF()
  WAIT_FRM(1)
  GOTO(5)
  EVT_END()


  EVT_BEGIN(doopliss_death)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(enable_disable_rpg_menu, 0)
    USER_FUNC(evt_npc_get_unitwork, PTR("npc2"), 3, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
      USER_FUNC(evt_npc_set_anim, PTR("npc2"), 13, 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_CRUSH1"))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_LAND_COMICAL1"))
      WAIT_MSEC(1000)
      USER_FUNC(evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(5), LW(6), LW(7), FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(evt_npc_delete, PTR("npc2"))
      RETURN()
    END_IF()
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    WAIT_FRM(5)
    USER_FUNC(evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
    ADD(LW(5), -100)
    SET(LW(13), UW(3))
    SUB(LW(13), 100)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(doopliss_death_feign_1), 0, PTR("npc2"))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(doopliss_death_feign_2), 0, PTR("npc2"))
    INLINE_EVT()
      SUB(LW(13), 10)
      ADD(LW(5), 50)
      USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
    END_INLINE()
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 9, 1)
    WAIT_FRM(20)
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 9, 1)
    WAIT_FRM(20)
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 9, 1)
    USER_FUNC(evt_npc_wait_anim_end, PTR("npc2"), 0)
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 12, 1)
    WAIT_FRM(10)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_STOP_WATCH1"))
    BROTHER_EVT_ID(LW(5))
      USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_init, 11, 0, 255, 500)
      DO(0)
        USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_get_value)
        USER_FUNC(evt_npc_set_color, PTR("npc2"), 255, LW(0), 0, 255)
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
        DO_BREAK()
        END_IF()
      WHILE()
      USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_init, 11, 255, 0, 500)
      DO(0)
        USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_get_value)
        USER_FUNC(evt_npc_set_color, PTR("npc2"), LW(0), 255, 0, 255)
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
        DO_BREAK()
        END_IF()
      WHILE()
      USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_init, 11, 0, 255, 500)
      DO(0)
        USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_get_value)
        USER_FUNC(evt_npc_set_color, PTR("npc2"), LW(0), 255, LW(0), 255)
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
        DO_BREAK()
        END_IF()
      WHILE()
    END_BROTHER()
    DO(0)
      CHK_EVT(LW(5), LW(6))
      IF_EQUAL(LW(6), 0)
        DO_BREAK()
      END_IF()
      WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
    USER_FUNC(evt_npc_delete, PTR("npc2"))
    USER_FUNC(fix_doopliss_being_annoying, PTR("npc2"))
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("npc2"), PTR("p_mario"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("npc2"), LW(5), LW(6), LW(7))
    USER_FUNC(evt_npc_set_disp_callback, PTR("npc2"), PTR(marioLuigiNpcDispCb))
    USER_FUNC(evt_npc_set_property, PTR("npc2"), 14, (s32)animsDooplissMario)
    USER_FUNC(evt_npc_set_anim, PTR("npc2"), 0, 1)
    USER_FUNC(evt_npc_set_color, PTR("npc2"), 252, 3, 53, 255)
    USER_FUNC(evt_npc_set_unitwork, PTR("npc2"), 0, LW(5))
    USER_FUNC(evt_npc_set_unitwork, PTR("npc2"), 1, LW(6))
    USER_FUNC(evt_npc_set_unitwork, PTR("npc2"), 2, LW(7))
    USER_FUNC(evt_npc_set_unitwork, PTR("npc2"), 3, 1)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 7, LW(5), LW(6), LW(7), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_NEMURE_SUCCESS1"))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
    USER_FUNC(spm::evt_msg::evt_msg_print_add, 1, PTR(doopliss_transform_system))
    USER_FUNC(spm::an2_08::evt_rpg_status_remove, 0, 1, 0x8000)
    USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, 1, -15, 0, LW(0))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 500, 11)
    WAIT_FRM(4)
    RETURN()
  EVT_END()

  void doopliss_main()
  {
    npcTribes[529].animDefs = animsDoopliss;
    npcTribes[529].animPoseName = "c_ranpel";
    npcTribes[529].maxHp = 15;
    npcTribes[529].killXp = 3500;
    npcTribes[529].attackStrength = 4;
    spm::map_data::MapData * mi3_03_md = spm::map_data::mapDataPtr("mi3_03");
    evtpatch::hookEvtReplace(mi3_03_md->initScript, 32, (spm::evtmgr::EvtScriptCode*)hampter_room_init);
  }

}
