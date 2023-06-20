#include "mod.h"
#include "patch.h"
#include "scripting.cpp"

#include <spm/iValues.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/npcdrv.h>
#include <spm/camdrv.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>

namespace mod {
/*
    Title Screen Custom Text
    Prints "SPM RPG Battles" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor green = {0, 255, 0, 255};
    f32 scale = 0.8f;
    const char * msg = "SPM RPG Battles";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0f, msg);
    seq_titleMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
}

/*
    General mod functions
*/

s32 npcEntryFromTribeId(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::npcdrv::NPCWork * npcWork = spm::npcdrv::npcGetWorkPtr();
  spm::evtmgr::EvtVar * evtVariables = evtEntry->pCurData;
  s32 id = spm::evtmgr_cmd::evtGetValue(evtEntry, *evtVariables);
  for (int i = 0; i < 535; i++) {
    if (npcWork->entries[i].tribeId == id) {
      spm::evtmgr_cmd::evtSetValue(evtEntry, evtEntry->lw[0], (s32)npcWork->entries[i].name);
    }
  }
  return 2;
}

void patchScripts() {
spm::iValues::theParentOfBeginRPG = parentOfBeginRPG;
}

void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
    patchScripts();
}

}
