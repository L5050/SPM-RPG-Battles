#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <patch.h>
#include <spm/rel/an.h>
#include <spm/evtmgr.h>
#include <spm/rel/an.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod {

EVT_DECLARE(mimi_attack)
EVT_DECLARE(mimi_ruby_attack)
EVT_DECLARE(mimi_onspawn)
EVT_DECLARE(mimi_onhit) 
EVT_DECLARE(mimi_throw_script)

void mimi_main();
NPCTribeAnimDef * getMimiAnims();

}
