#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <spm/evtmgr.h>
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
#include <spm/rel/an.h>
#include <spm/rel/sp4_13.h>
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
EVT_DECLARE_USER_FUNC(getRpgNpc, 0)
//EVT_DECLARE_USER_FUNC(osReportLWZero, 0)

EVT_BEGIN(brobot_appear)
    USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 2000)
    USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435579, LW(10))
    IF_EQUAL(LW(10), 0)
        USER_FUNC(spm::evt_mario::evt_mario_set_pos, 0, 0, 0)
        USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 123, 137, 100, 0, 0, LW(10), EVT_NULLPTR)
        USER_FUNC(80107c38, LW(10), 0)
        USER_FUNC(spm::evt_mario::evt_mario_set_anim_change_handler, PTR(&spm::sp4_13::func_80c5c304))
        USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    END_IF()
    USER_FUNC(spm::evt_npc::func_80104694, LW(10), 0)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
    USER_FUNC(spm::evt_seq::evt_seq_wait, 2)
    USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
    USER_FUNC(spm::evt_mario::evt_mario_fairy_reset)
    USER_FUNC(spm::evt_cam::evt_cam_look_at_door, 1, 0)
    USER_FUNC(800d4a54, PTR("sp4_17"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 30, 1)
    USER_FUNC(801049ec, LW(10))
    USER_FUNC(80104a3c, LW(10))
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
    USER_FUNC(801037a4, LW(10), 1, 256)
    USER_FUNC(801072a4, LW(10))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    USER_FUNC(80105924, LW(10), LW(3))
    USER_FUNC(spm::evt_npc::func_801059d0, PTR("ルイージ"), LW(3)) //80ccaf5c
    USER_FUNC(801093e0, PTR("ルイージ"))
    USER_FUNC(spm::evt_npc::func_80108194, PTR("ルイージ"), 0)
    IF_EQUAL(LW(3), -1)
        USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(80d9e8a0))
    ELSE()
        USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(80d9e8c0))
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("ルイージ"), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), 0, -100, 0)
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 1073741824)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 30, 1)
    WAIT_MSEC(100)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(0), LW(1), LW(2))
    USER_FUNC(80102574, PTR("ルイージ"), LW(0), 0, LW(2), 400, 0, FLOAT(50.0), 11, 0, 0)
    IF_SMALL(LW(0), -200)
        ADD(LW(0), 50)
        USER_FUNC(80102574, PTR("ルイージ"), LW(0), 0, LW(2), 400, 0, FLOAT(50.0), 11, 0, 0)
        ADD(LW(0), 50)
        USER_FUNC(80102574, PTR("ルイージ"), LW(0), 0, LW(2), 400, 0, FLOAT(50.0), 11, 0, 0)
    END_IF()
    INLINE_EVT()
        USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(0), LW(1), LW(2))
        ADD(LW(0), -100)
        USER_FUNC(800f046c, LW(0), LW(2), FLOAT(80.0))
        USER_FUNC(800effb8, PTR("ルイージ"))
    END_INLINE()
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR(80ccb1fc), PTR("ルイージ"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR(80ccb214), PTR("ルイージ"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 31, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR("ルイージ"), 1)
    WAIT_MSEC(2000)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 32, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR("ルイージ"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(&spm::sp4_13::mrLTribeAnimDefs))
    USER_FUNC(spm::evt_npc::func_801059d0, PTR("ルイージ"), -1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 0, 1)
    USER_FUNC(801093e0, PTR("ルイージ"))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(0), LW(1), LW(2))
    USER_FUNC(800efa4c, LW(3), LW(4), LW(5))
    ADD(LW(0), LW(3))
    DIV(LW(0), 2)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), 55, 334, LW(0), 55, -16, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR(80ccb228), 0, PTR("ルイージ"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), 55, 334, LW(0), 55, -16, 500, 11)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_V_DLUIGI_HEY1"), PTR("ルイージ"))
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(80d9e830))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 26, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR("ルイージ"), 1)
    WAIT_MSEC(300)
    USER_FUNC(800fbc64, 0, PTR("stg4_4_018_1"))
    USER_FUNC(8010c600, PTR(80ccb244))
    USER_FUNC(8010be80, 138)
    USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 124, 138, 0, 1000, 0, 0, EVT_NULLPTR)
    USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435580, LW(0))
    USER_FUNC(80107c38, LW(0), 0)
    USER_FUNC(80103a44, LW(0), 5, 1, 128)
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 1, PTR(80ccb258))
    INLINE_EVT_ID(LW(11))
        USER_FUNC(spm::evt_cam::evt_cam_shake, 5, FLOAT(1.19921875), FLOAT(1.19921875), FLOAT(0.0), 0, 0)
    END_INLINE()
    USER_FUNC(800d2800, PTR(80ccb270))
    USER_FUNC(800d3030, LW(10))
    USER_FUNC(800ec2c8, PTR(80ccb288))
    USER_FUNC(800ec3ac, PTR(80ccb288), PTR(80ccb244))
    USER_FUNC(800ec458, PTR(80ccb288), PTR(80ccad20))
    USER_FUNC(800ec4ec, PTR(80ccb288), 0, 0, 0, 0, 0, 608, 480)
    USER_FUNC(800ec7e0, PTR(80ccb288), 1)
    WAIT_MSEC(2000)
    INLINE_EVT()
        WAIT_MSEC(300)
        USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435580, LW(0))
        USER_FUNC(spm::evt_npc::evt_npc_set_camid, LW(0), 10)
        USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(10), LW(11), LW(12))
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(0), LW(10), 20, -100)
        USER_FUNC(800fe82c, LW(0), 1, 0, 90, 0)
        USER_FUNC(800d2800, PTR(80ccb28c))
        USER_FUNC(80102574, LW(0), LW(10), 200, 100, 500, FLOAT(0.0), FLOAT(0.0), 11, 0, 0)
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(0), 0, 1000, 0)
        USER_FUNC(800fe82c, LW(0), 1, 0, 0, 0)
        USER_FUNC(spm::evt_npc::evt_npc_set_camid, LW(0), 5)
    END_INLINE()
    USER_FUNC(800d2800, PTR(80ccb2a0))
    USER_FUNC(800ec458, PTR(80ccb288), PTR(80ccb2b8))
    USER_FUNC(800ec920, PTR(80ccb288))
    WAIT_MSEC(2000)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 0, 250, 644, 0, 250, -16, 0, 11)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("ルイージ"), 100, 0, 0)
    USER_FUNC(spm::evt_npc::func_801059d0, PTR("ルイージ"), -1)
    USER_FUNC(spm::evt_mario::evt_mario_set_pos, -125, 0, 0)
    USER_FUNC(800effb8, PTR("ルイージ"))
    USER_FUNC(800ec7e0, PTR(80ccb288), 0)
    USER_FUNC(800ec8c4, PTR(80ccb288))
    USER_FUNC(8010c660, PTR(80ccb244))
    WAIT_MSEC(300)
    DELETE_EVT(LW(11))
    USER_FUNC(spm::evt_cam::evt_cam_shake, 5, FLOAT(0.0), FLOAT(0.0), FLOAT(0.0), 200, 0)
    USER_FUNC(800d2f74, LW(10))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc_delay, PTR(80ccb0b8), PTR("ルイージ"), 2000)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 0, 100, 644, 0, 100, -16, 3000, 11)
    USER_FUNC(800d2800, PTR(80ccb2bc))
    USER_FUNC(800d37d8, 11, 40000, 0, 3000)
    DO(0)
        USER_FUNC(800d3878)
        DIVF(LW(0), FLOAT(100.0))
        USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435580, LW(2))
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(2), 0, LW(0), 0)
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR(80ccb2dc), 0, PTR("ルイージ"))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, -10, 165, 434, -10, 165, -16, 500, 11)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR(80ccb2e8), 0, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(&spm::sp4_13::mrLTribeAnimDefs))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 25, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("ルイージ"), 0, 150, 0, FLOAT(100.0), 500)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR(80ccb2fc), 0, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("ルイージ"), 0, -1000, 0)
    USER_FUNC(80102f5c, PTR(80ccb310), PTR(80ccb318), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR(80ccb310), 0, -1000, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR(80ccb310), 14, PTR(80d9e8e0))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(80ccb310), 34, 1)
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR(80ccb310), 1, 71434244)
    USER_FUNC(801039b8, PTR(80ccb310), 1, 32)
    USER_FUNC(80103054, PTR(80ccb310))
    USER_FUNC(spm::evt_npc::func_80104694, PTR(80ccb310), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR(80ccb310), 10, 50)
    USER_FUNC(spm::evt_npc::func_80108194, PTR(80ccb310), 0)
    USER_FUNC(spm::evt_npc::evt_npc_id_to_name, 268435580, LW(10))
    USER_FUNC(800fe338, LW(10), 5, LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR(80ccb310), LW(0), LW(1), LW(2))
    SET(LW(3), LW(1))
    ADD(LW(3), -100)
    MUL(LW(1), 100)
    MUL(LW(3), 100)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR(80ccb310), LW(7), LW(8), LW(9))
    USER_FUNC(800d37d8, 11, LW(3), LW(1), 1000)
    DO(0)
        USER_FUNC(800d3878)
        DIVF(LW(0), FLOAT(100.0))
        USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR(80ccb310), LW(7), LW(0), LW(9))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    WAIT_MSEC(400)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(80ccb310), 33, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR(80ccb310), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(80ccb310), 0, 1)
    USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 1, 3000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_020"), 0, PTR(80ccb310))
    USER_FUNC(800e720c, 30, 29)
    USER_FUNC(8010d0f0, PTR("sp4_17"), 0)
    RETURN()
EVT_END()

EVT_BEGIN(mr_l_appear)
    USER_FUNC(spm::evt_cam::evt_cam_shake, 5, FLOAT(0.0), FLOAT(1.5), FLOAT(0.0), 400, 0)
    END_INLINE()
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d_player, PTR("SFX_P_V_MARIO_BIKKURI1"), PTR("SFX_P_V_PEACH_BIKKURI1"), PTR("SFX_P_V_KOOPA_BIKKURI1"), 0)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("E_3"), 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_TAMARA_DAMAGE1"), PTR("ルイージ"))
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("タマラ"), 14, PTR(80d9e778))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("タマラ"), 0, 1)
    ADD(LW(0), 100)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_P_LUIGI_JUMP1"), PTR("ルイージ"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("ルイージ"), LW(0), 0, LW(2), FLOAT(80.0), 500)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 0, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_P_LUIGI_LAND1"), PTR("ルイージ"))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_P_LUIGI_LAND1"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_007"), 0, PTR("タマラ"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("BGM_EVT_DLUIGI_APPEAR1"), 0, PTR("__mario__"))
            WAIT_MSEC(300)
        CASE_EQUAL(2)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_009"), 0, PTR("__mario__"))
            WAIT_MSEC(300)
    END_SWITCH()
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 1, PTR("BGM_EVT_DLUIGI_APPEAR1"))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 100, 38, 234, 100, 38, -16, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc_delay, PTR("SFX_EVT_V_DLUIGI_LAUGH1"), PTR("ルイージ"), 750)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_010"), 0, PTR("ルイージ"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_8"), 0)
    USER_FUNC(800f19c0)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_5"), 0)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 0, 85, 584, 0, 85, -16, 0, 11)
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_011"), 0, PTR("__guide__"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(&spm::sp4_13::mrLTribeAnimDefs))
    USER_FUNC(spm::evt_npc::func_801059d0, PTR("ルイージ"), -1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 0, 1)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 100, 85, 584, 100, 85, -16, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_012"), 0, PTR("ルイージ"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_V_DLUIGI_HEY1"), PTR("ルイージ"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_LUIGI_PAUSE1"), PTR("ルイージ"))
    USER_FUNC(spm::evt_env::evt_env_blur_on, 0, 0)
    DO(3)
        USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 100, 85, 584, 100, 85, -16, 0, 0)
        WAIT_FRM(1)
        USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 100, 37, 234, 100, 37, -16, 250, 0)
        WAIT_MSEC(250)
    WHILE()
    USER_FUNC(spm::evt_env::evt_env_static_blur_on)
    WAIT_MSEC(300)
    INLINE_EVT()
        WAIT_MSEC(1500)
        USER_FUNC(spm::sp4_13::func_80c5c36c)
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(&spm::sp4_13::theGreenThunderTribeAnimDefs))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 35, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR("ルイージ"), 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_012_1"), 0, PTR("ルイージ"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_eff::evt_eff_softdelete, PTR("eff"))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, -35, 60, 384, -35, 60, -16, 0, 11)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_013"), 0, PTR("__guide__"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_mario::evt_mario_set_anim_change_handler, PTR(&spm::sp4_13::returnCharAnimPos))
    USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_013_1"), 0, PTR("__mario__"))
            WAIT_MSEC(300)
        CASE_EQUAL(2)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_013_2"), 0, PTR("__mario__"))
            WAIT_MSEC(300)
    END_SWITCH()
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(&spm::sp4_13::mrLTribeAnimDefs))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 36, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR("ルイージ"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 0, 1)
    USER_FUNC(spm::evt_npc::func_80108194, PTR("ルイージ"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("タマラ"), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("ルイージ"), LW(0), LW(2), 0, FLOAT(120.0), 0, 2, 0)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_hit"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_EVT_TAMARA_KICK1"), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_EVT_TAMARA_FLY1"), LW(0), LW(1), LW(2))
    ADD(LW(1), -100)
    ADD(LW(2), 100)
    USER_FUNC(spm::evt_npc::evt_npc_set_camid, PTR("タマラ"), 9)
    INLINE_EVT_ID(LW(10))
        SET(LW(0), 0)
        DO(0)
            ADD(LW(0), 20)
            MOD(LW(0), 360)
            USER_FUNC(spm::evt_npc::evt_npc_set_camid, PTR("タマラ"), 0, 10, 0)
            USER_FUNC(spm::evt_npc::evt_npc_rotate, PTR("タマラ"), 0, 0, LW(0))
            WAIT_FRM(1)
        WHILE()
    END_INLINE()
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("タマラ"), LW(0), LW(1), LW(2), FLOAT(100.0), 600)
    DELETE_EVT(LW(10))
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("タマラ"), 0, -1000, 0)
    USER_FUNC(spm::evt_npc::evt_npc_rotate, PTR("タマラ"), 0, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_camid, PTR("タマラ"), 5)
    USER_FUNC(spm::evt_npc::func_80108194, PTR("ルイージ"), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_014"), 0, PTR("ルイージ"))
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, -25, 45, 184, -25, 45, -16, 500, 11)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("ルイージ"), 14, PTR(&spm::sp4_13::mrLTauntTribeAnimDefs))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("ルイージ"), 26, 1)
    USER_FUNC(spm::evt_npc::func_80103410, PTR("ルイージ"), 1)
    USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_015"), 0, PTR("ルイージ"))
            WAIT_MSEC(300)
        CASE_EQUAL(1)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_016"), 0, PTR("ルイージ"))
            WAIT_MSEC(300)
        CASE_EQUAL(2)
            USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg4_4_017"), 0, PTR("ルイージ"))
            WAIT_MSEC(300)
    END_SWITCH()
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("ルイージ"), LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 123, 137, LW(0), LW(1), LW(2), LW(3), EVT_NULLPTR)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(3), 8, spm::sp4_13::brobot_appear)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("ルイージ"), 0, -1000, 0)
    SET(GSW(0), 170)
    INLINE_EVT()
        USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 1, 1000)
        WAIT_MSEC(1000)
        USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_MIDDLE1"))
    END_INLINE()
    USER_FUNC(spm::evt_mario::evt_mario_set_anim_change_handler, PTR(&spm::sp4_13::func_80c5c304))
    USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
    IF_EQUAL(LW(0), 1)
        USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1B"), 0)
    ELSE()
        USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    RETURN()
EVT_END()

}