#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"
#include "barabara.h"

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

  EVT_BEGIN(barabara_attack)
    SET(LW(0), 0)
    IF_EQUAL(LW(0), 0)
          USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
          USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
          USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(8))
          ADD(LW(1), LW(8))
          SET(LW(5), LW(0))
          ADD(LW(5), 100)
          INLINE_EVT()
            SET(LW(13), UW(3))
            SUB(LW(13), 25)
            USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, LW(5), EVT_NULLPTR, LW(13), LW(5), EVT_NULLPTR, 200, 1000, 11)
          END_INLINE()
          USER_FUNC(evt_npc_set_anim, LW(15), 2, 1)
          USER_FUNC(evt_npc_glide_to, LW(15), LW(0), LW(1), LW(2), 0, FLOAT(200.0), 0, 0, 0, 0)
          USER_FUNC(evt_npc_set_anim, LW(15), 5, 1)
          WAIT_MSEC(500)
          USER_FUNC(evt_npc_set_anim, LW(15), 6, 1)
          WAIT_MSEC(500)
          USER_FUNC(check_guards, 3, 14, LW(11))
          IF_EQUAL(LW(11), 2)
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_COUNTER_REBOUND1"))
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("J_1B"), 0)
            USER_FUNC(evt_npc_set_anim, LW(15), 4, 1)
            USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
            USER_FUNC(spm::evt_mario::evt_mario_jump_to, LW(0), LW(1), LW(2), 20, 300)
            BROTHER_EVT_ID(LW(4))
              RUN_CHILD_EVT(superguard_stylish)
            END_BROTHER()
            ADD(LW(0), 50)
            USER_FUNC(evt_npc_jump_to, LW(15), LW(0), LW(1), LW(2), 75, FLOAT(500.0))
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
            USER_FUNC(evt_npc_set_anim, LW(15), 14, 1)
            USER_FUNC(evt_npc_get_position, LW(15), LW(5), LW(6), LW(7))
            USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_I_RECOVER_HP_EFFECT_SMALL1"))
            USER_FUNC(spm::evt_eff::evt_eff, PTR("health"), PTR("spm_recovery"), LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0, 0)
            MUL(LW(10), -1)
            USER_FUNC(spm::an2_08::evt_rpg_enemy_take_damage, UW(0), LW(10), 0, EVT_NULLPTR)
            WAIT_FRM(75)
          END_IF()
      USER_FUNC(evt_npc_set_anim, LW(15), 1, 1)
      INLINE_EVT()
        USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 0, UW(1), EVT_NULLPTR, UW(3), UW(1), EVT_NULLPTR, 200, 1000, 11)
      END_INLINE()
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 0, LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 1, LW(1))
      USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(15), 2, LW(2))
      USER_FUNC(evt_npc_glide_to, LW(15), LW(0), LW(1), LW(2), 0, FLOAT(200.0), 0, 0, 0, 0)
      USER_FUNC(evt_npc_set_anim, LW(15), 0, 1)
      USER_FUNC(ac_success_reset)
      LBL(5)
      CHK_EVT(LW(4), LW(0))
      IF_EQUAL(LW(0), 0)
        RETURN()
      END_IF()
      WAIT_FRM(1)
      GOTO(5)
    END_IF()
  EVT_END()

  void barabara_main()
  {
    npcTribes[108].attackStrength = 2;
  }

}
