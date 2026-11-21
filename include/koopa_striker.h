#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"

#include <patch.h>
#include <spm/evtmgr.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod {

EVT_DECLARE(koopa_striker_attack)
EVT_DECLARE(koopa_striker_onhit)
NPCTribeAnimDef * getKoopaStrikerAnims();
void koopa_striker_main();

}
