#pragma once
#include "evt_cmd.h"

#include <common.h>

namespace mod {

  EVT_DECLARE_USER_FUNC(npcEntryFromTribeId, 1)
  EVT_DECLARE_USER_FUNC(increaseAttack, 1)
  EVT_DECLARE_USER_FUNC(rpg_npc_setup, 0)
  EVT_DECLARE_USER_FUNC(rpg_off, 0)
  EVT_DECLARE_USER_FUNC(enable_disable_rpg_menu, 1)
  EVT_DECLARE_USER_FUNC(enable_disable_stylish, 1)
  EVT_DECLARE_USER_FUNC(start_boss_fight, 1)
  EVT_DECLARE_USER_FUNC(getFP, 1)
  EVT_DECLARE_USER_FUNC(setFP, 1)
  EVT_DECLARE_USER_FUNC(getMaxFP, 1)
  EVT_DECLARE_USER_FUNC(setMaxFP, 1)
  EVT_DECLARE_USER_FUNC(addFP, 1)
  EVT_DECLARE_USER_FUNC(getMaxBP, 1)
  EVT_DECLARE_USER_FUNC(setMaxBP, 1)
  EVT_DECLARE_USER_FUNC(addBP, 1)
  EVT_DECLARE_USER_FUNC(getCurMap, 1)
  EVT_DECLARE_USER_FUNC(subtractFP, 1)
  EVT_DECLARE_USER_FUNC(osReportLW, 1)
  EVT_DECLARE_USER_FUNC(check_pressed_2_ac, 1)
  EVT_DECLARE_USER_FUNC(check_pressed_b_ac, 1)
  EVT_DECLARE_USER_FUNC(check_pressed_a_ac, 1)
  EVT_DECLARE_USER_FUNC(mario_rotate, 1)
  EVT_DECLARE_USER_FUNC(check_ac_success, 1)
  EVT_DECLARE_USER_FUNC(check_superguard_success, 1)
  EVT_DECLARE_USER_FUNC(ac_success_toggle, 0)
  EVT_DECLARE_USER_FUNC(superguard_toggle, 0)
  EVT_DECLARE_USER_FUNC(ac_success_reset, 0)
  EVT_DECLARE_USER_FUNC(displayDamage, 4)
  EVT_DECLARE_USER_FUNC(rpg_get_technique_index, 1)
  EVT_DECLARE_USER_FUNC(evt_rpg_add_player_effect, 3)
  EVT_DECLARE_USER_FUNC(mario_rotate_x, 1)
  EVT_DECLARE_USER_FUNC(mario_rotate_z, 1)
  EVT_DECLARE_USER_FUNC(patchSleepyStompTex, 0)
  EVT_DECLARE_USER_FUNC(mario_paper_on, 1)
  EVT_DECLARE_USER_FUNC(mario_chg_paper, 1)
  EVT_DECLARE_USER_FUNC(npc_change_name, 2)

  EVT_DECLARE(parentOfBeginRPG)
  EVT_DECLARE(deleteAttackedEnemy)
  EVT_DECLARE(marioRPGtakeDamage)
  EVT_DECLARE(damageAnims)
  EVT_DECLARE(rpg_jump)
  EVT_DECLARE(insertNop)
  EVT_DECLARE(runEnemyDeath)
  EVT_DECLARE(levelUpScript)

}
