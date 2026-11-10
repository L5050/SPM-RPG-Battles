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
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/npcdrv.h>
#include <spm/spmario.h>
#include <spm/mario_pouch.h>
#include <spm/evt_snd.h>
#include <spm/mot_fairy_mario.h>
#include <spm/evt_cam.h>
#include <spm/evt_img.h>
#include <spm/evt_paper.h>
#include <spm/evt_mario.h>
#include <spm/evt_sub.h>
#include <spm/evt_case.h>
#include <spm/evt_eff.h>
#include <spm/evt_env.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/spmario_snd.h>

using namespace spm::npcdrv;
using namespace spm::evt_npc;
using namespace spm::evt_case;
using namespace spm::evt_snd;
using namespace spm::evt_cam;
using namespace spm::evt_img;
using namespace spm::evt_eff;
using namespace spm::evt_mario;
using namespace spm::evt_paper;
using namespace spm::evt_sub;

namespace mod {

u32 unfurlDuration = 0;
u32 curFrames = 0;
s32 unfurlEvtId = 0;
bool unfurled = false;
bool endingUnfurl = false;
bool unfurlTargetHit = false;

void new_func_spsndSFXOn(char * name)
{
  if (unfurled)
  {
    return;
  }
  spm::spmario_snd::spsndSFXOn(name);
  return;
}

void new_func_80058404(s32 camId, s32 isFlipped)
{
  spm::camdrv::func_80058404(camId, isFlipped);
  if (endingUnfurl)
  {
    writeWord(spm::camdrv::func_80058404, 0x138, LI(3, 0x10a));
    unfurled = false;
    endingUnfurl = false;
    curFrames = 0;
    unfurlDuration = 0;
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

s32 make2d_unfurl(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::mario::func_80129d8c();
  writeWord(spm::camdrv::func_80058404, 0x138, LI(3, 1000));
  return 2;
}
EVT_DECLARE_USER_FUNC(make2d_unfurl, 0)

s32 make3d_unfurl(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  unfurled = true;
  unfurlDuration = 20;
  spm::mario::func_80129d8c();
  writeWord(spm::camdrv::func_80058404, 0x138, LI(3, 1000));
  return 2;
}
EVT_DECLARE_USER_FUNC(make3d_unfurl, 0)

s32 hammer_hit_unfurl(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  s32 ret = 0;
  spm::hitdrv::HitObj * obj = spm::mot_fairy_mario::fairyHammerHitChk(&ret);
  spm::mario::marioGetPtr()->hitObjLastHammeredWall = obj;
  if (ret != 0)
  {
    ret = 1;
  }
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], ret);
  return 2;
}
EVT_DECLARE_USER_FUNC(hammer_hit_unfurl, 1)

EVT_BEGIN(unfurl_evt)
  USER_FUNC(make3d_unfurl)
  USER_FUNC(evt_snd_bgm_fadeout, 0, 0)
  USER_FUNC(evt_snd_bgmon, 1, PTR("BGM_EVT_UNFURL"))
  USER_FUNC(evt_snd_bgm_fadein, 1, 0)
  USER_FUNC(evt_mario_get_character, LW(0))
  USER_FUNC(evt_mario_start_intermission, LW(0))
  WAIT_MSEC(20000)
  RETURN()
EVT_END()

EVT_BEGIN(unfurl_evt_end)
  USER_FUNC(evt_snd_bgm_fadeout, 1, 1000)
  USER_FUNC(evt_mario_get_character, LW(0))
  USER_FUNC(evt_mario_end_intermission, LW(0))
  USER_FUNC(make2d_unfurl)
  USER_FUNC(evt_snd_bgm_fadein, 0, 500)
  WAIT_MSEC(500)
  USER_FUNC(evt_snd_bgmoff, 1)
  RETURN()
EVT_END()

EVT_BEGIN(unfurl_hit_visual)
  INLINE_EVT_ID(LW(9))
      USER_FUNC(evt_cam_shake, 5, FLOAT(1.5), FLOAT(1.5), FLOAT(0.0), 0, 0)
  END_INLINE()
  USER_FUNC(evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
  USER_FUNC(evt_snd_get_last_sfx_id, LW(8))
  USER_FUNC(evt_mario_direction_face, 90, 200)
  USER_FUNC(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(evt_mario_set_pose, PTR("E_5"), 0)
  USER_FUNC(evt_mario_wait_anim)
  USER_FUNC(evt_mario_set_pose, PTR("T_7"), 0)
  USER_FUNC(evt_paper_entry, PTR("OFF_d_16pazu"))
  USER_FUNC(spm::evt_env::evt_env_blur_on, 0, 2600)
  USER_FUNC(evt_paper_entry, PTR("OFF_rot"))
  USER_FUNC(evt_img_entry, PTR("dummy"))
  USER_FUNC(evt_img_set_paper, PTR("dummy"), PTR("OFF_rot"))
  USER_FUNC(evt_img_alloc_capture, PTR("dummy"), 0, 0, 1, 0, 0, 608, 480)
  USER_FUNC(evt_img_set_paper_anim, PTR("dummy"), PTR("Z_1"))
  USER_FUNC(evt_img_free_capture, PTR("dummy"), 1)
  USER_FUNC(evt_img_set_position, PTR("dummy"), 0, 0)
  USER_FUNC(evt_img_set_z, PTR("dummy"), 100)
  WAIT_FRM(1)
  //USER_FUNC(evt_map_playanim, PTR(&s_anm_zun_80cb6ef8), 2, 0)
  USER_FUNC(evt_mario_set_character, 2)
  WAIT_FRM(1)
  USER_FUNC(evt_img_entry, PTR("img"))
  USER_FUNC(evt_img_set_paper, PTR("img"), PTR("OFF_d_16pazu"))
  USER_FUNC(evt_img_alloc_capture, PTR("img"), 0, 0, 0, 0, 0, 608, 480)
  USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("Z_1"))
  USER_FUNC(evt_img_set_position, PTR("img"), 0, 0)
  USER_FUNC(evt_img_free_capture, PTR("img"), 1)
  USER_FUNC(evt_img_set_z, PTR("img"), -100)
  INLINE_EVT()
      DELETE_EVT(LW(9))
      USER_FUNC(evt_cam_shake, 5, FLOAT(1.0), FLOAT(1.0), FLOAT(0.0), 500, 0)
      USER_FUNC(evt_cam_shake, 5, FLOAT(0.5), FLOAT(0.5), FLOAT(0.0), 500, 0)
      USER_FUNC(evt_cam_shake, 5, FLOAT(0.0), FLOAT(0.0), FLOAT(0.0), 500, 0)
  END_INLINE()
  USER_FUNC(evt_snd_sfx_fadeout, LW(8), 1000)
  USER_FUNC(evt_sub_intpl_msec_init, 11, 500000, 0, 1000)
  DO(0)
      USER_FUNC(evt_sub_intpl_msec_get_value)
      DIVF(LW(0), FLOAT(100.0))
      USER_FUNC(evt_img_set_position, PTR("img"), 0, LW(0))
      USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("A_1"))
      WAIT_FRM(1)
      IF_EQUAL(LW(1), 0)
          DO_BREAK()
      END_IF()
  WHILE()
  USER_FUNC(evt_sub_intpl_msec_init, 11, 100, 0, 1000)
  DO(0)
      USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("A_1"))
      WAIT_FRM(1)
      IF_EQUAL(LW(1), 0)
          DO_BREAK()
      END_IF()
  WHILE()
  USER_FUNC(evt_snd_sfxon, PTR("SFX_EVT_PUZZLE1"))
  USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("A_1"))
  WAIT_FRM(1)
  USER_FUNC(evt_img_wait_animend, PTR("img"))
  WAIT_MSEC(2000)
  USER_FUNC(evt_cam_zoom_to_coords, 0, 11)
  USER_FUNC(evt_img_release, PTR("img"))
  USER_FUNC(evt_img_release, PTR("dummy"))
  USER_FUNC(evt_paper_delete, PTR("OFF_d_16pazu"))
  USER_FUNC(evt_paper_delete, PTR("OFF_rot"))
  RETURN()
EVT_END()

EVT_BEGIN(unfurl_evt_fwd)
  INLINE_EVT()
    USER_FUNC(evt_snd_sfxon, PTR("SFX_MOBJ_BLUE_SWITCH1"))
    USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("unfurl"), PTR("A_1"))
  END_INLINE()
  RUN_CHILD_EVT(unfurl_evt)
  //USER_FUNC(evt_mario_key_off, 1)
  //RUN_CHILD_EVT(unfurl_hit_visual)
  USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("unfurl"), PTR("A_2"))
  USER_FUNC(spm::evt_mobj::evt_mobj_wait_animation_end, PTR("unfurl"), 0)
  USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("unfurl"), PTR("S_1"))
  RETURN()
EVT_END()

EVT_BEGIN(unfurl_hammer)
  USER_FUNC(evt_mario_key_off, 1)
  WAIT_FRM(1)
  USER_FUNC(hammer_hit_unfurl, LW(0))
  IF_EQUAL(LW(0), 0)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("H_1C"), 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_HAMMER_SWING1"))
    USER_FUNC(spm::evt_mario::evt_mario_wait_anim)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_HAMMER_HIT1"))
  ELSE()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("H_1D"), 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_HAMMER_SWING1"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_HAMMER_HIT1"))
    USER_FUNC(spm::evt_mario::evt_mario_wait_anim)
  END_IF()
  WAIT_FRM(5)
  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
  USER_FUNC(evt_mario_key_on)
  RETURN()
EVT_END()


static void( * seqMain)();

static void new_seqMain()
{
  seqMain();
  if (unfurled)
  {
    // Initialize unfurl, maybe could make a sep bool for that but this works and im lazy as FUQ rn
    if (curFrames == 0 && unfurlDuration > 0)
    {
      wii::gx::GXColor newMarioColor = {0, 255, 255, 255};
      spm::mario::MarioWork *mario = spm::mario::marioGetPtr();

      for (s32 i = 0; i < 8; ++i)
      {
        if (mario->animPoseIds[i] != -1)
        {
          spm::animdrv::animPoseSetMaterialEvtColor(mario->animPoseIds[i], newMarioColor);
          spm::animdrv::animPoseSetFlagF4On(mario->animPoseIds[i], 100000);
          spm::animdrv::animPoseSetMaterialFlagOn(mario->animPoseIds[i], 0x2000);
        }
      }
      ++curFrames;
      return;
    }
    // Ends unfurl
    if (curFrames == spm::spmario::gp->fps * unfurlDuration)
    {
      wii::gx::GXColor newMarioColor = {255, 255, 255, 255};
      spm::mario::MarioWork *mario = spm::mario::marioGetPtr();
      for (s32 i = 0; i < 8; ++i)
      {
        if (mario->animPoseIds[i] != -1)
        {
          spm::animdrv::animPoseSetMaterialEvtColor(mario->animPoseIds[i], newMarioColor);
          spm::animdrv::animPoseSetFlagF4Off(mario->animPoseIds[i], 100000);
          spm::animdrv::animPoseSetMaterialFlagOff(mario->animPoseIds[i], 0x2000);
        }
      }
      endingUnfurl = true;
      unfurled = false;
      unfurlEvtId = evtEntry(unfurl_evt_end, 1, 0)->id;
      return;
    }
    u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
    if ((pressed & 0x200) && !evtCheckID(unfurlEvtId))
    {
      s32 id = spm::mario::marioGetPtr()->motionId;
      if (id == 0 || id == 1 || id == 2)
      {
        unfurlEvtId = evtEntry(unfurl_hammer, 1, 0)->id;
      }
    }
    ++curFrames;
  }
  return;
}

void gameInitPatch()
{
  if (unfurled)
  {
    spm::mario::func_80129d8c();
    writeWord(spm::camdrv::func_80058404, 0x138, LI(3, 2500));
    wii::gx::GXColor newMarioColor = {0, 255, 255, 255};
    spm::mario::MarioWork *mario = spm::mario::marioGetPtr();

    for (s32 i = 0; i < 8; ++i)
    {
      if (mario->animPoseIds[i] != -1)
      {
        spm::animdrv::animPoseSetMaterialEvtColor(mario->animPoseIds[i], newMarioColor);
        spm::animdrv::animPoseSetFlagF4On(mario->animPoseIds[i], 100000);
        spm::animdrv::animPoseSetMaterialFlagOn(mario->animPoseIds[i], 0x2000);
      }
    }
    spm::spmario_snd::spsndBGMFadeout(0, 0);
  }
  return;
}

EVT_BEGIN(ta1_01_patch)
  USER_FUNC(evt_mobj_sui_new, 11, PTR("unfurl"), -150, 100, FLOAT(-138.0), (s32)unfurl_evt, 0, EVT_NULLPTR)
  USER_FUNC(evt_run_case_evt, 6, 1, PTR("MOBJ2_unfurl"), PTR("MOBJ3_unfurl"), PTR(unfurl_evt_fwd), LW(7))
  USER_FUNC(spm::evt_mobj::evt_mobj_save_blk, PTR("save"), -50, 100, FLOAT(-138.0), 0)
RETURN_FROM_CALL()

void unfurl_main()
{
  writeBranchLink(spm::camdrv::cam3dMain, 0x474, new_func_80058404);
  writeBranchLink(spm::mario_pouch::pouchMain, 0x14, new_checkHaveItem);
  writeBranchLink(spm::mario_pouch::pouchMain, 0x19C, new_func_spsndSFXOn);
  seqMain = patch::hookFunction(spm::seqdrv::seqMain, new_seqMain);
  writeBranch(spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].init, 0xC0, gameInitPatch);
}

void ta1_01_main()
{
  spm::map_data::MapData * ta1_01_md = spm::map_data::mapDataPtr("ta1_01");
  spm::evtmgr::EvtScriptCode* ta1_01_init_evt = ta1_01_md->initScript;
  evtpatch::hookEvtReplace(ta1_01_init_evt, 108, (spm::evtmgr::EvtScriptCode*)ta1_01_patch);
  unfurl_main();
  return;
}

}