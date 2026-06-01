#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"

#include <spm/animdrv.h>
#include <spm/evtmgr.h>
#include <spm/effdrv.h>
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
#include <spm/evt_hit.h>
#include <spm/evt_door.h>
#include <spm/evt_case.h>
#include <spm/evt_pouch.h>
#include <spm/evt_seq.h>
#include <spm/spmario.h>
#include <spm/map_data.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/swdrv.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/rel/an.h>
#include <wii/os/OSError.h>
#include <msl/string.h>
#include <patch.h>

using namespace spm::npcdrv;

namespace mod {

 s32 spawn_robo_eff(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
 {
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  spm::mario::MarioWork * mwpp =  spm::mario::marioGetPtr();
  spm::effdrv::EffEntry * entry = spm::effdrv::eff_robo_vacuum(mwpp->position.x + 70.0, mwpp->position.y, mwpp->position.z);
  robo_vacuum_set_target(entry, evtEntry->id, (char*)spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
  const char * effName = (char*)spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
  spm::effdrv::effSetName(entry, effName);
  
  return 2;
 }

  void robo_succ_main()
  {
    
    return;
  }

}
