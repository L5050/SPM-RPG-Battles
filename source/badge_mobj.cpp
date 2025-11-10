#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "npc_rpgdrv.h"
#include "hpwindow.h"
#include "main_scripting.h"
#include "ip_messages.h"
#include "ip.h"
#include "ip_badges.h"
#include "ip_badgepouch.h"
#include "tplpatch.h"

#include <spm/system.h>
#include <spm/acdrv.h>
#include <spm/camdrv.h>
#include <spm/spmario_snd.h>
#include <spm/spmario.h>
#include <spm/evtmgr.h>
#include <spm/mario.h>
#include <spm/evtmgr_cmd.h>
#include <spm/msgdrv.h>
#include <spm/seq_mapchange.h>
#include <spm/effdrv.h>
#include <spm/hitdrv.h>
#include <spm/eff/eff_nice.h>
#include <spm/eff/eff_spm_hit.h>
#include <spm/dispdrv.h>
#include <spm/animdrv.h>
#include <spm/item_data.h>
#include <spm/eff/eff_map_block_del.h>
#include <spm/npcdrv.h>
#include <spm/map_data.h>
#include <spm/evt_mobj.h>
#include <spm/eff/eff_small_star.h>
#include <spm/mario_motion.h>
#include <spm/fontmgr.h>
#include <spm/fairy.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <spm/system.h>
#include <spm/filemgr.h>
#include <spm/icondrv.h>
#include <spm/wpadmgr.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <wii/tpl.h>
#include <wii/mtx.h>
#include <msl/string.h>
#include <msl/stdio.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/an.h>
#include <spm/rel/sp4_13.h>
#include <cstdio>
USING(wii::mtx::Vec3)
USING(ip::BadgeId)

using namespace spm;

namespace mod {

  // Patch agb async, maybe this does something
  const char *customModels[] = {"MOBJ_w_sui",
                                "terminator"};
  u32 (*animGroupBaseAsyncReal)(const char *animPoseName, s32 param_2, void *readDoneCb);
  static void agbAsyncPatch()
  {
    animGroupBaseAsyncReal = patch::hookFunction(spm::animdrv::animGroupBaseAsync,
                                                 [](const char *animPoseName, s32 param_2, void *readDoneCb)
                                                 {
                                                   // Patch functions that rely on ag2tg to fileAsyncf custom textures
                                                   bool patch = false;
                                                   for (s32 idx = 0; msl::string::strcmp(customModels[idx], "terminator") != 0; idx = idx + 1)
                                                   {
                                                     if (msl::string::strcmp(animPoseName, customModels[idx]) == 0)
                                                     {
                                                       patch = true;
                                                     }
                                                   }
                                                   if (patch)
                                                   {
                                                     spm::filemgr::FileEntry *texture = spm::filemgr::fileAsyncf(4, 0, "%s/%s-", "a", animPoseName);
                                                     return (-(s32)texture | (u32)texture) >> 0x1f;
                                                   }
                                                   return animGroupBaseAsyncReal(animPoseName, param_2, readDoneCb);
                                                 });
  }


    const char *mobjSuiNamesNew[] = {
        "MOBJ_b_sui",
        "MOBJ_r_sui",
        "MOBJ_big_b_sui",
        "MOBJ_big_r_sui",
        "MOBJ_p_b_sui",
        "MOBJ_p_r_sui",
        "MOBJ_p_b_sui",
        "MOBJ_p_r_sui",
        "MOBJ_w_sui",
        "MOBJ_01_sui",
        "MOBJ_02_sui",
        "MOBJ_03_sui",
        "MOBJ_04_sui",
        "HOBJ_BlueSwitch",
        "HOBJ_RedSwitch",
        "MOBJ_stg3_sui",
        "MOBJ_stg3_sui",
        "MOBJ_STG3_p_b_sui"};

const char *mobjSuiPerishableNames[] = {
        "MOBJ_b_sui",
        "MOBJ_big_b_sui",
        "MOBJ_p_b_sui",
        "MOBJ_w_sui",
        "HOBJ_BlueSwitch",
        "MOBJ_STG3_p_b_sui"};

    s32 mobj_sui_new(mobjdrv::MobjEntry *mobj)
    {
        const char *name = mobjdrv::mobjGetModelName(mobj);
        bool perishable = false;
        if (mobj->state == 1 && (((f32)animdrv::animPoseGetLoopTimes(mobj->animPoseId)) >= 1.0))
        {
            s32 variant = 1;
            f32 scale = 0.5;
            u32 i = 0;
            while (i < (sizeof(mobjSuiPerishableNames) / 4))
            {
                if ((msl::string::strcmp(name, mobjSuiPerishableNames[i]) == 0))
                {
                    mobj->flag0 = (mobj->flag0 | 0x4000); // Unknown flag
                    perishable = true;
                    mobjdrv::mobjHitOff(mobj->instanceName);
                    if ((msl::string::strcmp(name, "MOBJ_big_b_sui") == 0))
                    {
                        scale = 1.0;
                    }
                    break;
                }
                i = i + 1;
                if (i == (sizeof(mobjSuiPerishableNames) / 4))
                {
                    variant = 0;
                    scale = 1.0;
                }
            }
            eff_map_block_del::effMapBlockDelEntry(mobj->pos.x, ((mobj->pos.y) + 12.5), mobj->pos.z, scale, variant);
            mobj->state = mobj->state + 1;
        }
        else if (mobj->state == 0 && ((mobj->flag0 & 0x8) != 0)) // IS_USING
        {
            mobjdrv::func_8002ed90();
            mobjdrv::mobjRunEvent(mobj, mobj->interactScript);
            if ((mobj->unknown_0x250 == 0x800000) || (mobj->unknown_0x250 == 0x2000000))
            {
                mario_motion::marioChgMot(8);
            }
            spmario_snd::spsndSFXOn_3D("SFX_MOBJ_BLUE_SWITCH1", &mobj->pos);
            animdrv::animPoseSetAnim(mobj->animPoseId, "A_1", 1);
            eff_small_star::effSmallStarEntry(mobj->pos.x, (mobj->pos.y + 12.5), mobj->pos.z, 0,
                                              -1, 0, 0, 4);
            mobj->state = mobj->state + 1;
        }
        else if (mobj->state == 2 && ((evtmgr::evtCheckID(mobj->evtId)) == false)) // Probably just state == 2, but maybe it's also meant to account for -1, etc.
        {
            animdrv::animPoseSetAnim(mobj->animPoseId, "S_1", 1);
            if (mobj->collectedVar != 0)
            {
                evtmgr_cmd::evtSetValue(0, mobj->collectedVar, 1);
            }
            mobjdrv::func_8002ed80();
            mobj->state = 0;
            mobj->flag0 = (mobj->flag0 & ~0x8); // IS_USING
            name = mobjdrv::mobjGetModelName(mobj);
            if (perishable)
            {
                mobjdrv::mobjDelete(mobj->instanceName);
            }
        }
        return 0;
    }

s32 evt_mobj_sui_new(evtmgr::EvtEntry *evtEntry, bool isFirstCall)
    {
        (void)isFirstCall;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 type = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        char *name = (char *)evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        f32 x = evtmgr_cmd::evtGetFloat(evtEntry, args[2]);
        f32 y = evtmgr_cmd::evtGetFloat(evtEntry, args[3]);
        f32 z = evtmgr_cmd::evtGetFloat(evtEntry, args[4]);
        evtmgr::EvtScriptCode *interact = (evtmgr::EvtScriptCode *)evtmgr_cmd::evtGetValue(evtEntry, args[5]);
        s32 inactive = args[6];
        void *stg3SuiNames = (void *)evtmgr_cmd::evtGetValue(evtEntry, args[7]);
        s32 agbAsync = animdrv::animGroupBaseAsync(mobjSuiNamesNew[type], 0, 0);
        if (agbAsync == 0)
        {
            return 0;
        }
        mobjdrv::mobjEntry(name, mobjSuiNamesNew[type]);
        mobjdrv::MobjEntry *mobj = mobjdrv::mobjNameToPtr(name);
        if (type == evt_mobj::MobjSuiType::MOBJ_SUI_BLUE_3D || type == evt_mobj::MobjSuiType::MOBJ_SUI_RED_3D)
        {
            mobj->flag0 = (mobj->flag0 | 0x400); // 3D_ONLY
            (mobj->rotation).y = 270.0;
            hitdrv::hitDelete(mobj->joints[1].name);
            mobj->joints[1].hitObj = (hitdrv::HitObj *)0x0;
        }
        else if (type == evt_mobj::MobjSuiType::MOBJ_SUI_BLUE_2D || type == evt_mobj::MobjSuiType::MOBJ_SUI_RED_2D || type == evt_mobj::MobjSuiType::MOBJ_SUI_STG3_BLUE_2D)
        {
            mobj->flag0 = (mobj->flag0 | 0x200); // 2D_ONLY
            hitdrv::hitDelete(mobj->joints[0].name);
            mobj->joints[0].hitObj = (hitdrv::HitObj *)0x0;
        }
        mobjdrv::mobjSetPosition(name, x, y, z);
        if (type == evt_mobj::MobjSuiType::MOBJ_SUI_STG3_BLUE || type == evt_mobj::MobjSuiType::MOBJ_SUI_STG3_PINK)
        {
        }
        else
        {
            mobj->updateFunction = (mobjdrv::MobjUpdateFunc *)mobj_sui_new;
        }
        mobj->interactScript = interact;
        mobj->flag0 = (mobj->flag0 | 0xeab00004); // tons of unknown flags
        mobj->collectedVar = inactive;
        mobj->state = 0;
        if (type == evt_mobj::MobjSuiType::MOBJ_SUI_BLUE_BIG || type == evt_mobj::MobjSuiType::MOBJ_SUI_RED_BIG)
        {
            mobj->flag0 = (mobj->flag0 & 0x20fffff); // I have no idea what this means
        }
        if (type == evt_mobj::MobjSuiType::MOBJ_SUI_BLUE_SMALL || type == evt_mobj::MobjSuiType::MOBJ_SUI_RED_SMALL)
        {
            (mobj->scale).x = 0.5;
            (mobj->scale).y = 0.5;
            (mobj->scale).z = 1.0;
            mobj->flag0 = (mobj->flag0 | 0x800);
        }
        if (type == evt_mobj::MobjSuiType::MOBJ_SUI_STG3_PINK)
        {
            animdrv::animPoseSetAnim(mobj->animPoseId, "S_2", 1);
        }
        else
        {
            animdrv::animPoseSetAnim(mobj->animPoseId, "S_1", 1);
        }
        mobjdrv::mobjCalcMtx(mobj);
        if ((inactive != 0) && ((evtmgr_cmd::evtGetValue(evtEntry, inactive)) != 0))
            mobjdrv::mobjDelete(name);
        return 2;
    }

EVT_DECLARE_USER_FUNC(evt_mobj_sui_new, 8)

  const char * sleepy_stomp_text = "<system><p>\n"
  "You got Sleepy Stomp!\n"
  "Sleepy Stomp automatically\n"
  "pacifies enemies you defeat!\n"
  "<k>\n"
  "<p>\n"
  "It costs 0 BP to wear.\n"
  "<k>\n";

  EVT_BEGIN(getSleepyStomp)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(sleepy_stomp_text), 0, 0)
    USER_FUNC(ip::evt_pouch_add_badge, 2)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
  EVT_END()

  EVT_BEGIN(addSleepyStomp)
    USER_FUNC(ip::evt_pouch_init)
    USER_FUNC(ip::evt_pouch_check_for_badge, 2, LW(15))
    IF_EQUAL(LW(15), 0)
      USER_FUNC(evt_mobj_sui_new, 0x8, PTR("sleepyStomp"), FLOAT(100.0), FLOAT(150.0), FLOAT(0.0), (s32)getSleepyStomp, 0, EVT_NULLPTR)
    END_IF()
  RETURN_FROM_CALL()

  s32 patchSleepyStompTex(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
        mobjdrv::MobjEntry * entry = mobjdrv::mobjNameToPtr("sleepyStomp");
        animdrv::AnimPose *pose = animdrv::animPoseGetAnimPosePtr(entry->animPoseId);
        animdrv::AnimGroup *grp = &animdrv::animdrv_wp->animGroups[pose->animGroupId];
        animdrv::TextureGroup *tex = animdrv::animdrv_wp->textureGroups + grp->textureGroupId;
        tplpatch::TextureWork texWrk = {
            0,
            2,
            (wii::tpl::TPLHeader *)tex->file->sp->data,
            ip::badgeIconTpl,
            nullptr,
            false,
            spm::memory::Heap::HEAP_MEM1_UNUSED};
        tplpatch::patchTpl(&texWrk);
        return 2;
  }

  void mobj_main() {
    agbAsyncPatch();
    spm::map_data::MapData * he1_md = spm::map_data::mapDataPtr("he1_01");
    evtpatch::hookEvt(he1_md->initScript, 32, addSleepyStomp);
  }

}
