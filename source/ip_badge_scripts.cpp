#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "npc_rpgdrv.h"
#include "hpwindow.h"
#include "main_scripting.h"
#include "ip_messages.h"
#include "ip.h"
#include "ip_badges.h"
#include "ip_badgepouch.h"
#include "tplpatch.h"
#include "oChunks.h"

#include <spm/system.h>
#include <spm/acdrv.h>
#include <spm/camdrv.h>
#include <spm/spmario_snd.h>
#include <spm/spmario.h>
#include <spm/evtmgr.h>
#include <spm/mario.h>
#include <spm/evtmgr_cmd.h>
#include <spm/msgdrv.h>
#include <spm/seq_mapchange.h>
#include <spm/effdrv.h>
#include <spm/evt_mobj.h>
#include <spm/hitdrv.h>
#include <spm/eff_nice.h>
#include <spm/eff_spm_hit.h>
#include <spm/dispdrv.h>
#include <spm/animdrv.h>
#include <spm/item_data.h>
#include <spm/npcdrv.h>
#include <spm/map_data.h>
#include <spm/fontmgr.h>
#include <spm/fairy.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <spm/system.h>
#include <spm/filemgr.h>
#include <spm/icondrv.h>
#include <spm/wpadmgr.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <wii/tpl.h>
#include <wii/mtx.h>
#include <msl/string.h>
#include <msl/stdio.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/an.h>

#include <common.h>

using namespace mod;

namespace ip {

#define PB_OVERRIDE_COUNT 1

struct PowerBounceOverride
{
    s32 tribeId;
    const spm::evtmgr::EvtScriptCode * script;
};

PowerBounceOverride pbOverrides[PB_OVERRIDE_COUNT] = {
  {
    270,
    power_bounce_chunks
  }
};

const char * no_fp = "<dq>\n"
"<p>\n"
"Not enough FP!\n"
"<k>\n";

s32 get_pb_override(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 index = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
    s32 tribe = getRpgTribeID(index);
    spm::evtmgr::EvtScriptCode * script = nullptr;
    for (s32 i = 0; i < PB_OVERRIDE_COUNT; i++)
    {
      if (pbOverrides[i].tribeId == tribe) {
        script = (spm::evtmgr::EvtScriptCode *)pbOverrides[i].script;
        break;
      }
    }
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)script);
    return 2;
}

EVT_DECLARE_USER_FUNC(get_pb_override, 2)

EVT_BEGIN(power_bounce_stylish)
  USER_FUNC(ac_success_reset)
  INLINE_EVT()
    SET(LW(10), 0)
    LBL(1)
        USER_FUNC(check_pressed_a_ac, LW(11))
        USER_FUNC(enable_disable_stylish, 1)
        IF_EQUAL(LW(11), 1)
          USER_FUNC(ac_success_toggle)
          GOTO(2)
        END_IF()
      WAIT_FRM(1)
      ADD(LW(10), 1)
      IF_EQUAL(LW(10), 21)
        GOTO(2)
      END_IF()
    GOTO(1)
    LBL(2)
    USER_FUNC(enable_disable_stylish, 0)
  END_INLINE()
  WAIT_FRM(11)
  USER_FUNC(check_ac_success, LW(11))
  IF_EQUAL(LW(11), 1)
    USER_FUNC(ac_success_reset)
    USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(5))
    ADD(LW(5), 100)
    USER_FUNC(spm::an2_08::evt_rpg_add_xp, LW(5))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU1"))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), -250, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    RUN_CHILD_EVT(increase_stylish)
    SET(LW(5), UW(4))
    INLINE_EVT()
      SET(LW(11), 0)
      SETF(LW(10), FLOAT(0.0))
      DO(0)
        USER_FUNC(mario_rotate_z, LW(10))
        ADD(LW(11), 1)
        ADDF(LW(10), FLOAT(18.0))
        WAIT_FRM(1)
        IF_EQUAL(LW(11), 20)
          DO_BREAK()
        END_IF()
      WHILE()
    END_INLINE()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("KJ_1A"), 0)
  END_IF()
  RETURN()
EVT_END()

EVT_BEGIN(power_bounce)
    USER_FUNC(getFP, LW(14))
    IF_SMALL(LW(14), 2)
      USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(no_fp), 0, 0)
      SET(LF(1), 1)
      RETURN()
    END_IF()
    USER_FUNC(get_pb_override, UW(0), LW(14))
    IF_NOT_EQUAL(LW(14), 0)
      SWITCH(UW(0))
        CASE_EQUAL(0)
          SET(LW(15), PTR("npc1"))
        CASE_EQUAL(1)
          SET(LW(15), PTR("npc2"))
        CASE_EQUAL(2)
          SET(LW(15), PTR("npc3"))
      END_SWITCH()
      RUN_CHILD_EVT(LW(14))
      RETURN()
    END_IF()
  SWITCH(UW(0))
    CASE_EQUAL(0)
      SET(LW(15), PTR("mobj1"))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("npc1"), LW(5), LW(6), LW(7))
    CASE_EQUAL(1)
      SET(LW(15), PTR("mobj2"))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("npc2"), LW(5), LW(6), LW(7))
    CASE_EQUAL(2)
      SET(LW(15), PTR("mobj3"))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("npc3"), LW(5), LW(6), LW(7))
  END_SWITCH()
  RUN_EVT(power_bounce_stylish)
  WAIT_FRM(9)
  USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 1, LW(15))
  INLINE_EVT()
  SET(LW(10), 0)
  LBL(1)
    IF_SMALL(LW(10), 20)
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
  ADD(LW(6), 30)
  USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 90, 500)
  SUB(LW(6), 30)
  USER_FUNC(mario_reset_rotation)
  USER_FUNC(check_ac_success, LW(11))
  USER_FUNC(spm::an2_08::evt_rpg_calc_damage_to_enemy, UW(0), 0, LW(10))
  IF_EQUAL(LW(11), 1)
    ADD(LW(10), 3)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_P_ACROBAT_RENZOKU2"))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("nice"), UW(6), 150, 0, 0, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, UW(0), LW(10), 0, LW(0))
    RUN_EVT(mod::damageAnims)
    USER_FUNC(displayDamage, LW(5), LW(6), LW(7), LW(10))
    USER_FUNC(subtractFP, 2)
  ELSE()
    USER_FUNC(displayDamage, LW(5), LW(6), LW(7), LW(10))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_ATTACK1"), PTR("SFX_P_V_PEACH_ATTACK1"), PTR("SFX_P_V_KOOPA_ATTACK1"), PTR("SFX_P_V_LUIGI_ATTACK1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_E_SMASH1"))
    USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, UW(0), LW(10), 0, LW(0))
    RUN_EVT(mod::damageAnims)
  END_IF()
  SUB(LW(5), 35)
  USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(5), LW(6), LW(7), 10, 300)
  SET(LW(5), UW(4))
  BROTHER_EVT_ID(LW(2))
    RUN_CHILD_EVT(runEnemyDeath)
  END_BROTHER()
  USER_FUNC(spm::evt_mario::evt_mario_pos_change, LW(5), LW(7), FLOAT(180.0))
  USER_FUNC(spm::evt_mario::evt_mario_set_pos, LW(5), FLOAT(0.0), FLOAT(0.0))
  USER_FUNC(spm::evt_mario::evt_mario_direction_face, 90, 200)
  USER_FUNC(spm::evt_mobj::evt_mobj_hit_onoff, 0, LW(15))
  DO(0)
    CHK_EVT(LW(2), LW(0))
    IF_EQUAL(LW(0), 0)
      DO_BREAK()
    END_IF()
    WAIT_FRM(1)
  WHILE()
  RETURN()
EVT_END()

}