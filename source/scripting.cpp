#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <spm/iValues.h>
#include <spm/evtmgr.h>
#include <spm/evt_msg.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_paper.h>
#include <spm/evt_img.h>
#include <spm/evt_snd.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_npc.h>
#include <spm/evt_door.h>
#include <spm/evt_case.h>
#include <spm/evt_pouch.h>
#include <spm/evt_seq.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/item_data.h>
#include <spm/item_event_data.h>
#include <wii/os/OSError.h>
#include <patch.h>
#include <string>
using namespace std;
namespace mod {

EVT_DECLARE_USER_FUNC(npcEntryFromTribeId, 1)
//EVT_DECLARE_USER_FUNC(osReportLWZero, 0)

EVT_BEGIN(techChild1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_MISS1"))
    DO(3)
        USER_FUNC(spm::evt_mario::evt_rpg_status_remove, 1, LW(10), 32)
        WAIT_FRM(3)
        USER_FUNC(spm::evt_mario::evt_rpg_status_remove, 0, LW(10), 32)
        WAIT_FRM(3)
    WHILE()
    RETURN()
EVT_END()

EVT_BEGIN(techChild2)
    DO(4)
        USER_FUNC(spm::evt_mario::evt_rpg_status_remove, 1, LW(10), 16)
        WAIT_FRM(4)
        USER_FUNC(spm::evt_mario::evt_rpg_status_remove, 0, LW(10), 16)
        WAIT_FRM(4)
    WHILE()
    RETURN()
EVT_END()

EVT_BEGIN(marioRPGtakeDamage)
    USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, PTR("SFX_P_MARIO_DAMAGE1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
    USER_FUNC(spm::evt_cam::evt_cam_shake, 11, FLOAT(1.5), FLOAT(1.5), FLOAT(0.0), 200, 0)
    RETURN()
EVT_END()

EVT_BEGIN(marioRPGdodge)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_MISS1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_EVT_HELWANWAN_MISS1"))
    RETURN()
EVT_END()

EVT_BEGIN(checkMarioDeath)
    USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
    IF_NOT_FLAG(LW(0), 0x8000)
        RETURN()
    END_IF()
    USER_FUNC(spm::evt_pouch::evt_pouch_check_have_item, 84, LW(0))
    IF_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
                USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_107"))
            CASE_EQUAL(2)
                USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_108"))
            CASE_EQUAL(3)
                USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_109"))
        END_SWITCH()
        USER_FUNC(spm::evt_fade::evt_fade_entry, 65538, 1000, 0, 0, 0, 255)
        USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
        USER_FUNC(spm::evt_seq::evt_seq_set_seq, 4, 0, 0)
    ELSE()
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(0))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_110"), LW(0))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_KINKYU_KINOKO1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_RECOVER_HP_EFFECT_SMALL1"))
        WAIT_MSEC(350)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_RECOVER_HP_SMALL1"))
        USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_111"))
        USER_FUNC(spm::evt_pouch::evt_pouch_add_hp, 5)
        USER_FUNC(spm::evt_pouch::evt_pouch_remove_item, 84)
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 32768)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(underchompAttack1)
    USER_FUNC(spm::evt_mario::evt_rpg_enemy_death_check, 0, LW(0))
    USER_FUNC(spm::evt_mario::evt_rpg_underchomp_name_grab, 0, LW(1))
    IF_FLAG(LW(0), 0x8000)
        RETURN()
    END_IF()
    IF_FLAG(LW(0), 0x3)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_112"), LW(1))
        RETURN()
    END_IF()
    IF_FLAG(LW(0), 0x4)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_114"), LW(1))
        RETURN()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(2))
    USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(3))
    IF_FLAG(LW(2), 0x5)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_113"), LW(1), LW(3))
        RETURN()
    END_IF()
    USER_FUNC(spm::evt_sub::evt_sub_random, 3000, LW(10))
    SWITCH(LW(10))
        CASE_SMALL(1000)
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_093"), LW(1))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_ATTACK1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_EVT_HELWANWAN_ATTACK1"))
            USER_FUNC(spm::evt_sub::evt_sub_random, 1000, LW(10))
            USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
            IF_FLAG(LW(0), 0x600)
                SET(LW(10), 1000)
            END_IF()
            IF_SMALL(LW(10), 300)
                RUN_CHILD_EVT(mod::marioRPGdodge)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_096"), LW(3))
            ELSE()
                USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
                IF_FLAG(LW(0), 0x10)
                    USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_095"))
                ELSE()
                    RUN_CHILD_EVT(mod::marioRPGtakeDamage)
                    USER_FUNC(spm::evt_mario::evt_rpg_calc_mario_damage, 0, LW(10))
                    USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_094"), LW(3), LW(10))
                    USER_FUNC(spm::evt_mario::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
                END_IF()
            END_IF()
        CASE_SMALL(2000)
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_097"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_FIRE1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_EVT_HELWANWAN_FIRE1"))
            USER_FUNC(spm::evt_sub::evt_sub_random, 1000, LW(10))
            USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
            IF_FLAG(LW(0), 0x600)
                SET(LW(10), 1000)
            END_IF()
            IF_SMALL(LW(10), 300)
                RUN_CHILD_EVT(mod::marioRPGdodge)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_099"), LW(3))
            ELSE()
                USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
                IF_FLAG(LW(0), 0x10)
                    USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_095"))
                ELSE()
                    RUN_CHILD_EVT(mod::marioRPGtakeDamage)
                    USER_FUNC(spm::evt_mario::evt_rpg_calc_mario_damage, 0, LW(10))
                    USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_098"), LW(3), LW(10))
                    USER_FUNC(spm::evt_mario::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
                END_IF()
            END_IF()
        CASE_ETC()
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_HELWANWAN_POWERUP1"))
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_106"), LW(1))
            USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_EVT_HELWANWAN_POWERUP1"))
    END_SWITCH()
    RUN_CHILD_EVT(mod::checkMarioDeath)
    RETURN()
EVT_END()

EVT_BEGIN(takeDamageSFX)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_DAMAGE1"))
  USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
  USER_FUNC(spm::evt_cam::evt_cam_shake, 11, FLOAT(1.5), FLOAT(1.5), FLOAT(0.0), 200, 0)
  RETURN()
EVT_END()

EVT_BEGIN(rpgDeathHandler)
  USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
  IF_NOT_FLAG(LW(0), 0x8000)
      RETURN()
  END_IF()
  USER_FUNC(spm::evt_pouch::evt_pouch_check_have_item, 84, LW(0)) //checks if mario has a life shroom
  IF_EQUAL(LW(0), 0) //if not, rpg death screen plays
      USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
      SWITCH(LW(0))
          CASE_EQUAL(0)
              USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_107"))
          CASE_EQUAL(2)
              USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_108"))
          CASE_EQUAL(3)
              USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_109"))
      END_SWITCH()
      USER_FUNC(spm::evt_fade::evt_fade_entry, 65538, 1000, 0, 0, 0, 255)
      USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
      USER_FUNC(spm::evt_seq::evt_seq_set_seq, 4, 0, 0)
  ELSE() //if yes, life shroom revives the party
      USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(0))
      USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_110"), LW(0))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_KINKYU_KINOKO1"))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_RECOVER_HP_EFFECT_SMALL1"))
      WAIT_MSEC(350)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_RECOVER_HP_SMALL1"))
      USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_111"))
      USER_FUNC(spm::evt_pouch::evt_pouch_add_hp, 5)
      USER_FUNC(spm::evt_pouch::evt_pouch_remove_item, 84)
      USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 32768)
  END_IF()
  RETURN()
EVT_END()

EVT_BEGIN(attack)
    USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(10))
    USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_004"), LW(10))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_MARIO_JUMP1"), PTR("SFX_P_PEACH_JUMP1"), PTR("SFX_P_KOOPA_JUMP1"), PTR("SFX_P_LUIGI_JUMP1"))
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_sub::evt_sub_random, 1000, LW(10))
    IF_SMALL(LW(10), 100)
        SET(LW(10), LW(2))
        RUN_CHILD_EVT(mod::techChild1)
        USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_006"))
    ELSE()
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
        SET(LW(10), LW(2))
        RUN_CHILD_EVT(PTR(mod::techChild2))
        USER_FUNC(spm::evt_mario::evt_mario_calc_rpg_damage, LW(2), 0, LW(10))
        USER_FUNC(spm::evt_sub::evt_sub_get_language, LW(5))
        SWITCH(LW(5))
            CASE_EQUAL(0)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_005"), LW(1), LW(10))
            CASE_EQUAL(3)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_005"), LW(1), LW(10))
            CASE_EQUAL(5)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_005"), LW(1), LW(10))
            CASE_EQUAL(6)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_005"), LW(1), LW(10))
            CASE_EQUAL(4)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_005"), LW(1), LW(10))
            CASE_ETC()
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_005"), LW(10), LW(1))
        END_SWITCH()
        USER_FUNC(spm::evt_mario::evt_rpg_char_death_check, LW(2), LW(10), 0, LW(0))
        IF_FLAG(LW(0), 0x8000)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_ENEMY_DIE1"))
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_007"), LW(1)) //You defeated [enemy]!
        ELSE()
            IF_FLAG(LW(0), 0x1)
                USER_FUNC(spm::evt_mario::evt_rpg_status_remove, 0, LW(2), 1)
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_115"), LW(1))
            END_IF()
        END_IF()
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(technique)
USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(10))
SWITCH(LW(10))
    CASE_EQUAL(0)
        USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_011"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_VOICE1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_EFFECT1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_CURSOR1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_MARIO_3D2D_CURSOR1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_CHANGE1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_MARIO_3D2D_CHANGE1"))
        USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_012"))
        CASE_EQUAL(1)
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_011"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_VOICE1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_EFFECT1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_CURSOR1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_MARIO_3D2D_CURSOR1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_3D2D_CHANGE1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_MARIO_3D2D_CHANGE1"))
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_012"))
    CASE_EQUAL(2)
        USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_014"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_KOOPA_FIRE1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_KOOPA_FLAME1"))
        USER_FUNC(spm::evt_snd::evt_snd_get_last_sfx_id, LW(0))
        WAIT_MSEC(600)
        USER_FUNC(spm::evt_snd::func_800d2fa4, LW(0), 1000)
        SET(LW(10), LW(4))
        RUN_CHILD_EVT(mod::techChild1)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_015"), LW(3))
    CASE_EQUAL(3)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_017"), LW(3))
        USER_FUNC(spm::evt_sub::evt_sub_random, 1000, LW(10))
        IF_SMALL(LW(10), 150)
            SET(LW(10), LW(4))
            RUN_CHILD_EVT(mod::techChild1)
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_019"))
        ELSE()
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_LUIGI_SUPERJUMP_SET1"))
            WAIT_MSEC(300)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_LUIGI_SUPERJUMP_SET2"))
            WAIT_MSEC(300)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_LUIGI_SUPERJUMP_SET3"))
            WAIT_MSEC(300)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_LUIGI_SUPERJUMP1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_V_LUIGI_SJUMP1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_V_LUIGI_SJUMP1"))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
            SET(LW(10), LW(4))
            RUN_CHILD_EVT(mod::techChild2)
            USER_FUNC(spm::evt_mario::evt_mario_calc_rpg_damage, LW(4), 4, LW(10))
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_018"), LW(3), LW(10))
            USER_FUNC(spm::evt_mario::evt_rpg_char_death_check, LW(4), LW(10), 0, LW(0))
            IF_FLAG(LW(0), 0x8000)
                USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_ENEMY_DIE1"))
                USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_007"), LW(3))
            ELSE()
                IF_FLAG(LW(0), 0x1)
                    USER_FUNC(spm::evt_mario::evt_rpg_status_remove, 0, LW(4), 1)
                    USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_115"), LW(3))
                END_IF()
            END_IF()
        END_IF()
END_SWITCH()
EVT_END()

EVT_BEGIN(switchChars)
USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_CHANGE_LINE_DRAW1"))
USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_CHANGE_LINE_DRAW1"))
USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_CHANGE_LINE_DRAW1"))
USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_P_CHANGE_LINE_DRAW1"))
SWITCH(LW(2))
    CASE_EQUAL(0)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_V_DESIDE_MARIO1"))
    CASE_EQUAL(2)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_V_DESIDE_KOOPA1"))
    CASE_EQUAL(3)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_V_DESIDE_LUIGI1"))
END_SWITCH()
SWITCH(LW(2))
    CASE_EQUAL(0)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_087"), LW(10))
    CASE_EQUAL(2)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_088"), LW(10))
    CASE_EQUAL(3)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_089"), LW(10))
END_SWITCH()
USER_FUNC(spm::evt_mario::evt_mario_set_character, LW(2))
EVT_END()


EVT_BEGIN(checkWinOrContinue)
    USER_FUNC(spm::evt_mario::evt_rpg_wakeup_check, 0, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_mario::evt_rpg_underchomp_name_grab, 0, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, LW(0), LW(1))
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_wakeup_check, 1, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_mario::evt_rpg_underchomp_name_grab, 1, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, LW(0), LW(1))
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_wakeup_check, 2, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_mario::evt_rpg_underchomp_name_grab, 2, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, LW(0), LW(1))
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 0, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_125"), LW(1))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_SLIT_RETURN1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_F_SLIT_RETURN1"))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 1)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 1, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_126"), LW(1))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_MARIO_JUMP1"), PTR("SFX_P_PEACH_JUMP1"), PTR("SFX_P_KOOPA_JUMP1"), PTR("SFX_P_LUIGI_JUMP1"))
        WAIT_MSEC(500)
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 2)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 2, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_127"), LW(1))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_MINI_KAKUDAI1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_F_MINI_KAKUDAI1"))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 4)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 3, LW(0))
    IF_EQUAL(LW(0), 1)
        RUN_CHILD_EVT(mod::takeDamageSFX)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_119"), LW(1), 1)
        USER_FUNC(spm::evt_mario::evt_rpg_mario_take_damage, 1, 0, LW(1))
        RUN_CHILD_EVT(mod::rpgDeathHandler) //80df1a00
    ELSE()
        IF_EQUAL(LW(0), 2)
            USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_120"), LW(1))
            USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 8)
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 4, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_124"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 16)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 12, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_124_03"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 4096)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 14, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_124_04"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 16384)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 13, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_124_05"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 8192)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 11, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_117"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 2048)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 5, LW(0))
    IF_EQUAL(LW(0), 1)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_117"), LW(1))
        USER_FUNC(spm::evt_pouch::evt_pouch_add_hp, 1)
    ELSE()
        IF_EQUAL(LW(0), 2)
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_118"))
            USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 32)
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 6, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_123"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 64)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 7, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_122"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 128)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 8, LW(0))
    IF_EQUAL(LW(0), 2)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_121"), LW(1))
        USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 256)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 9, LW(0))
    IF_EQUAL(LW(0), 1)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_124_01"), LW(1))
    ELSE()
        IF_EQUAL(LW(0), 2)
            USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_124_02"), LW(1))
            USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 512)
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effects_handle, 10, LW(0))
    IF_EQUAL(LW(0), 1)
        USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_112"), LW(1))
    ELSE()
        IF_EQUAL(LW(0), 2)
            USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(1))
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_115"), LW(1))
            USER_FUNC(spm::evt_mario::evt_rpg_menu_effects_handler, 0, 1024)
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_enemy_death_check, 0, LW(0))
    IF_NOT_FLAG(LW(0), 0x8000)
        RETURN()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_enemy_death_check, 1, LW(0))
    IF_NOT_FLAG(LW(0), 0x8000)
        RETURN()
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_enemy_death_check, 2, LW(0))
    IF_NOT_FLAG(LW(0), 0x8000)
        RETURN()
    END_IF()
    INLINE_EVT()
        USER_FUNC(spm::evt_snd::evt_snd_bgmoff, 0)
        USER_FUNC(spm::evt_snd::evt_snd_bgmon, 2, PTR("BGM_EVT_STG7_RPG_FF1"))
        WAIT_MSEC(2000)
        USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_STG7"), 1000)
        USER_FUNC(spm::evt_snd::evt_snd_envon_f, 0, PTR("ENV_AN2_08"), 1000)
    END_INLINE()
    USER_FUNC(spm::evt_mario::evt_rpg_char_get, LW(10))
    USER_FUNC(spm::evt_mario::evt_rpg_point_calculation, 0, LW(11))
    USER_FUNC(spm::evt_mario::evt_rpg_point_calculation, 1, LW(12))
    USER_FUNC(spm::evt_mario::evt_rpg_point_calculation, 2, LW(13)) //fills the lws with points gained from the turn based combat
    USER_FUNC(spm::evt_mario::evt_rpg_point_handling, LW(14))
    ADD(LW(11), LW(12))
    ADD(LW(11), LW(13))
    ADD(LW(11), LW(14)) //and adds them to a total here
    USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_008"), LW(10), LW(11))
    USER_FUNC(spm::evt_pouch::evt_pouch_add_xp, LW(11))
    USER_FUNC(spm::evt_pouch::evt_pouch_get_xp, LW(0))
    USER_FUNC(spm::evt_pouch::evt_pouch_get_next_level_xp, LW(1))
    IF_LARGE_EQUAL(LW(0), LW(1))
        USER_FUNC(spm::evt_pouch::evt_pouch_get_level, LW(0))
        ADD(LW(0), 1)
        USER_FUNC(spm::evt_pouch::evt_pouch_set_level, LW(0))
        USER_FUNC(spm::evt_pouch::evt_pouch_get_level, LW(0))
        MOD(LW(0), 2)
        IF_EQUAL(LW(0), 0)
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_009"))
            WAIT_MSEC(500)
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_009_02"), 5)
            USER_FUNC(spm::evt_pouch::evt_pouch_get_max_hp, LW(0))
            ADD(LW(0), 5)
            USER_FUNC(spm::evt_pouch::evt_pouch_set_max_hp, LW(0))
        ELSE()
            USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_009"))
            WAIT_MSEC(500)
            USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_009_01"), 1)
            USER_FUNC(spm::evt_pouch::evt_pouch_get_attack, LW(0))
            ADD(LW(0), 1)
            USER_FUNC(spm::evt_pouch::evt_pouch_set_attack, LW(0))
        END_IF()
        USER_FUNC(spm::evt_pouch::evt_pouch_get_max_hp, LW(0))
        USER_FUNC(spm::evt_pouch::evt_pouch_set_hp, LW(0))
    END_IF()
    SET(LF(0), 1)
    SET(LW(0), 1)
    RETURN()
EVT_END()


EVT_BEGIN(beginRPG) //80df2e90
USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_STG7_RPG1"))
USER_FUNC(spm::evt_snd::evt_snd_string_call, 0)
USER_FUNC(spm::evt_paper::evt_paper_entry, PTR("OFF_house_02"))
USER_FUNC(spm::evt_img::evt_img_battle_start, PTR("img"))
USER_FUNC(spm::evt_img::evt_img_pose_handle, PTR("img"), PTR("OFF_house_02"))
USER_FUNC(spm::evt_img::evt_img_pose_handle2, PTR("img"), PTR("A_2"))
USER_FUNC(spm::evt_img::evt_img_properties_set, PTR("img"), 0, 0, 1, 0, 0, 608, 480)
USER_FUNC(spm::evt_img::evt_img_battle_state_set, PTR("img"), 1)
USER_FUNC(spm::evt_img::evt_img_property_check, PTR("img"))
USER_FUNC(spm::evt_fade::evt_fade_entry, 2, 0, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_rpg_npctribe_handle)
END_INLINE()
USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
USER_FUNC(spm::evt_fade::evt_fade_entry, 65554, 0, 0, 0, 0, 255)
USER_FUNC(spm::evt_img::evt_img_battle_state_set, PTR("img"), 0)
USER_FUNC(spm::evt_img::evt_img_battle_setup, PTR("img"))
USER_FUNC(spm::evt_paper::evt_paper_delete, PTR("OFF_house_02"))
USER_FUNC(spm::evt_map::evt_mapdisp_onoff, 0)
USER_FUNC(spm::evt_fade::evt_fade_entry, 1, 300, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_001"))
WAIT_MSEC(2000)
DO(0)
    USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
    IF_FLAG(LW(0), 0x1000)
        USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, FLOAT(0.5))
    END_IF()
    USER_FUNC(spm::evt_mario::evt_rpg_effect_check, LW(0))
    IF_NOT_FLAG(LW(0), 0x600)
        USER_FUNC(spm::evt_mario::evt_rpg_choice_handler, LW(0), LW(1), LW(2), LW(3), LW(4))
        SWITCH(LW(0))
            CASE_EQUAL(0)
                RUN_CHILD_EVT(mod::attack) //seems to handle the attack input
            CASE_EQUAL(1)
                RUN_CHILD_EVT(mod::technique) //handles the technique input, none avaliable for peach
            CASE_EQUAL(2)
                RUN_CHILD_EVT(spm::iValues::usePixls) //handles pixls
            CASE_EQUAL(3)
                RUN_CHILD_EVT(spm::iValues::useItems) //handles items
            CASE_EQUAL(4)
                RUN_CHILD_EVT(mod::switchChars) //handles switching characters
            CASE_EQUAL(5)
                SET(LF(0), 0)
                RUN_CHILD_EVT(spm::iValues::runningAway) //handles run away
                IF_EQUAL(LF(0), 1)
                    DO_BREAK()
                END_IF()
                END_SWITCH()
END_IF()
USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, FLOAT(1.0))
RUN_CHILD_EVT(spm::iValues::underchompAttack1)
SET(LF(0), 0)
RUN_CHILD_EVT(mod::checkWinOrContinue)
IF_EQUAL(LF(0), 1)
   DO_BREAK()
END_IF()
USER_FUNC(spm::evt_msg::evt_msg_continue)
USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
WHILE()
USER_FUNC(spm::evt_fade::evt_fade_entry, 2, 500, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
USER_FUNC(spm::evt_msg::evt_msg_continue)
USER_FUNC(spm::evt_npc::evt_npc_stats_set_up, 324, 1)
USER_FUNC(spm::evt_npc::evt_npc_stats_set_up, 325, 1)
USER_FUNC(spm::evt_npc::evt_npc_stats_set_up, 326, 1)
USER_FUNC(spm::evt_fade::evt_fade_callfade, 1)
USER_FUNC(spm::evt_mario::changeRandomDAT1)
USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 0, 11)
USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
USER_FUNC(spm::evt_map::evt_mapdisp_onoff, 1)
USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, FLOAT(1.0))
USER_FUNC(spm::evt_fade::evt_fade_entry, 1, 500, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
IF_EQUAL(LW(0), 0)
USER_FUNC(spm::evt_snd::evt_snd_bgmoff, 0)
USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, PTR("BGM_MAP_STG7"), 1000)
USER_FUNC(spm::evt_snd::evt_snd_envon_f, 0, PTR("ENV_AN2_08"), 1000)
SET(LW(0), 0)
RETURN()
END_IF()
IF_EQUAL(LW(0), 1)
SET(LW(0), 1)
RETURN()
END_IF()
EVT_END()

EVT_BEGIN(parentOfBeginRPG)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
    INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_env_blur_on, 0, 500)
    END_INLINE()
    USER_FUNC(spm::evt_fade::evt_fade_entry, 24, 500, 0, 0, 0, 255)
    USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
    RUN_CHILD_EVT(mod::beginRPG)
    IF_EQUAL(LW(0), 0) //only happens if you run away successfully
    SET(GSW(0), 324)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_map::evt_mapobj_get_position, PTR("kuti_1"), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_mario::evt_mario_face_coords, LW(0), LW(2))
    USER_FUNC(spm::evt_map::evt_mapobj_get_position, PTR("kuti_1"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_msg::evt_msg_toge, 2, LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg7_2_133_2_128"), PTR(spm::evt_msg::evt_msg_print_kao), 0)
  ELSE()//only happens if you win the RPG battle
  SET(GSW(0), 325)
  USER_FUNC(spm::evt_pouch::evt_pouch_increment_enemies_defeated)
  WAIT_MSEC(500)
  USER_FUNC(spm::evt_map::evt_mapobj_get_position, PTR("kuti_1"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_msg::evt_msg_toge, 2, LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg7_2_134"), PTR(spm::evt_msg::evt_msg_print_kao), 0)
  USER_FUNC(spm::evt_map::evt_map_playanim, PTR("anm_kao_4"), 0, 0)
  USER_FUNC(spm::evt_map::evt_map_checkanim, PTR("anm_kao_4"), LW(0), LW(1))
  WAIT_MSEC(LW(1))
  USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("anm_kao_1"), 1)
  USER_FUNC(spm::evt_door::evt_door_enable_disable_map_door_desc, 1, PTR("doa2_l"))
  USER_FUNC(spm::evt_door::evt_door_set_event, PTR("doa2_l"), 0, PTR(spm::iValues::finalRpgChild)) //final param is a short event
  USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 500, 11)
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  USER_FUNC(spm::evt_mario::evt_unknown_pointer_change, 0)
  USER_FUNC(spm::evt_case::evt_del_case_evt, 1, GW(5))
  USER_FUNC(spm::evt_case::evt_exit_case_evt)
  RETURN()
  END_IF()
  USER_FUNC(spm::evt_map::evt_map_playanim, PTR("anm_kao_4"), 0, 0)
  USER_FUNC(spm::evt_map::evt_map_checkanim, PTR("anm_kao_4"), LW(0), LW(1))
  WAIT_MSEC(LW(1))
  USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("anm_kao_1"), 1)
  USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 500, 11)
  USER_FUNC(spm::evt_mario::evt_unknown_pointer_change, 0)
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  RETURN()
  EVT_END()

}
