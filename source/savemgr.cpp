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
#include <spm/rel/an2_08.h>
#include <spm/rel/sp4_13.h>
#include <wii/os/OSError.h>
#include <msl/string.h>
#include <patch.h>

using namespace spm::npcdrv;
using namespace spm::swdrv;
using namespace spm::evt_map;

namespace mod {

  const char * fp_restore = "<dq>\n"
  "<p>\n"
  "FP restored to max.\n"
  "<k>\n";

  const char * enemies_left = "<system><col c00000ff>\n"
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

s32( * evt_read_setup_file)(spm::evtmgr::EvtEntry * evtEntry, bool firstRun);

s32 genocide_check(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  s32 enemyRequirement = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  spm::mario_pouch::MarioPouchWork * mwpp = spm::mario_pouch::pouchGetPtr();
  if (mwpp->enemiesDefeated >= enemyRequirement && swGet(1801) != 1) {
    swSet(1802);
    wii::os::OSReport("gswf 1802 set to 1\n");
  } else {
    wii::os::OSReport("gswf 1802 set to 0\n");
    swClear(1802);
    swSet(1801);
  }
  return 2;
}

s32 genocide_check_2(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  s32 enemyRequirement = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  spm::mario_pouch::MarioPouchWork * mwpp = spm::mario_pouch::pouchGetPtr();
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], enemyRequirement - mwpp->enemiesDefeated);
  return 2;
}

s32 new_evt_read_setup_file(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  if (firstRun){
  char *name = spm::spmario::gp->mapName;
  const char *result = msl::string::strstr(name, "he2");
  if (result != nullptr) {
    if (swGet(1802)) {
      wii::os::OSReport("gswf 1802 is set to 1\n");
      return 2;
    }
  }}
  return evt_read_setup_file(evtEntry, firstRun);
}

s32 reduce_game_speed(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::mario::marioGetPtr()->statusFlags = 0x800;
  return 2;
}

EVT_DECLARE_USER_FUNC(restore_fp, 0)
EVT_DECLARE_USER_FUNC(genocide_check, 1)
EVT_DECLARE_USER_FUNC(genocide_check_2, 1)
EVT_DECLARE_USER_FUNC(reduce_game_speed, 0)

  EVT_BEGIN(genocide_check_evt)
    USER_FUNC(genocide_check_2, LW(15))
    IF_LARGE(LW(15), 0)
      USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
      USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(enemies_left), 0, 0, LW(15))
      USER_FUNC(spm::evt_mario::evt_mario_key_on)
    END_IF()
  RETURN()
  EVT_END()

  EVT_BEGIN(he3_08_hook)
    DO(0)
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        IF_LARGE(LW(0), FLOAT(-200.0))
            DO_BREAK()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    SET(LW(15), 40)
    RUN_CHILD_EVT(genocide_check_evt)
  RETURN()
  EVT_END()

  EVT_BEGIN(he3_08_hook_fwd)
    IF_EQUAL(GSWF(1802), 1)
      RUN_EVT(he3_08_hook)
    END_IF()
  RETURN_FROM_CALL()

  EVT_BEGIN(he4_09_hook)
    WAIT_FRM(80)
    DO(0)
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        IF_LARGE(LW(0), FLOAT(0.0))
            DO_BREAK()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    SET(LW(15), 55)
    RUN_CHILD_EVT(genocide_check_evt)
  RETURN()
  EVT_END()

  EVT_BEGIN(he4_09_hook_fwd)
    IF_EQUAL(GSWF(1802), 1)
      RUN_EVT(he4_09_hook)
    END_IF()
  RETURN_FROM_CALL()

  EVT_BEGIN(he2_hook)
    USER_FUNC(genocide_check, 16)
  RETURN_FROM_CALL()

  EVT_BEGIN(he4_hook)
    USER_FUNC(genocide_check, 40)
  RETURN_FROM_CALL()

  EVT_BEGIN(savemgr_evt)
    USER_FUNC(restore_fp)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(fp_restore), 0, 0)
  RETURN_FROM_CALL()

  EVT_BEGIN(he2_03_hook)
    IF_EQUAL(GSWF(1802), 1)
      USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A2D_hasi_2"), 1, 1)
      USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A2D_hasi_1"), 1, 1)
      USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A3D_hasi_1"), 1, 1)
      USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("hasi_1"), 1)
      USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("hasi_1_01"), 1)
      USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("hasi_2"), 1)
      USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("hasi_2_01"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mise"))
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mura_1"))
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mura_2"))
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mura_3"))
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mura_4"))
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mura_5"))
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("son"))
      USER_FUNC(reduce_game_speed)
    END_IF()
  RETURN_FROM_CALL()

  EVT_BEGIN(he2_03_red_green_hook)
    IF_EQUAL(GSWF(1802), 1)
      USER_FUNC(reduce_game_speed)
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("mon"))
    END_IF()
  RETURN_FROM_CALL()

  EVT_BEGIN(he2_03_red_green_hook_us)
    IF_EQUAL(GSWF(1802), 1)
      USER_FUNC(reduce_game_speed)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("mon"), FLOAT(0.0), FLOAT(-100.0), FLOAT(0.0))
    END_IF()
  RETURN_FROM_CALL()

  EVT_BEGIN(he2_03_shop_hook)
    IF_EQUAL(GSWF(1802), 1)
      RETURN()
    END_IF()
  RETURN_FROM_CALL()

void remove_shop_door()
{
  spm::map_data::MapData * he2_md = spm::map_data::mapDataPtr("he2_03");
  spm::evt_door::DoorDesc* desc = (spm::evt_door::DoorDesc*)getInstructionEvtArg(he2_md->initScript, 93, 1);
  evtpatch::hookEvt(desc->unkScript1, 1, const_cast<spm::evtmgr::EvtScriptCode*>(he2_03_shop_hook));
  return;
}

  void savemgr_main()
  {
    evt_read_setup_file = patch::hookFunction(spm::evt_npc::evt_read_setup_file, new_evt_read_setup_file);
    evtpatch::hookEvt((spm::evtmgr::EvtScriptCode*)evt_memcard_save, 4, (spm::evtmgr::EvtScriptCode*)savemgr_evt);
    spm::map_data::MapData * he2_md = spm::map_data::mapDataPtr("he2_01");
    evtpatch::hookEvt(he2_md->initScript, 45, const_cast<spm::evtmgr::EvtScriptCode*>(he2_hook));
    he2_md = spm::map_data::mapDataPtr("he2_03");
    evtpatch::hookEvt(he2_md->initScript, 136, const_cast<spm::evtmgr::EvtScriptCode*>(he2_03_hook));
    he2_md = spm::map_data::mapDataPtr("he2_04");
    evtpatch::hookEvt(he2_md->initScript, 44, const_cast<spm::evtmgr::EvtScriptCode*>(he2_03_red_green_hook_us));
    he2_md = spm::map_data::mapDataPtr("he2_05");
    evtpatch::hookEvt(he2_md->initScript, 23, const_cast<spm::evtmgr::EvtScriptCode*>(he2_03_red_green_hook));
    spm::map_data::MapData * he3_md = spm::map_data::mapDataPtr("he3_08");
    evtpatch::hookEvt(he3_md->initScript, 2, const_cast<spm::evtmgr::EvtScriptCode*>(he3_08_hook_fwd));
    spm::map_data::MapData * he4_md = spm::map_data::mapDataPtr("he4_01");
    evtpatch::hookEvt(he4_md->initScript, 2, const_cast<spm::evtmgr::EvtScriptCode*>(he4_hook));
    he4_md = spm::map_data::mapDataPtr("he4_09");
    evtpatch::hookEvt(he4_md->initScript, 2, const_cast<spm::evtmgr::EvtScriptCode*>(he4_09_hook_fwd));
    remove_shop_door();
    return;
  }

}
