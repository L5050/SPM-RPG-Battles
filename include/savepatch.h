#pragma once
#include "evt_cmd.h"

#include <common.h>

namespace mod::savepatch {

  void main(const char * name);
  EVT_DECLARE(savepatch_transition)
  EVT_DECLARE(auto_save)
  EVT_DECLARE_USER_FUNC(evt_Activate_Autosave, 2)
  EVT_DECLARE_USER_FUNC(evt_Deactivate_Autosave, 0)
  EVT_DECLARE_USER_FUNC(set_choice, 1)

}