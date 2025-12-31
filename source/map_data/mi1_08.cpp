#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "map_data/mi1_08.h"
#include "npc_rpgdrv.h"

#include <patch.h>
#include <spm/evt_mobj.h>
#include <spm/map_data.h>
#include <spm/evtmgr.h>
#include <spm/npcdrv.h>
#include <spm/evt_snd.h>
#include <spm/evt_mario.h>
#include <spm/evt_sub.h>
#include <spm/evt_env.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/seqdef.h>

using namespace spm::npcdrv;
using namespace spm::evt_npc;

namespace mod {

const char * kamek = "kamek";
const char * peach = "peach";
const char * tippi = "__guide__";
const char * pixl = "__fairy__";

NPCTribeAnimDef animsKMK[] = {
  {0, "KMK_S_1"},
  {1, "KMK_W_1"},
  {2, "KMK_R_1"},
  {3, "KMK_T_1"},
  {4, "KMK_D_1"},
  {5, "KMK_S_3"},
  {6, "KMK_A_1A"},
  {7, "KMK_A_1B"},
  {8, "KMK_A_1C"},
  {9, "KMK_A_1D"},
  {10, "KMK_A_1E"},
  {14, "KMK_N_1"},
  {-1, nullptr}
};

NPCTribeAnimDef animsPeach_KMK[] = {
  {0, "S_1B"},
  {1, "W_1"},
  {2, "R_1"},
  {3, "T_1"},
  {4, "D_4"},
  {5, "S_1D"},
  {6, "A_1"},
  {7, "E_3"},
  {10, "U_1"},
  {14, "N_1"},
  {-1, nullptr}
};

const char * tippi_wait = "<p><fairy>\n"
"Something isn't right.\n"
"There must be something\n"
"or someone hidden here.\n"
"<k>\n";

const char * kamek_1 = "<p>\n"
"Ah, Mario! Peach!<wait 250> \n"
"Fancy meeting you here!<wait 250> \n"
"King Bowser sends his regards. \n"
"<k>\n"
"<p>\n"
"In this room is a hidden door\n"
"that opens the way to King\n"
"Bowser's hiding place.\n"
"<k>\n"
"<p>\n"
"Your highness... I'm sure he\n"
"would love to see you again...\n"
"<k>\n";

const char * peach_1 = "<p>\n"
"If Bowser is alive... I wonder\n"
"if he knows where Luigi is.\n"
"<k>\n";

const char * peach_2 = "<p>\n"
"Please, take us to him!\n"
"<k>\n";

const char * kamek_2 = "<p>\n"
"Of course...\n"
"<k>\n";

const char * kamek_3 = "<p>\n"
"Very well. We can do this\n"
"the hard way.\n"
"<k>\n"
"<p>\n"
"<scale 1.4>ALL<wait 250> HAIL<wait 250> COUNT<wait 250> BLECK!\n"
"<k>\n";

const char * throw_silence = "<p><small>\n"
"... <wait 750> \n";

const char * throw_2 = "<p>\n"
"Pardon me, Princess, but this\n"
"wizard is not to be trusted.\n"
"<k>\n";

EVT_BEGIN(mi1_08_patch)
  USER_FUNC(spm::evt_mobj::evt_mobj_save_blk, PTR("save"), FLOAT(-962.5), 100, FLOAT(12.5), 0)
RETURN_FROM_CALL()

EVT_BEGIN(kamek_evt)
  USER_FUNC(spm::evt_sub::evt_sub_get_entername, LW(0))
  IF_STR_EQUAL(LW(0), PTR(kamek))
    WAIT_FRM(1)
    USER_FUNC(spm::evt_mario::evt_mario_set_character, 0)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
    USER_FUNC(spm::evt_mario::evt_mario_set_pos, -69, 0, 0)
    USER_FUNC(evt_npc_set_position, PTR(kamek), 95, 0, 0)
    USER_FUNC(evt_npc_set_position, PTR(peach), -20, 0, 0)
    USER_FUNC(evt_npc_set_disp_callback, PTR(peach), PTR(peachNpcDispCb))
    USER_FUNC(evt_npc_rotate, PTR(peach), 0, 180, 0)
    USER_FUNC(spm::evt_cam::evt_cam_get_pos, 5, LW(5), LW(6), LW(2))
    SET(LW(0), -50)
    ADD(LW(0), 100)
    SET(LW(1), 45)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), LW(6), LW(2), LW(0), LW(1), 0, 0, 11)
    WAIT_MSEC(3000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(kamek_1), 0, PTR(kamek))
    WAIT_MSEC(500)
    INLINE_EVT() 
      USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(throw_silence), 0, PTR(pixl))
    END_INLINE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(peach_1), 0, PTR(peach))
    USER_FUNC(evt_npc_set_anim, PTR(peach), 2, 1)
    USER_FUNC(evt_npc_walk_to, PTR(peach), 0, 0, 0, FLOAT(40.0), 0, 0, 0)
    WAIT_FRM(1)
    USER_FUNC(evt_npc_set_anim, PTR(peach), 0, 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(peach_2), 0, PTR(peach))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(kamek_2), 0, PTR(kamek))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_MAGIC_SING1"))
    USER_FUNC(evt_npc_set_anim, PTR(kamek), 6, 1)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_MAGIC_SING1"))
    USER_FUNC(evt_npc_set_anim, PTR(kamek), 7, 1)
    USER_FUNC(evt_npc_wait_anim_end, PTR(kamek), 0)
    USER_FUNC(evt_npc_set_anim, PTR(kamek), 8, 1)
    WAIT_MSEC(500)
    USER_FUNC(evt_npc_set_anim, PTR(kamek), 9, 1)
    USER_FUNC(evt_npc_wait_anim_end, PTR(kamek), 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_KAMEKU_AGERU1"))
    USER_FUNC(evt_npc_set_anim, PTR(kamek), 10, 1)
    USER_FUNC(spm::evt_fairy::evt_fairy_all_enter_run_mode1)
    USER_FUNC(spm::evt_fairy::evt_fairy_fly_to, 0, 0, 20, 0, 200)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_1"))
    USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_NAGE_FIRLING1"))
    USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_2B"))
    WAIT_MSEC(200)
    USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_3C"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_THROW1"))
    INLINE_EVT()
      USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
      USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("A_4A"))
      USER_FUNC(spm::evt_fairy::evt_fairy_wait_anim_end, 0)
      USER_FUNC(spm::evt_fairy::evt_fairy_set_anim, 0, PTR("S_1"))
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(peach), 7, 1)
    USER_FUNC(evt_npc_jump_to, PTR(peach), -69, 0, -75, 100, FLOAT(500.0))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(peach), 4, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_CRUSH1"))
    USER_FUNC(evt_npc_wait_anim_end, PTR(peach), 0)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, 0, 20, 0, FLOAT(1.2), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
    WAIT_FRM(60)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(kamek), 0, 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(throw_2), 0, PTR(pixl))
    USER_FUNC(spm::evt_fairy::evt_fairy_all_enter_run_mode2)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(kamek_3), 0, PTR(kamek))
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_KAMEK_BTL"))
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    USER_FUNC(start_boss_fight, 63)
  ELSE()
    IF_EQUAL(GSWF(1803), 1)
      RETURN()
    END_IF()
    SET(GSWF(1803), 1)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
    WAIT_MSEC(2000)
    USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 2000)
    USER_FUNC(spm::evt_mario::evt_mario_pos_change, -50, 0, FLOAT(40.0))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(tippi_wait), 0, PTR(tippi))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_cam::evt_cam_get_pos, 5, LW(5), LW(6), LW(2))
    SUB(LW(5), 70)
    ADD(LW(6), 50)
    ADD(LW(0), 100)
    SET(LW(1), 45)
    SUB(LW(2), 200)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), LW(6), LW(2), LW(0), LW(1), 0, 1500, 11)
    WAIT_MSEC(1000)
    INLINE_EVT()
      USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_KAMEK"))
      USER_FUNC(spm::evt_env::evt_env_blur_on, 0, 1000)
    END_INLINE()
    USER_FUNC(spm::evt_fade::evt_set_transition, 26, 25)
    USER_FUNC(spm::evt_seq::evt_seq_mapchange, PTR("mi1_09"), PTR(kamek))
  END_IF()
  RETURN()
EVT_END()

EVT_BEGIN(mi1_09_patch)
  USER_FUNC(evt_npc_delete, PTR("npc"))
  USER_FUNC(evt_npc_entry, PTR(kamek), PTR("e_kamek"), 0)
  USER_FUNC(evt_npc_entry, PTR(peach), PTR("p_peach"), 0)
  USER_FUNC(evt_npc_set_position, PTR(kamek), 0, -100, 0)
  USER_FUNC(evt_npc_set_position, PTR(peach), 0, -100, 0)
  USER_FUNC(evt_npc_set_property, PTR(peach), 14, (s32)animsPeach_KMK)
  USER_FUNC(evt_npc_set_property, PTR(kamek), 14, (s32)animsKMK)
  USER_FUNC(evt_npc_set_anim, PTR(kamek), 0, 1)
  USER_FUNC(evt_npc_set_anim, PTR(peach), 0, 1)
  RUN_EVT(kamek_evt)
RETURN_FROM_CALL()

EVT_BEGIN(mi_spike_damage)
  WAIT_MSEC(25000)
  USER_FUNC(getCurrentCombatStatus, LW(0))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(displayDamage, LW(0), LW(1), LW(2), 99)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, 4, 0, 0)
  END_IF()
RETURN()
EVT_END()

EVT_BEGIN(mi_spike_patch)
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  USER_FUNC(start_boss_fight, 99)
  USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_STG7_RPG1"))
  RUN_EVT(mi_spike_damage)
RETURN_FROM_CALL()

void mi1_08_main()
{
  spm::map_data::MapData * mi1_08_md = spm::map_data::mapDataPtr("mi1_08");
  spm::evtmgr::EvtScriptCode* mi1_08_init_evt = mi1_08_md->initScript;
  evtpatch::hookEvtReplace(mi1_08_init_evt, 20, (spm::evtmgr::EvtScriptCode*)mi1_08_patch);
  spm::map_data::MapData * mi1_09_md = spm::map_data::mapDataPtr("mi1_09");
  spm::evtmgr::EvtScriptCode* mi1_09_init_evt = mi1_09_md->initScript;
  evtpatch::hookEvtReplace(mi1_09_init_evt, 19, (spm::evtmgr::EvtScriptCode*)mi1_09_patch);
  spm::map_data::MapData * mi2_06_md = spm::map_data::mapDataPtr("mi2_06");
  spm::evtmgr::EvtScriptCode* mi2_06_init_evt = mi2_06_md->initScript;
  spm::evtmgr::EvtScriptCode* mi2_06_spike = getInstructionEvtArg(mi2_06_init_evt, 11, 5);
  evtpatch::hookEvtReplaceBlock(mi2_06_spike, 81, (spm::evtmgr::EvtScriptCode*)mi_spike_patch, 85);
}

}