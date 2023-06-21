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
    CASE_EQUAL(2)
        USER_FUNC(spm::evt_msg::evt_msg_print_add, 0, PTR("stg7_2_133_2_014"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_KOOPA_FIRE1"))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_KOOPA_FLAME1"))
        USER_FUNC(spm::evt_snd::evt_snd_get_last_sfx_id, LW(0))
        WAIT_MSEC(600)
        USER_FUNC(spm::evt_snd::func_800d2fa4, LW(0), 1000)
        SET(LW(10), LW(4))
        RUN_CHILD_EVT(spm::iValues::techChild1)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_015"), LW(3))
    CASE_EQUAL(3)
        USER_FUNC(spm::evt_msg::evt_msg_print_add_insert, 0, PTR("stg7_2_133_2_017"), LW(3))
        USER_FUNC(spm::evt_sub::evt_sub_random, 1000, LW(10))
        IF_SMALL(LW(10), 150)
            SET(LW(10), LW(4))
            RUN_CHILD_EVT(spm::iValues::techChild1)
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
            RUN_CHILD_EVT(spm::iValues::techChild2)
            USER_FUNC(spm::evt_mario::evt_mario_calc_damage_to_enemy, LW(4), 4, LW(10))
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
    USER_FUNC(spm::evt_mario::evt_flower_check, LW(0))
    IF_FLAG(LW(0), 0x1000)
        USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, FLOAT(0.5))
    END_IF()
    USER_FUNC(spm::evt_mario::evt_flower_check, LW(0))
    IF_NOT_FLAG(LW(0), 0x600)
        USER_FUNC(spm::evt_mario::evt_rpg_choice_handler, LW(0), LW(1), LW(2), LW(3), LW(4))
        SWITCH(LW(0))
            CASE_EQUAL(0)
                RUN_CHILD_EVT(spm::iValues::attacking) //seems to handle the attack input
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
RUN_CHILD_EVT(spm::iValues::underchompAttack2)
RUN_CHILD_EVT(spm::iValues::underchompAttack3)
SET(LF(0), 0)
RUN_CHILD_EVT(spm::iValues::checkWinOrContinue)
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
USER_FUNC(spm::evt_npc::evt_env_blur_on, 0, 500)
END_INLINE()
USER_FUNC(spm::evt_fade::evt_fade_entry, 24, 500, 0, 0, 0, 255)
USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 3420, 1040, 234, 3420, 1040, -16, 500, 11)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435579, LW(0))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(0), 1, 1073741824)
USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435580, LW(0))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(0), 1, 1073741824)
USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435581, LW(0))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(0), 1, 1073741824)
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
EVT_END()

}
