#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "bowser.h"

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
#include <spm/evt_mobj.h>
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
#include <spm/rel/mi4.h>
#include <patch.h>
#include <string>

using namespace spm::npcdrv;
using namespace spm::evt_npc;

NPCTribeAnimDef animsBrowserFirefoxExtension[] = {
  {0, "S_1"},
  {1, "W_1"},
  {2, "R_1"},
  {3, "T_1"},
  {4, "D_1"},
  {7, "S_2"},
  {8, "A_1_a"},
  {9, "A_1_b"},
  {10, "A_1_c"},
  {11, "A_1_d"},

  {14, "N_1"},
  {20, "E_8"},
  {21, "E_9"},
  {22, "S_1G"},
  {30, "AC_7"},
  {-1, nullptr}
};

  static NPCTribeAnimDef animsCharge[] = {
    {0, "Z_1"},
    {1, "S_1"},
    {-1, nullptr}
  };

  const char * browser_dia = "<dq>Bowser used all of his\n"
  "FP to muster power into his\n"
  "fire breath!!\n"
  "<o>\n";

  const char * browser_dia_2 = "<scale 0.9>Hmmph... Lets see if you can handle...\n"
  "<dkey><wait 550></dkey><scale 2.0><shake>THIS!!\n"
  "<k>\n";

  const char * browser_dia_3 = "<dq>Bowser matched his HP, FP, and\n"
  "ATK stats with Mario's stats!\n"
  "<k>\n";

namespace mod {

  EVT_BEGIN(browser_safarshit_onspawn)
    USER_FUNC(evt_npc_set_disp_callback, LW(15), PTR(bowserNpcDispCb))
    USER_FUNC(set_pre_battle_cutscene, PTR(browser_tor_prebat))
    USER_FUNC(spm::evt_mobj::evt_mobj_set_scale, LW(14), FLOAT(3.5), FLOAT(2.3), FLOAT(3.5))
    RETURN()
  EVT_END()

  EVT_BEGIN(browser_tor_prebat)
    SET(LW(15), PTR("npc2"))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(browser_dia_2), 0, LW(15))
    USER_FUNC(evt_npc_set_anim, LW(15), 30, 1)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(15), LW(1), LW(2), LW(3))
    SUB(LW(2), 10)
    USER_FUNC(evt_npc_set_position, LW(15), LW(1), LW(2), LW(3))
    INLINE_EVT_ID(LW(10))
      SET(LW(9), FLOAT(0.0))
      DO(0)
      ADDF(LW(9), FLOAT(18.0))
      USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, LW(9), EVT_NULLPTR)
      WAIT_FRM(1)
      WHILE()
    END_INLINE()
    USER_FUNC(evt_npc_entry_autoname, PTR("EFF_batug_tue"), 0, LW(14))
    USER_FUNC(spm::evt_npc::evt_npc_set_scale, LW(14), FLOAT(3.0), FLOAT(3.0), FLOAT(3.0))
    USER_FUNC(spm::evt_npc::evt_npc_set_property, LW(14), 14, PTR(animsCharge))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
    USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(14), LW(5), LW(6), LW(7))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(14), 1, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, LW(14), 1)
    USER_FUNC(spm::evt_npc::evt_npc_delete, LW(14))
    DELETE_EVT(LW(10))
    USER_FUNC(spm::evt_npc::evt_npc_rotate, LW(15), EVT_NULLPTR, FLOAT(0.0), EVT_NULLPTR)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_TUYOTUYO1"))
    ADD(LW(6), 10)
    USER_FUNC(spm::evt_npc::evt_npc_jump_to, LW(15), LW(5), LW(6), LW(7), 100, FLOAT(200.0))
    USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfx_wait_name, PTR("SFX_I_TUYOTUYO1"))
    USER_FUNC(spm::evt_pouch::evt_pouch_get_max_hp, LW(0))
    USER_FUNC(set_npc_rpg_hp, 1, LW(0))
    USER_FUNC(spm::evt_pouch::evt_pouch_get_attack, LW(0))
    USER_FUNC(set_npc_rpg_atk, 1, LW(0))
    USER_FUNC(evt_npc_set_anim, LW(15), 20, 1)
    USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 21, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_V_KOOPA_PLEASURE1"))
    WAIT_MSEC(500)
    USER_FUNC(evt_npc_set_anim, LW(15), 22, 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(browser_dia_3), 0, 0)
    USER_FUNC(getMaxFP, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 3, LW(0))
    RETURN()
  EVT_END()
  
  EVT_BEGIN(browser_chromium_onhit)
      USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
      BROTHER_EVT_ID(LW(4))
        USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 60, 500)
      END_BROTHER()
      WAIT_MSEC(300)
      DELETE_EVT(LW(4))
      USER_FUNC(evt_npc_set_anim, LW(15), 30, 1)
      USER_FUNC(evt_npc_get_position, LW(15), LW(1), LW(2), LW(3))
      SUB(LW(2), 10)
      USER_FUNC(evt_npc_set_position, LW(15), LW(1), LW(2), LW(3))
      BROTHER_EVT_ID(LW(4))
        SET(LW(9), FLOAT(0.0))
        LBL(59)
        ADDF(LW(9), FLOAT(15.0))
        USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, LW(9), EVT_NULLPTR)
        WAIT_FRM(1)
        GOTO(59)
      END_BROTHER()
      WAIT_FRM(1)
      USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("D_3"), 0)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_MARIO_DAMAGE1"))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
      SET(LW(10), 1)
      USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
      SUB(LW(5), 50)
      ADD(LW(6), 50)
      USER_FUNC(displayDamage, LW(5), LW(6), LW(7), LW(10))
      WAIT_MSEC(1200)
      INLINE_EVT()
        USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
      END_INLINE()
      USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, FLOAT(0.0), EVT_NULLPTR)
      USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
      USER_FUNC(evt_npc_get_position, LW(15), LW(1), LW(2), LW(3))
      ADD(LW(2), 10)
      USER_FUNC(evt_npc_set_position, LW(15), LW(1), LW(2), LW(3))
      RETURN()
  EVT_END()
  
  EVT_BEGIN(browserFIREfoxingItAttack)
    USER_FUNC(rpg_set_dialogue, PTR(browser_dia))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
    ADD(LW(1), 90)
    USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
    USER_FUNC(evt_npc_walk_to, LW(15), LW(1), LW(3), FLOAT(0.0), FLOAT(100.0), 0, 0, 0)
    WAIT_FRM(1)
    USER_FUNC(evt_npc_set_anim, LW(15), 8, 1)
    USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 9, 1)
    USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 10, 1)
    USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
    SUB(LW(5), 34)
    ADD(LW(6), 18)
    SUB(LW(7), 3)
    USER_FUNC(eff_spawn_bowser_fire, LW(5), LW(6), LW(7), FLOAT(1.1), 8, 0, FLOAT(0.0), PTR("b_p_fire"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_V_KOOPA_FIRE1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_KOOPA_FLAME1"))
    USER_FUNC(spm::evt_snd::evt_snd_get_last_sfx_id, LW(14))
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_eff::evt_eff_softdelete, PTR("b_p_fire"))
    USER_FUNC(spm::evt_snd::evt_snd_sfx_fadeout, LW(14), 1000)
    USER_FUNC(check_guards, 3, 11, LW(11))
    IF_EQUAL(LW(11), 2)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(6), LW(7))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
      USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
      USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(1), LW(6), LW(7), 20, 300)
      BROTHER_EVT_ID(LW(4))
        RUN_CHILD_EVT(superguard_stylish)
      END_BROTHER()
    END_IF()
    IF_EQUAL(LW(11), 1)
      USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
      USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
      USER_FUNC(getMaxFP, LW(9))
      ADD(LW(10), LW(9))
      SUB(LW(10), 2)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      RUN_EVT(mod::marioRPGtakeDamage)
      USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
    END_IF()
    IF_EQUAL(LW(11), 0)
      USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_2"), 0)
      USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
      USER_FUNC(getMaxFP, LW(9))
      ADD(LW(10), LW(9))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      RUN_EVT(mod::marioRPGtakeDamage)
      USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
    END_IF()
    USER_FUNC(evt_npc_set_anim, LW(15), 11, 1)
    USER_FUNC(evt_npc_wait_anim_end, LW(15), 0)
    USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
    USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(2), FLOAT(0.0), FLOAT(190.0), 0, 0, 0)
    WAIT_FRM(1)
    USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(15), 3, 0)
    DO(0)
      CHK_EVT(LW(4), LW(0))
      IF_EQUAL(LW(0), 0)
        DO_BREAK()
      END_IF()
      WAIT_FRM(1)
    WHILE()
    RETURN()
  EVT_END()

  EVT_BEGIN(browser_edging_attack_2)
      USER_FUNC(evt_npc_set_anim, LW(15), 30, 1)
      USER_FUNC(evt_npc_get_position, LW(15), LW(1), LW(2), LW(3))
      SUB(LW(2), 10)
      USER_FUNC(evt_npc_set_position, LW(15), LW(1), LW(2), LW(3))
      BROTHER_EVT_ID(LW(8))
        SET(LW(9), FLOAT(0.0))
        LBL(59)
        ADDF(LW(9), FLOAT(15.0))
        USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, LW(9), EVT_NULLPTR)
        WAIT_FRM(1)
        GOTO(59)
      END_BROTHER()
      WAIT_MSEC(1500)
      USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
      USER_FUNC(spm::evt_npc::evt_npc_jump_to, LW(15), LW(1), LW(2), LW(3), 100, FLOAT(500.0))
      USER_FUNC(check_guards, 3, 11, LW(11))
      IF_EQUAL(LW(11), 2)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
        USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
        USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(1), LW(6), LW(7), 20, 300)
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
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        RUN_EVT(mod::marioRPGtakeDamage)
        USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
      END_IF()
      IF_EQUAL(LW(11), 0)
        USER_FUNC(spm::an2_08::evt_rpg_calc_mario_damage, 1, LW(10))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        RUN_EVT(mod::marioRPGtakeDamage)
        USER_FUNC(spm::an2_08::evt_rpg_mario_take_damage, LW(10), 0, LW(0))
      END_IF()
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
      ADD(LW(1), 40)
      USER_FUNC(spm::evt_npc::evt_npc_jump_to, LW(15), LW(1), LW(2), LW(3), 50, FLOAT(400.0))
      DELETE_EVT(LW(8))
      USER_FUNC(evt_npc_rotate, LW(15), EVT_NULLPTR, FLOAT(0.0), EVT_NULLPTR)
      USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
      USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(2), FLOAT(0.0), FLOAT(190.0), 0, 0, 0)
      WAIT_FRM(1)
      USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
      RETURN()
  EVT_END()

  EVT_BEGIN(browser_edging_attack)
    USER_FUNC(getMaxFP, LW(9))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 3, LW(0))
    IF_SMALL(LW(0), LW(9))
      IF_EQUAL(LW(0), 0)
        // vacuum evt here
      ELSE()
      
      END_IF()
    END_IF()
    USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(1))
    IF_SMALL(LW(0), LW(9))
      SUB(LW(1), 61)
    END_IF()
    SWITCH(LW(1))
      CASE_SMALL_EQUAL(30)
        RUN_CHILD_EVT(browser_edging_attack_2)
      CASE_ETC()
        RUN_CHILD_EVT(browserFIREfoxingItAttack)
    END_SWITCH()
    RETURN()
  EVT_END()

  NPCTribeAnimDef *getAnimsBrowserFirefoxExtension()
  {
    return animsBrowserFirefoxExtension;
  }

  EVT_BEGIN(browser_patch)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    USER_FUNC(start_boss_fight, 315)
    WAIT_FRM(70)
    USER_FUNC(evt_npc_delete, LW(10))
  RETURN_FROM_CALL()

  void browser_main()
  {
    npcTribes[69].attackStrength = 3;
    spm::map_data::MapData * ta1_04_md = spm::map_data::mapDataPtr("ta1_04");
    spm::evtmgr::EvtScriptCode* bowserBoomEvt = getInstructionEvtArg(ta1_04_md->initScript, 34, 5);
    evtpatch::hookEvtReplace(bowserBoomEvt, 325, browser_patch);
  }

}
