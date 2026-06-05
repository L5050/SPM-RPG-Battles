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

  {14, "N_1"},
  {30, "AC_7"},
  {-1, nullptr}
};

namespace mod {

  EVT_BEGIN(browser_safarshit_onspawn)
    USER_FUNC(evt_npc_set_disp_callback, LW(15), PTR(bowserNpcDispCb))
    USER_FUNC(getMaxFP, LW(5))
    USER_FUNC(setFP, LW(5))
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
  
  EVT_BEGIN(browser_edging_attack)
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
        ADD(LW(10), 2)
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
        ADD(LW(10), 2)
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
      USER_FUNC(evt_npc_walk_to, LW(15), LW(0), LW(2), FLOAT(0.0), FLOAT(80.0), 0, 0, 0)
      WAIT_FRM(1)
      USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
      RETURN()
  EVT_END()

      NPCTribeAnimDef *getAnimsBrowserFirefoxExtension()
      {
        return animsBrowserFirefoxExtension;
      }

  void browser_main()
  {
    npcTribes[69].attackStrength = 3;
  }

}
