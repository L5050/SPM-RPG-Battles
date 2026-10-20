#pragma once
#include "evt_cmd.h"

#include <common.h>

namespace mod::savepatch {

  void main(const char * name);
  EVT_DECLARE(savepatch_transition)

}