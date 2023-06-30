#include "mod.h"
#include "patch.h"
#include "scripting.cpp"

#include <spm/iValues.h>
#include <spm/evtmgr.h>
#include <spm/mario.h>
#include <spm/evtmgr_cmd.h>
#include <spm/effdrv.h>
#include <spm/animdrv.h>
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

s32 rpgTribeID[3] = {0, 0, 0};

void getTribe();
asm
(
  ".global getTribe\n"
    "getTribe:\n"
    "lis 6, rpgTribeID@h\n"
    "ori 6, 6, rpgTribeID@l\n"
    "slwi 4, 4, 2\n"
    "lwzx 4, 6, 4\n"
    "blr\n"
);

void getTribe2();
asm
(
  ".global getTribe2\n"
    "getTribe2:\n"
    "lis 7, rpgTribeID@h\n"
    "ori 7, 7, rpgTribeID@l\n"
    "mr 3, 28\n"
    "slwi 3, 3, 2\n"
    "lwzx 3, 7, 3\n"
    "blr\n"
);

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
bool rpgInProgress = false;
bool bossFight = false;
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
void (*marioTakeDamage)(wii::mtx::Vec3 * position, u32 flags, s32 damage);
s32 (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
spm::effdrv::EffEntry * (*effNiceEntry)(double param_1, double param_2, double param_3, double param_4, int param_5);
//s32 (*evt_inline_evt)(spm::evtmgr::EvtEntry * entry);

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
  } else {
  entry = evtEntryType(script, priority, flags, type);}
  return entry;
}

/*s32 new_evt_inline_evt(spm::evtmgr::EvtEntry * entry) {
  wii::os::OSReport("%x\n", entry->scriptStart);
  return evt_inline_evt(entry);
}*/

spm::effdrv::EffEntry * newEffNiceEntry(double param_1, double param_2, double param_3, double param_4, int param_5) {

    wii::os::OSReport("%d %d %d %d %d\n", param_1, param_2, param_3, param_4, param_5);
    return effNiceEntry(param_1, param_2, param_3, param_4, param_5);

}

s32 newMarioCalcDamageToEnemy(s32 damageType, s32 tribeId) {
  //spm::effdrv::EffEntry * effentry = effNiceEntry(1, 0, -2139062144, 1600222564, 1601071459);

  if (rpgInProgress == false){
  rpgTribeID[0] = tribeId;
  wii::os::OSReport("%d %d\n", tribeId, rpgTribeID[0]);
  spm::evtmgr::evtEntry(parentOfBeginRPG, 1, 0);
  rpgInProgress = true;
  return 100;
} else {
  return marioCalcDamageToEnemy(damageType, tribeId);
}
}

void hookEvent() {
  evtEntry1 = patch::hookFunction(spm::evtmgr::evtEntry, newEvtEntry);

  evtChildEntry = patch::hookFunction(spm::evtmgr::evtChildEntry, newEvtChildEntry);

  evtBrotherEntry = patch::hookFunction(spm::evtmgr::evtBrotherEntry, newEvtBrotherEntry);

  evtEntryType = patch::hookFunction(spm::evtmgr::evtEntryType, newEvtEntryType);

  //evt_inline_evt = patch::hookFunction(spm::evtmgr_cmd::evt_inline_evt, new_evt_inline_evt);

  effNiceEntry = patch::hookFunction(spm::effdrv::effNiceEntry, newEffNiceEntry);

  marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy, newMarioCalcDamageToEnemy);

  marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
    [](wii::mtx::Vec3 * position, u32 flags, s32 damage)
            {
          //spm::effdrv::EffEntry * effentry = effNiceEntry(1, 0, -2139062144, 1600222564, 1601071459);
              marioTakeDamage(position, flags, damage);
            });

  writeBranchLink(&spm::iValues::techtext1, 0, chooseNewCharacterString);
  writeBranchLink(&spm::iValues::rpgTribePatch1, 0, getTribe);
  writeBranchLink(&spm::iValues::rpgTribePatch2, 0, getTribe2);
  writeWord(&spm::iValues::underchompRepeatPatch, 0, 0x3B9C0004);
}

s32 npcEntryFromTribeId(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::npcdrv::NPCWork * npcWork = spm::npcdrv::npcGetWorkPtr();
  spm::evtmgr::EvtVar * evtVariables = evtEntry->pCurData;
  s32 id = spm::evtmgr_cmd::evtGetValue(evtEntry, *evtVariables);
  for (int i = 0; i < 535; i++) {
    if (npcWork->entries[i].tribeId == id) {
      spm::evtmgr_cmd::evtSetValue(evtEntry, evtEntry->lw[0], (s32)npcWork->entries[i].name);
    }
  } if (firstRun ==  false){}
  return 2;
}

s32 getRpgNpc(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
spm::npcdrv::NPCWork * npcWork = spm::npcdrv::npcGetWorkPtr();
 if (bossFight == false) {
    for (int i = 0; i < 525; i++) {
      if (npcWork->entries[i].tribeId == rpgTribeID[0]) {
        spm::evtmgr_cmd::evtSetValue(evtEntry, evtEntry->lw[0], (s32)npcWork->entries[i].name);
        break;
      }
    }
 } if (firstRun ==  false){}
 return 2;
}

void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
    hookEvent();
}

}
