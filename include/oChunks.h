#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"

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

//EVT_DECLARE(koopa_attack)

EVT_DECLARE(chunks_onhit)
EVT_DECLARE(chunks_on_spawn)
EVT_DECLARE(chunks_attack)

NPCTribeAnimDef * getChunksAnims();
spm::evtmgr::EvtScriptCode * getChunksDeath();
void chunks_main();

}
