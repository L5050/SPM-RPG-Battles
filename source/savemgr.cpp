#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "npc_rpgdrv.h"

#include <spm/animdrv.h>
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
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/rel/an.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/sp4_13.h>
#include <wii/os/OSError.h>
#include <patch.h>

using namespace spm::npcdrv;

namespace mod {

  const char * fp_restore = "<dq>\n"
  "<p>\n"
  "FP restored to max.\n"
  "<k>\n";

  const char * enemies_left = "<dq>\n"
  "<p>\n"
  "%d left.\n"
  "<k>\n";

s32 restore_fp(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  if (*maxFp == 0)
  {
    *maxFp = 5;
  }
  *fp = *maxFp;
  return 2;
}

EVT_DECLARE_USER_FUNC(restore_fp, 0)

  EVT_BEGIN(savemgr_evt)
    USER_FUNC(restore_fp)
    //USER_FUNC(spm::evt_msg::evt_msg_print_insert, 0, LW(0), 0, LW(1), LW(9), LW(10))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(fp_restore), 0, 0)
  RETURN_FROM_CALL()

  void savemgr_main()
  {
      evtpatch::hookEvt((spm::evtmgr::EvtScriptCode*)evt_memcard_save, 4, (spm::evtmgr::EvtScriptCode*)savemgr_evt);
  }

}
