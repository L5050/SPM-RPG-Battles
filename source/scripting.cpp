#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <spm/iValues.h>
#include <spm/evtmgr.h>
#include <spm/evt_msg.h>
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
//pointers in eu0, I need to port them to all versions

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
DO()
    USER_FUNC(spm::evt_mario::evt_flower_check, LW(0))
    IF_FLAG(LW(0), 0x1000)
        USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, 0.5)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_flower_check, LW(0))
    IF_NOT_FLAG(LW(0), 0x600)
        USER_FUNC(spm::evt_mario::evt_rpg_choice_handler, LW(0), LW(1), LW(2), LW(3), LW(4))
        SWITCH(LW(0))
            CASE_EQUAL(0)
                RUN_CHILD_EVT(0x80def500) //seems to handle the attack input
            CASE_EQUAL(1)
                RUN_CHILD_EVT(0x80def738) //handles the technique input, none avaliable for peach
            CASE_EQUAL(2)
                RUN_CHILD_EVT(0x80def9d8) //handles pixls
            CASE_EQUAL(3)
                RUN_CHILD_EVT(0x80df0500)
            CASE_EQUAL(4)
                RUN_CHILD_EVT(0x80df1860)
            CASE_EQUAL(5)
                SET(LF(0), 0)
                RUN_CHILD_EVT(0x80df1958)
                IF_EQUAL(LF(0), 1)
                    DO_BREAK()
                END_IF()
                END_SWITCH()
END_IF()
USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, 1.0)
RUN_CHILD_EVT(0x80df1b70)
RUN_CHILD_EVT(0x80df1eb0)
RUN_CHILD_EVT(0x80df21f0)
SET(LF(0), 0)
RUN_CHILD_EVT(0x80df2530)
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
USER_FUNC(spm::evt_sub::evt_sub_gameSpeedChange, 1.0)
USER_FUNC(spm::evt_fade::evt_fade_entry, 1, 500, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
IF_EQUAL(LW(0), 0)
USER_FUNC(spm::evt_snd::evt_snd_bgmoff, 0)
USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, PTR("BGM_MAP_STG7"), 1000)
USER_FUNC(spm::evt_snd::evt_snd_envon_f, 0, PTR("ENV_AN2_08"), 1000)
SET(LW(0), 0)
END_EVT()
END_IF()
IF_EQUAL(LW(0), 1)
SET(LW(0), 1)
END_EVT()
END_IF()
END_EVT()
END_SCRIPT()

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
RUN_CHILD_EVT(beginRPG)
IF_EQUAL(LW(0), 0) //only happens if you run away successfully
SET(GSW(0), 324)
WAIT_MSEC(500)
USER_FUNC(spm::evt_map::evt_mapobj_get_position, PTR("kuti_1"), LW(0), LW(1), LW(2))
USER_FUNC(spm::evt_mario::evt_mario_face_coords, LW(0), LW(2))
USER_FUNC(spm::evt_map::evt_mapobj_get_position, PTR("kuti_1"), LW(0), LW(1), LW(2))
USER_FUNC(spm::evt_msg::evt_msg_toge, 2, LW(0), LW(1), LW(2))
USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg7_2_133_2_128"), spm::evt_msg::evt_msg_print_kao, 0)
ELSE()//only happens if you win the RPG battle
SET(GSW(0), 325)
USER_FUNC(spm::evt_pouch::evt_pouch_increment_enemies_defeated)
WAIT_MSEC(500)
USER_FUNC(spm::evt_map::evt_mapobj_get_position, PTR("kuti_1"), LW(0), LW(1), LW(2))
USER_FUNC(spm::evt_msg::evt_msg_toge, 2, LW(0), LW(1), LW(2))
USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg7_2_134"), spm::evt_msg::evt_msg_print_kao, 0)
USER_FUNC(spm::evt_map::evt_map_playanim, PTR("anm_kao_4"), 0, 0)
USER_FUNC(spm::evt_map::evt_map_checkanim, PTR("anm_kao_4"), LW(0), LW(1))
WAIT_MSEC(LW(1))
USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("anm_kao_1"), 1)
USER_FUNC(spm::evt_door::evt_door_enable_disable_map_door_desc, 1, PTR("doa2_l"))
USER_FUNC(spm::evt_door::evt_door_set_event, PTR("doa2_l"), 0, spm::iValues::finalRpgChild) //final param is a short event
USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 500, 11)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
USER_FUNC(spm::evt_mario::evt_unknown_pointer_change, 0)
USER_FUNC(spm::evt_case::evt_del_case_evt, 1, GW(5))
USER_FUNC(spm::evt_case::evt_exit_case_evt)
END_EVT()
END_IF()
USER_FUNC(spm::evt_map::evt_map_playanim, PTR("anm_kao_4"), 0, 0)
USER_FUNC(spm::evt_map::evt_map_checkanim, PTR("anm_kao_4"), LW(0), LW(1))
WAIT_MSEC(LW(1))
USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("anm_kao_1"), 1)
USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 500, 11)
USER_FUNC(spm::evt_mario::evt_unknown_pointer_change, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
END_EVT()
END_SCRIPT()
}
