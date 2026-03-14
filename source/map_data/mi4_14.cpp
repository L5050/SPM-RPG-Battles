#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "map_data/mi4_14.h"
#include "npc_rpgdrv.h"

#include <patch.h>
#include <spm/evt_mobj.h>
#include <spm/map_data.h>
#include <spm/evtmgr.h>
#include <spm/npcdrv.h>
#include <spm/evt_snd.h>
#include <spm/evt_mario.h>
#include <spm/evt_sub.h>
#include <spm/evt_env.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/seqdef.h>

using namespace spm::npcdrv;
using namespace spm::evt_npc;

namespace mod {

const char * peach = "peach";
const char * tippi = "__guide__";
const char * pixl = "__fairy__";

EVT_BEGIN(turtle)
  USER_FUNC(start_boss_fight, 280)
  RETURN()
EVT_END()

void mi4_14_main()
{
  spm::map_data::MapData * mi4_14_md = spm::map_data::mapDataPtr("mi4_14");
  spm::evtmgr::EvtScriptCode* mi4_14_init_evt = mi4_14_md->initScript;
  spm::evtmgr::EvtScriptCode* quiz = getInstructionEvtArg(mi4_14_init_evt, 154, 0);
  evtpatch::hookEvtReplace(quiz, 456, (spm::evtmgr::EvtScriptCode*)turtle);
  evtpatch::hookEvtReplaceBlock(quiz, 449, (spm::evtmgr::EvtScriptCode*)insertNop, 453);
  return;
}

}