#include "mod.h"
#include "patch.h"
#include "scripting.cpp"

#include <spm/spmario_snd.h>
#include <spm/evtmgr.h>
#include <spm/mario.h>
#include <spm/evtmgr_cmd.h>
#include <spm/msgdrv.h>
#include <spm/seq_mapchange.h>
#include <spm/effdrv.h>
#include <spm/eff_nice.h>
#include <spm/animdrv.h>
#include <spm/npcdrv.h>
#include <spm/camdrv.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <spm/rel/an2_08.h>
#include <spm/rel/an.h>
#include <spm/rel/sp4_13.h>
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
bool loadedStage7 = false;
const char rpgStart[] = {"Flint Cragley Attacks!<o>"};

/*
    Title Screen Custom Text
    Prints "SPM RPG Battles" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static spm::seqdef::SeqFunc *seq_gameMainReal;
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

static void seq_gameMainOverride(spm::seqdrv::SeqWork *wp)
{
  seq_gameMainReal(wp);
}

static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
    seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &seq_gameMainOverride;
}

/*
    General mod functions
*/

spm::evtmgr::EvtEntry * (*evtEntry1)(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags);
spm::evtmgr::EvtEntry * (*evtChildEntry)(spm::evtmgr::EvtEntry * entry, const spm::evtmgr::EvtScriptCode * script, u8 flags);
spm::evtmgr::EvtEntry * (*evtBrotherEntry)(spm::evtmgr::EvtEntry * brother, const spm::evtmgr::EvtScriptCode * script, u8 flags);
spm::evtmgr::EvtEntry * (*evtEntryType)(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags, u8 type);
spm::effdrv::EffEntry * (*effNiceEntry)(double param_1, double param_2, double param_3, double param_4, int param_5);
bool (*spsndBGMOn)(u32 flags, const char * name);
bool (*spsndSFXOn)(const char * name);
s32 (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
s32 (*evt_inline_evt)(spm::evtmgr::EvtEntry * entry);
void (*msgUnLoad)(s32 slot);
const char * (*msgSearch)(const char * msgName);

const char * newMsgSearch(const char * msgName) {

  const char ogRpgStart[] = {"stg7_2_133_2_001"};

  if (*msgName == *ogRpgStart) {
    return rpgStart;
  } else {
    return msgSearch(msgName);
  }

}

spm::evtmgr::EvtEntry * newEvtEntry(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags) {
  spm::evtmgr::EvtEntry * entry;
  wii::os::OSReport("%x\n", script);
  if (script == spm::an2_08::begin_rpg_parent_evt) {
    wii::os::OSReport("evtEntry\n");
    entry = evtEntry1(parentOfBeginRPG, priority, flags);
  } else {
    entry = evtEntry1(script, priority, flags);}
    return entry;
}

spm::evtmgr::EvtEntry * newEvtChildEntry(spm::evtmgr::EvtEntry * entry, const spm::evtmgr::EvtScriptCode * script, u8 flags){
  spm::evtmgr::EvtEntry * entry1;
    if (script == spm::sp4_13::brobot_appear) {
    wii::os::OSReport("evtChildEntry\n");
    wii::os::OSReport("%x\n", entry->scriptStart);
      entry1 = evtChildEntry(entry, spm::sp4_13::brobot_appear, flags);
    } else {
  entry1 = evtChildEntry(entry, script, flags);}
  return entry1;
}

spm::evtmgr::EvtEntry * newEvtBrotherEntry(spm::evtmgr::EvtEntry * brother, const spm::evtmgr::EvtScriptCode * script, u8 flags){
  spm::evtmgr::EvtEntry * entry;
    if (script == spm::an2_08::begin_rpg_parent_evt) {
    wii::os::OSReport("evtBrotherEntry\n");
      entry = evtBrotherEntry(brother, parentOfBeginRPG, flags);
    } else {
  entry = evtBrotherEntry(brother, script, flags);}
  return entry;
}

spm::evtmgr::EvtEntry * newEvtEntryType(const spm::evtmgr::EvtScriptCode * script, u32 priority, u8 flags, u8 type) {
  spm::evtmgr::EvtEntry * entry;
  if (script == spm::an2_08::begin_rpg_parent_evt) {
    wii::os::OSReport("evtEntryType\n");
    entry = evtEntryType(parentOfBeginRPG, priority, flags, type);
  } else {
  entry = evtEntryType(script, priority, flags, type);}
  return entry;
}

s32 new_evt_inline_evt(spm::evtmgr::EvtEntry * entry) {
  wii::os::OSReport("%x\n", entry->scriptStart);
  return evt_inline_evt(entry);
}

spm::effdrv::EffEntry * newEffNiceEntry(double param_1, double param_2, double param_3, double param_4, int param_5) {

    wii::os::OSReport("%d %d %d %d %d\n", param_1, param_2, param_3, param_4, param_5);
    return effNiceEntry(param_1, param_2, param_3, param_4, param_5);

}

s32 newMarioCalcDamageToEnemy(s32 damageType, s32 tribeId) {
  //spm::effdrv::EffEntry * effentry = effNiceEntry(1, 0, -2139062144, 1600222564, 1601071459);

  if (rpgInProgress == false){
  rpgTribeID[1] = tribeId;
  wii::os::OSReport("%d %d\n", tribeId, rpgTribeID[0]);
  spm::evtmgr::evtEntry(parentOfBeginRPG, 1, 0);
  rpgInProgress = true;
  return 100;
} else {
  return marioCalcDamageToEnemy(damageType, tribeId);
}
}

void newMsgUnload(s32 slot) {
  if (slot != 7) {
    msgUnLoad(slot);
  }
}

bool new_spsndBGMOn(u32 flags, const char * name) {

  wii::os::OSReport("%s\n", name);
  return spsndBGMOn(flags, name);

}

bool new_spsndSFXOn(const char * name) {

  wii::os::OSReport("%s\n", name);
  return spsndSFXOn(name);

}

void nopTPL() {
  writeWord(&spm::an2_08::rpg_screen_draw, 0x204, 0x60000000);
  writeWord(&spm::an2_08::rpg_screen_draw, 0x208, 0x60000000);
  writeWord(&spm::an2_08::rpg_screen_draw, 0x210, 0x60000000);
  writeWord(&spm::an2_08::rpg_screen_draw, 0x310, 0x60000000);
}

void hookEvent() {
  evtEntry1 = patch::hookFunction(spm::evtmgr::evtEntry, newEvtEntry);

  evtChildEntry = patch::hookFunction(spm::evtmgr::evtChildEntry, newEvtChildEntry);

  evtBrotherEntry = patch::hookFunction(spm::evtmgr::evtBrotherEntry, newEvtBrotherEntry);

  evtEntryType = patch::hookFunction(spm::evtmgr::evtEntryType, newEvtEntryType);

  //evt_inline_evt = patch::hookFunction(spm::evtmgr_cmd::evt_inline_evt, new_evt_inline_evt);

  effNiceEntry = patch::hookFunction(spm::eff_nice::effNiceEntry, newEffNiceEntry);

  marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy, newMarioCalcDamageToEnemy);

  //spsndBGMOn = patch::hookFunction(spm::spmario_snd::spsndBGMOn, new_spsndBGMOn);

  spsndSFXOn = patch::hookFunction(spm::spmario_snd::spsndSFXOn, new_spsndSFXOn);

  msgUnLoad = patch::hookFunction(spm::msgdrv::msgUnLoad, newMsgUnload);

//  msgSearch = patch::hookFunction(spm::msgdrv::msgSearch, newMsgSearch);
  //nopTPL();
  writeBranchLink(&spm::an2_08::rpgHandleMenu, 0x1BC, chooseNewCharacterString);
  writeBranchLink(&spm::an2_08::evt_rpg_calc_damage_to_enemy, 0x44, getTribe);
  writeBranchLink(&spm::an2_08::evt_rpg_npctribe_handle, 0x94, getTribe2);
  writeWord(&spm::an2_08::evt_rpg_npctribe_handle, 0xA0, 0x3B9C0004);
  writeWord(&spm::an2_08::evt_rpg_npctribe_handle, 0x8C, 0x3BA00018);
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
      if (npcWork->entries[i].tribeId == rpgTribeID[1]) {
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
