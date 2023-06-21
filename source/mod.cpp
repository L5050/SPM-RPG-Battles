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
extern "C" {

  char marioString[] = "Flip";
  char peachString[] = "Heal";
  char bowserString[] = "Flame";
  char luigiString[] = "Super jump";

  char* characterStrings[] = {
    marioString,
    peachString,
    bowserString,
    luigiString
  };



void chooseNewCharacterString();
asm
(
    ".global chooseNewCharacterString\n"
    "chooseNewCharacterString:\n"
        "lis 4, characterStrings@ha\n"
        "ori 4, 4, characterStrings@l\n"
        "lwzx 3, 4, 0\n" //load new string pointer
        "blr\n"
);

}

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

spm::evtmgr::EvtEntry * (*evtEntry1)(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags);
spm::evtmgr::EvtEntry * (*evtChildEntry)(spm::evtmgr::EvtEntry * entry, const spm::evtmgr::EvtScriptCode * script, u8 flags);
spm::evtmgr::EvtEntry * (*evtBrotherEntry)(spm::evtmgr::EvtEntry * brother, const spm::evtmgr::EvtScriptCode * script, u8 flags);
spm::evtmgr::EvtEntry * (*evtEntryType)(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags, u8 type);

spm::evtmgr::EvtEntry * newEvtEntry(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags) {
  spm::evtmgr::EvtEntry * entry;
  //wii::os::OSReport("%x %x\n", &spm::iValues::theParentOfBeginRPG, &script);
  if (script == &spm::iValues::theParentOfBeginRPG) {
    wii::os::OSReport("evtEntry\n");
    entry = evtEntry1(parentOfBeginRPG, priority, flags);
  } else {
    entry = evtEntry1(script, priority, flags);}
    return entry;
}

spm::evtmgr::EvtEntry * newEvtChildEntry(spm::evtmgr::EvtEntry * entry, const spm::evtmgr::EvtScriptCode * script, u8 flags){
  spm::evtmgr::EvtEntry * entry1;
    if (script == &spm::iValues::theParentOfBeginRPG) {
    wii::os::OSReport("evtChildEntry\n");
      entry1 = evtChildEntry(entry, parentOfBeginRPG, flags);
    } else {
  entry1 = evtChildEntry(entry, script, flags);}
  return entry1;
}

spm::evtmgr::EvtEntry * newEvtBrotherEntry(spm::evtmgr::EvtEntry * brother, const spm::evtmgr::EvtScriptCode * script, u8 flags){
  spm::evtmgr::EvtEntry * entry;
    if (script == &spm::iValues::theParentOfBeginRPG) {
    wii::os::OSReport("evtBrotherEntry\n");
      entry = evtBrotherEntry(brother, parentOfBeginRPG, flags);
    } else {
  entry = evtBrotherEntry(brother, script, flags);}
  return entry;
}

spm::evtmgr::EvtEntry * newEvtEntryType(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags, u8 type) {
  spm::evtmgr::EvtEntry * entry;
  if (script == &spm::iValues::theParentOfBeginRPG) {
    wii::os::OSReport("evtEntryType\n");
    entry = evtEntryType(parentOfBeginRPG, priority, flags, type);
  }
  entry = evtEntryType(script, priority, flags, type);
  return entry;
}

void hookEvent() {
  evtEntry1 = patch::hookFunction(spm::evtmgr::evtEntry, newEvtEntry);

  evtChildEntry = patch::hookFunction(spm::evtmgr::evtChildEntry, newEvtChildEntry);

  evtBrotherEntry = patch::hookFunction(spm::evtmgr::evtBrotherEntry, newEvtBrotherEntry);

  evtEntryType = patch::hookFunction(spm::evtmgr::evtEntryType, newEvtEntryType);

  writeBranchLink(&spm::iValues::techtext1, 0, chooseNewCharacterString);
}

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


void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
    hookEvent();
}

}
