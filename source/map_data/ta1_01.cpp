#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "main_scripting.h"
#include "map_data/ta1_01.h"
#include "npc_rpgdrv.h"

#include <patch.h>
#include <spm/evt_mobj.h>
#include <spm/map_data.h>
#include <spm/evtmgr.h>
#include <spm/camdrv.h>
#include <spm/npcdrv.h>
#include <spm/mario_pouch.h>
#include <spm/evt_snd.h>
#include <spm/evt_mario.h>
#include <spm/evt_sub.h>
#include <spm/evt_case.h>
#include <spm/evt_env.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/seqdef.h>

using namespace spm::npcdrv;
using namespace spm::evt_npc;
using namespace spm::evt_case;
using namespace spm::evt_snd;

namespace mod {

bool unfurled = false;

void new_func_80058404(s32 camId, s32 isFlipped)
{
  spm::camdrv::func_80058404(camId, isFlipped);
  if (unfurled)
  {
    writeWord(spm::camdrv::func_80058404, 0x138, LI(3, 0x10a));
    unfurled = false;
  }
  return;
}

bool new_checkHaveItem(s32 id)
{
  if (unfurled)
  {
    return true;
  } else {
    return spm::mario_pouch::pouchCheckHaveItem(id);
  }
}

s32 make3d_unfurl(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  unfurled = true;
  spm::mario::func_80129d8c();
  writeWord(spm::camdrv::func_80058404, 0x138, LI(3, 750));
  return 2;
}
EVT_DECLARE_USER_FUNC(make3d_unfurl, 0)

EVT_BEGIN(unfurl_evt)
  USER_FUNC(animPoseSetMaterialEvtColorWrapper, 0, 255, 255, 255, 1)
  USER_FUNC(make3d_unfurl)
  USER_FUNC(evt_snd_bgm_fadeout, 0, 0)
  USER_FUNC(evt_snd_bgmon, 1, PTR("BGM_EVT_UNFURL"))
  USER_FUNC(evt_snd_bgm_fadein, 1, 0)
  USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
  USER_FUNC(spm::evt_mario::evt_mario_start_intermission, LW(0))
  WAIT_MSEC(20000)
  USER_FUNC(evt_snd_bgm_fadeout, 1, 1000)
  WAIT_MSEC(500)
  USER_FUNC(spm::evt_mario::evt_mario_end_intermission, LW(0))
  USER_FUNC(make3d_unfurl)
  USER_FUNC(animPoseSetMaterialEvtColorWrapper, 255, 255, 255, 255, 0)
  USER_FUNC(evt_snd_bgm_fadein, 0, 500)
  WAIT_MSEC(500)
  USER_FUNC(evt_snd_bgmoff, 1)
  RETURN()
EVT_END()

EVT_BEGIN(unfurl_evt_fwd)
  USER_FUNC(evt_snd_sfxon, PTR("SFX_MOBJ_BLUE_SWITCH1"))
  USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("unfurl"), PTR("A_1"))
  RUN_CHILD_EVT(unfurl_evt)
  USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("unfurl"), PTR("A_2"))
  USER_FUNC(spm::evt_mobj::evt_mobj_wait_animation_end, PTR("unfurl"), 0)
  USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("unfurl"), PTR("S_1"))
  RETURN()
EVT_END()


EVT_BEGIN(ta1_01_patch)
  USER_FUNC(evt_mobj_sui_new, 11, PTR("unfurl"), -150, 100, FLOAT(137.5), (s32)unfurl_evt, 0, EVT_NULLPTR)
  USER_FUNC(evt_run_case_evt, 6, 1, PTR("MOBJ2_unfurl"), PTR("MOBJ3_unfurl"), PTR(unfurl_evt_fwd), LW(7))
RETURN_FROM_CALL()

void ta1_01_main()
{
  spm::map_data::MapData * ta1_01_md = spm::map_data::mapDataPtr("ta1_01");
  spm::evtmgr::EvtScriptCode* ta1_01_init_evt = ta1_01_md->initScript;
  evtpatch::hookEvt(ta1_01_init_evt, 108, (spm::evtmgr::EvtScriptCode*)ta1_01_patch);
  writeBranchLink(spm::camdrv::cam3dMain, 0x474, new_func_80058404);
  writeBranchLink(spm::mario_pouch::pouchMain, 0x14, new_checkHaveItem);
  writeWord(spm::mario_pouch::pouchMain, 0x19C, NOP);
  return;
}

}