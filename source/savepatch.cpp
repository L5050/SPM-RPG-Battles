#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "savepatch.h"

#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_fade.h>
#include <spm/evt_map.h>
#include <spm/evt_paper.h>
#include <spm/evt_img.h>
#include <spm/evt_sub.h>
#include <spm/evt_case.h>
#include <spm/evt_snd.h>
#include <spm/spmario.h>
#include <spm/map_data.h>
#include <spm/spmario_snd.h>
#include <spm/mario_pouch.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/swdrv.h>
#include <spm/seqdrv.h>
#include <spm/seq_load.h>
#include <spm/seq_load_sub.h>
#include <spm/nandmgr.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>
#include <msl/stdio.h>
#include <msl/string.h>

using namespace spm::evtmgr_cmd;
using namespace spm::nandmgr;
using namespace spm::wpadmgr;
using namespace spm::camdrv;
using namespace spm::evtmgr;
using namespace spm::animdrv;
using namespace spm::dispdrv;
using namespace spm::spmario;
using namespace spm::mario_pouch;
using namespace spm::seqdrv;

namespace mod::savepatch {

  const char * _saveName = nullptr;
  bool loadMainPaused = false;
  bool areSecondarySavesLoaded = false;

  s32 ( * seq_load_sub_loadMain )();

  s32 seq_load_sub_loadMain_new()
  {
    if (loadMainPaused)
    {
      // if we kill player control then the dispEntry for the window doesn't run, so we have to do that ourselves
      dispEntry(CAM_ID_3D, 4, 1000.0, (DispCallback *)spm::seq_load_sub::windowRenderMain, 0);
      return 0;
    } 
    u32 pressed = wpadGetButtonsPressed(0);
    if (pressed & 0x400)
    {
      // if B button pressed then kill player control of the cursor and run the savepatch evt
      loadMainPaused = true;
      spm::evtmgr::evtEntry(savepatch_transition, 1, 0);
      dispEntry(CAM_ID_3D, 4, 1000.0, (DispCallback *)spm::seq_load_sub::windowRenderMain, 0);
      return 0;
    }

    return seq_load_sub_loadMain();
  }

// ============================================================
// Evt helpers
// ============================================================

  void curtainDisplay(s32 param_1, int animId)
  {
    Mtx34 mtx;

    wii::mtx::PSMTXTrans(mtx, 0.0, 60.0, 110.0);
    animPoseMain(animId);
    animPoseDrawMtx(animId, mtx, 1, 0.0, 10.0);
    animPoseDrawMtx(animId, mtx, 2, 0.0, 10.0);
    animPoseDrawMtx(animId, mtx, 3, 0.0, 10.0);
    return;
  }

  s32 evt_animPoseSetAnim(EvtEntry *entry, bool firstCall)
  {
    s32 ret;
    EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    s32 id = evtGetValue(entry, *args);
    char *anim = (char *)evtGetValue(entry, args[1]);
    if (firstCall == 0)
    {
      f32 loop = (double)animPoseGetLoopTimes(id);
      if (loop < 1.0)
      {
        ret = 0;
      }
      else
      {
        ret = 2;
      }
    }
    else
    {
      animPoseSetAnim(id, anim, 1);
      ret = 0;
    }
    return ret;
  }
  EVT_DECLARE_USER_FUNC(evt_animPoseSetAnim, 2)

  s32 evt_animPoseCurtainEntry(EvtEntry *entry, bool firstCall)
  {
    EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    u32 asyncTest = animGroupBaseAsync("OFF_d_maku_l_r_h", 0, 0);
    if (asyncTest == 0)
    {
      return 0;
    }
    else
    {
      s32 id = animPoseEntry("OFF_d_maku_l_r_h", 0);
      evtSetValue(entry, *args, id);
      return 2;
    }
  }
  EVT_DECLARE_USER_FUNC(evt_animPoseCurtainEntry, 1)

  s32 toggle_secondary_saves(EvtEntry *entry, bool firstCall)
  {
    // changes the active save file slots from 1-4 to 5-8
    if (areSecondarySavesLoaded)
    {
      areSecondarySavesLoaded = false;
    }
    else
    {
      areSecondarySavesLoaded = true;
    }
    // tells nandmgr to reload nandmgr_wp->saves
    nandmgr_wp->task = 5;
    nandmgr_wp->stage = 0;
    nandmgr_wp->flag = 1;
    nandmgr_wp->code = 0;
    nandmgr_wp->saveId = 0;
    return 2;
  }
  EVT_DECLARE_USER_FUNC(toggle_secondary_saves, 0)

  s32 finish_curtain(EvtEntry *entry, bool firstCall)
  {
    EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    s32 id = evtGetValue(entry, *args);
    animPoseRelease(id);
    loadMainPaused = false;
    return 2;
  }
  EVT_DECLARE_USER_FUNC(finish_curtain, 1)

  s32 evt_CurtainDisplay(EvtEntry *entry, bool firstCall)
  {
    EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    s32 id = evtGetValue(entry, *args);
    dispEntry(CAM_ID_2D, 4, 0.0, (DispCallback *)curtainDisplay, (void *)id);
    return 0;
  }
  EVT_DECLARE_USER_FUNC(evt_CurtainDisplay, 1)

  s32 new_sprintf(char *buffer, const char *saveName, s32 saveId)
  {
    if (areSecondarySavesLoaded)
    {
      saveId += 4;
    }
    return msl::stdio::sprintf(buffer, _saveName, saveId);
  }

// ============================================================
// Save transition evt and main func
// ============================================================

  EVT_BEGIN(savepatch_transition)
    USER_FUNC(evt_animPoseCurtainEntry, LW(0))
    WAIT_FRM(1)
    INLINE_EVT_ID(LW(15))
      USER_FUNC(evt_CurtainDisplay, LW(0))
    END_INLINE()
    USER_FUNC(toggle_secondary_saves) // start loading the new save files before the curtain animation even plays so that it finishes loading faster
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_OPEN_CURTAIN1"))
    USER_FUNC(evt_animPoseSetAnim, LW(0), PTR("A_4A"))
    WAIT_MSEC(250)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_EVT_CLOSE_CURTAIN1"))
    USER_FUNC(evt_animPoseSetAnim, LW(0), PTR("A_4B"))
    DELETE_EVT(LW(15))
    WAIT_FRM(1)
    USER_FUNC(finish_curtain, LW(0))
    RETURN()
  EVT_END()

// ============================================================
// Autosave system
// ============================================================

  bool isAutoSaveActive = false;
  SpmarioGlobals spmgBackup;
  MarioPouchWork mpBackup;
  char * autoSaveMapName = nullptr;
  char * autoSaveEntranceName = nullptr;
  

  s32 evt_Activate_Autosave(EvtEntry *entry, bool firstCall)
  {
    EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    autoSaveMapName = (char *)evtGetValue(entry, args[0]);
    autoSaveEntranceName = (char *)evtGetValue(entry, args[1]);
    msl::string::memcpy(&spmgBackup, gp, sizeof(SpmarioGlobals));
    msl::string::memcpy(&mpBackup, pouchGetPtr(), sizeof(MarioPouchWork));
    isAutoSaveActive = true;
    
    return 2;
  }

  s32 evt_Deactivate_Autosave(EvtEntry *entry, bool firstCall)
  {
    isAutoSaveActive = false;
    
    return 2;
  }

  void handleAutoSave(s32 seqNum, const char * p0, const char * p1)
  {
    if (!isAutoSaveActive)
    {
      return seqSetSeq(seqNum, p0, p1);
    }
    msl::string::memcpy(gp, &spmgBackup, sizeof(SpmarioGlobals));
    msl::string::memcpy(pouchGetPtr(), &mpBackup, sizeof(MarioPouchWork));
    seqSetSeq(SEQ_MAPCHANGE, autoSaveMapName, autoSaveEntranceName);
    return;
  }


  void main(const char * name)
  {
    _saveName = name;
    seq_load_sub_loadMain = patch::hookFunction(spm::seq_load_sub::seq_load_sub_loadMain, seq_load_sub_loadMain_new);
    writeBranchLink(nandWriteBannerLoadAllSavesMain, 0x42C, new_sprintf);
    writeBranchLink(nandWriteAllSavesMain, 0x184, new_sprintf);
    writeBranchLink(nandWriteAllSavesMain, 0x210, new_sprintf);
    writeBranchLink(nandWriteSaveMain, 0x16C, new_sprintf);
    writeBranchLink(nandDeleteSaveMain, 0x15C, new_sprintf);
    writeWord(nandWriteAllSavesMain, 0x458, CMPWI(0, 8));
    writeBranchLink(spm::seqdef::seq_data[SEQ_GAMEOVER].main, 0x448, handleAutoSave);
  }

}