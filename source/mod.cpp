#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "npc_rpgdrv.h"
#include "main_scripting.h"

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
#include <spm/eff_nice.h>
#include <spm/eff_spm_hit.h>
#include <spm/dispdrv.h>
#include <spm/animdrv.h>
#include <spm/npcdrv.h>
#include <spm/fontmgr.h>
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
#include <spm/rel/an2_08.h>
#include <spm/rel/an.h>
#include <spm/rel/sp4_13.h>
#include <cstdio>
USING(wii::mtx::Vec3)

//credit to rainchus for helping me out with these ASM hooks :)
extern "C" {
  char marioString[] = "Flip";
  char peachString[] = "Heal";
  char bowserString[] = "Flame";
  char luigiString[] = "Super Jump";

  char * characterStrings[] = {
    marioString,
    peachString,
    bowserString,
    luigiString
  };

  s32 rpgTribeID[3] = {
    0,
    0,
    0
  };

  bool rpgIsActive[3] = {
    false,
    false,
    false
  };

  void drawStuff() {
    // Flower disp
    const Vec3 fpVec = {-100.0, -202.0, 0.0};
    spm::icondrv::iconDispGx(0.7, &fpVec, 4, 105);
    // FP number disp
    wii::gx::GXColor white = {
      255,
      255,
      255,
      255
    };
    f32 scale = 1.0;
    char buffer [50];
    sprintf(buffer, "%d", mod::fp);
    const char * msg = buffer;
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor( & white);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x+-70, -172.0, msg);
    return;
  }

  s32 returnTribe(s32 index) {
    asm("mr 3, 28");
    return rpgTribeID[index];
  }

char * returnCharacterTechnique() {
  return characterStrings[spm::mario::marioGetPtr()->character];
}

  void setNewFloat();
  asm
  (
    "floatValue:\n"
    ".float 299\n"

    ".global setNewFloat\n"
    "setNewFloat:\n"
        "lis 3, floatValue@ha\n"
        "ori 3, 3, floatValue@l\n"
        "lfs 1, 0x0000 (3)\n"
        "blr\n"
  );

}

namespace mod {
  bool rpgInProgress = false;
  bool bossFight = false;
  bool screenOn = true;
  bool succeededActionCommand = false;
  bool superGuard = false;
  spm::npcdrv::NPCEntryUnkDef turnBasedCombatOverride[2];
  s32 fp = 0;
  wii::tpl::TPLHeader *myTplHeader = nullptr;

  /*
      Title Screen Custom Text
      Prints "Brobot After Some Windex" at the top of the title screen
  */

  static spm::seqdef::SeqFunc * seq_titleMainReal;
  static spm::seqdef::SeqFunc * seq_gameMainReal;
  static void seq_titleMainOverride(spm::seqdrv::SeqWork * wp) {
    wii::gx::GXColor green = {
      0,
      255,
      0,
      255
    };
    f32 scale = 0.8;
    const char * msg = "Brobot After Some Windex";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor( & green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0, msg);
    seq_titleMainReal(wp);
  }

    static void seq_gameMainOverride(spm::seqdrv::SeqWork * wp) {
    if (rpgInProgress == true) {
      wii::gx::GXColor green = {
        0,
        255,
        0,
        255
      };
      f32 scale = 0.8;
      char buffer [50];
      sprintf(buffer, "%d", fp);
      const char * msg = buffer;
      spm::fontmgr::FontDrawStart();
      spm::fontmgr::FontDrawEdge();
      spm::fontmgr::FontDrawColor( & green);
      spm::fontmgr::FontDrawScale(scale);
      spm::fontmgr::FontDrawNoiseOff();
      spm::fontmgr::FontDrawRainbowColor();
      f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
      spm::fontmgr::FontDrawString(x+-50, -161.0, msg);
      }
      /*
      if (rpgInProgress == true) {
        wii::gx::GXColor green = {
          0,
          255,
          0,
          255
        };
        f32 scale = 0.8;
        const char * msg = "FP:";
        spm::fontmgr::FontDrawStart();
        spm::fontmgr::FontDrawEdge();
        spm::fontmgr::FontDrawColor( & green);
        spm::fontmgr::FontDrawScale(scale);
        spm::fontmgr::FontDrawNoiseOff();
        spm::fontmgr::FontDrawRainbowColorOff();
        f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
        spm::fontmgr::FontDrawString(x+-90, -161.0, msg);
      }*/
      seq_gameMainReal(wp);
    }

  static void titleScreenCustomTextPatch() {
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = & seq_titleMainOverride;
    //seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    //spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = & seq_gameMainOverride;
  }

  /*
      Various hooks
  */
  spm::effdrv::EffEntry * ( * effNiceEntry)(double param_1, double param_2, double param_3, double param_4, int param_5);
  bool( * spsndBGMOn)(u32 flags,
    const char * name);
  s32( * spsndSFXOn)(const char * name);
  s32( * marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
  s32( * evt_inline_evt)(spm::evtmgr::EvtEntry * entry);
  void( * msgUnLoad)(s32 slot);
  void( * rpg_screen_draw)();
  const char * ( * msgSearch)(const char * msgName);

  const char fileName[] = {
    "stg7"
  };

  spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
  {
    spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
    wii::os::OSReport("%p\n", link);
    spm::evtmgr::EvtScriptCode* arg = evtpatch::getInstructionArgv(link)[instruction];
    wii::os::OSReport("%p\n", arg);
    return arg;
  }

  /*
      Custom Text
  */

static const char * getNpcName(s32 tribeId) {
  switch(tribeId) {
    case 0:
      return "Goomba";
    break;
    case 125:
      return "Squiglet";
    break;
    case 529:
      return "Doopliss";
    break;
    case 309:
      return "Super Dimentio";
    break;
    default:
    return "yeet";
  }
}

s32 getRpgTribeID(s32 index) {
  return rpgTribeID[index];
}

bool IsNpcActive(s32 index) {
  return rpgIsActive[index];
}

  const char * rpgStart = "Prepare for battle!\n"
  "<o>\n";
  const char * stg7_2_133_2_002 = "<dq>\n"
  "<p>\n"
  "What will you do?\n"
  "<o>\n";
  const char * stg7_2_133_2_003 = "<p>\n"
  "Attack who?\n"
  "<o>\n";
  const char * stg7_2_133_2_004 = "<p>\n"
  "%s attacks!\n"
  "<o>\n";
  const char * stg7_2_133_2_005 = "<p>\n"
  "You deal %d damage to\n"
  "%s!\n"
  "<k>\n"
  "<o>\n";
  const char * stg7_2_133_2_006 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But your attack is dodged!\n"
  "<k>\n"
  "<o>\n";
  const char * stg7_2_133_2_007 = "<p>\n"
  "You defeated\n"
  "%s!\n"
  "<k>\n"
  "<o>\n";
  const char * stg7_2_133_2_008 = "<p>\n"
  "You won!\n"
  "<k>\n"
  "<p>\n"
  "%s receives %d points!\n"
  "<k>\n"
  "<o>\n";
  const char * stg7_2_133_2_009 = "<p>\n"
  "You gained a level!\n"
  "<o>\n";
  const char * stg7_2_133_2_009_01 = "<p>\n"
  "Your attack power increased\n"
  "by %d!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_009_02 = "<p>\n"
  "Your HP increased by %d!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_010 = "<p>\n"
  "Choose a technique.\n"
  "<o>\n";

  const char * stg7_2_133_2_011 = "<p>\n"
  "Mario flips!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_012 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "...You found 2 FP!\n"
  "<k>\n"
  "<o>\n";

  const char * no_fp = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "no FP?\n"
  "*Megamind gif*"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_013 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_014 = "<p>\n"
  "Bowser breathes fire!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_015 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "...But the flames have no effect on %s!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_016 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_017 = "<p>\n"
  "Luigi glares at %s\n"
  "and launches a super jump!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_018 = "<p>\n"
  "You stomp %s for %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_019 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But the attack misses!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_020 = "<p>\n"
  "Select a Pixl.\n"
  "<o>\n";

  const char * stg7_2_133_2_021 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_022 = "<p>\n"
  "%s throws Thoreau at %s!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_023 = "<dkey><wait 500></dkey>\n"
  "<p>\n"
  "Urg... %s is too heavy to lift!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_024 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_025 = "<p>\n"
  "Take that, enemy!\n"
  "<wait 150>%s throws Boomer!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_026 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "Miss! The attack is a failure!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_027 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "Boomer explodes!\n"
  "<wait 150>%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_028 = "<p>\n"
  "%s uses Slim!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_029 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s turns sideways\n"
  "and is hard to see!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_031 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_032 = "<p>\n"
  "%s uses Thudley!\n"
  "A heavy-duty attack!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_033 = "<p>\n"
  "The attack is a success!\n"
  "%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_034 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But the attack is evaded!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_035 = "<p>\n"
  "%s hops onto Carrie!\n"
  "<dkey><wait 500></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_036 = "<p>\n"
  "You feel a little taller!\n"
  "<wait 450>...But that is the only\n"
  "effect, unfortunately.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_037 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_038 = "<p>\n"
  "%s uses Fleep on %s!\n"
  "<o>\n";

  const char * stg7_2_133_2_039 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s looks dizzy!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_040 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s avoids Fleep's effect!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_042 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But %s is asleep! It doesn't work!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_043 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_044 = "<p>\n"
  "%s uses Cudge\n"
  "to attack %s!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_045 = "<p>\n"
  "Direct hit!\n"
  "<wait 150>%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_046 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "So close!\n"
  "<wait 150>%s barely evades Cudge!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_047 = "<p>\n"
  "%s uses Dottie!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_048 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s gets tiny!\n"
  "Where did he go?!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_050 = "<p>\n"
  "%s uses Piccolo!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_051 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "Piccolo plays a mysterious song!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_052 = "<p>\n"
  "%s falls into a deep sleep!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_053 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_054 = "<p>\n"
  "Spinning destruction!\n"
  "<wait 150>Barry slams powerfully into %s!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_055 = "<p>\n"
  "Nice hit! <wait 150>%s\n"
  "takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_056 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But the attack is evaded!\n"
  "<wait 150>\n"
  "Your party is frustrated!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_056_01 = "<p>\n"
  "%s uses Dashell!\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_056_02 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s dashes quickly!\n"
  "<wait 450>\n"
  "...But nothing else happens.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_057 = "<p>\n"
  "Use which item?\n"
  "<o>\n";

  const char * stg7_2_133_2_058 = "<p>\n"
  "You don't have any items!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_059 = "<p>\n"
  "You can't use that right now!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_060 = "<p>\n"
  "Use it on who?\n"
  "<o>\n";

  const char * stg7_2_133_2_061 = "<p>\n"
  "%s uses\n"
  "<AN_ITEM>!\n"
  "<dkey><wait 500></dkey>\n"
  "<o>\n";

  const char * stg7_2_133_2_062 = "<p>\n"
  "But nothing happens.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_063 = "<p>\n"
  "%s recovers %d HP!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_064 = "<p>\n"
  "%s recovers from\n"
  "poison!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_065 = "<p>\n"
  "The curse on %s is\n"
  "lifted!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_066 = "<p>\n"
  "Your score increased by\n"
  "%d!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_067 = "<p>\n"
  "%s is poisoned!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_068 = "<p>\n"
  "%s's attack power\n"
  "increases!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_069 = "<p>\n"
  "%s starts to gradually\n"
  "recover HP!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_070 = "<p>\n"
  "%s gains the power to\n"
  "shock enemies on contact!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_071 = "<p>\n"
  "%s's defense increases!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_072 = "<p>\n"
  "%s is invincible\nto all attacks!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_073 = "<p>\n"
  "...The flames have no effect\n"
  "on %s!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_074 = "<p>\n"
  "%s takes\n"
  "%d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_075 = "<p>\n"
  "%s takes\n"
  "%d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_076 = "<p>\n"
  "%s takes\n"
  "%d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_077 = "<p>\n"
  "%s takes\n"
  "%d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_078 = "<p>\n"
  "A shell appears!\n"
  "<wait 150>\n"
  "%s kicks the shell!\n"
  "<dkey><wait 250></dkey>\n";

  const char * stg7_2_133_2_079 = "<p>\n"
  "%s takes\n"
  "%d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_080 = "<p>\n"
  "%s summons a Ghost\n"
  "Shroom! <wait 150>The Ghost Shroom\n"
  "attacks!\n"
  "<dkey><wait 250></dkey>\n";

  const char * stg7_2_133_2_081 = "<p>\n"
  "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But the Ghost Shroom's\n"
  "attacks don't reach!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_082 = "<p>\n"
  "%s falls\n"
  "into a deep sleep.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_083 = "<p>\n"
  "You got <AN_ITEM>!\n"
  "<wait 250>\n"
  "%s uses <AN_ITEM>!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_084 = "<p>\n"
  "%s's Max HP increases\n"
  "by 5!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085 = "<p>\n"
  "%s's Attack increases\n"
  "by 1!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085_01 = "<p>\n"
  "%s is frozen\n"
  "solid!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085_02 = "<p>\n"
  "%s is cursed!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085_03 = "<p>\n"
  "%s's movements are\n"
  "slowed!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085_04 = "<p>\n"
  "%s is tech-cursed!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085_05 = "<p>\n"
  "%s is heavy-cursed!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_085_06 = "<p>\n"
  "%s is reverse-cursed!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_086 = "<p>\n"
  "Switch with who?\n"
  "<o>\n";

  const char * stg7_2_133_2_087 = "<p>\n"
  "Tippi says, \"Go, Mario!\"\n"
  "<wait 250>\n"
  "%s switches places\n"
  "with Mario!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_088 = "<p>\n"
  "Bowser says, \"Stomping time!\"\n"
  "<wait 250>\n"
  "%s switches places\n"
  "with Bowser!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_089 = "<p>\n"
  "Luigi says, \"I'm on the job!\"\n"
  "<wait 250>\n"
  "%s switches places\n"
  "with Luigi!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_090 = "<p>\n"
  "%s runs away!\n"
  "<o>\n";
  const char * stg7_2_133_2_091 = "<p>\n"
  "<dkey>.<wait 250>.<wait 250>.<wait 250></dkey>A successful escape!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_092 = "<p>\n"
  "<dkey>.<wait 250>.<wait 250>.<wait 250></dkey>But they got noticed!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_093 = "<p>\n"
  "%s\n"
  "shoots a missile!\n"
  "<wait 300>\n"
  "<o>\n";

  const char * stg7_2_133_2_094 = "<p>\n"
  "%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_095 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "But you're invincible!\n"
  "<wait 150>\n"
  "The attack is ineffective!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_096 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s dodges like a pro!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_097 = "<p>\n"
  "Brobot shoots out\n"
  "blazing fire!\n"
  "<wait 350>\n"
  "<o>\n";

  const char * stg7_2_133_2_098 = "<p>\n"
  "Youch! %s takes\n"
  "%d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_099 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s dodges like a pro!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_100 = "<p>\n"
  "Blue Underchomp breathes\nblue fire!\n"
  "<wait 350>\n"
  "<o>\n";

  const char * stg7_2_133_2_101 = "<p>\n"
  "Now that's hot!\n"
  "<wait 150>\n"
  "%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_102 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s dodges with style!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_103 = "<p>\n"
  "Yellow Underchomp expels\nstinky breath!\n"
  "<wait 350>\n"
  "<o>\n";

  const char * stg7_2_133_2_104 = "<p>\n"
  "Oh no! It's putrid!\n"
  "<wait 150>\n"
  "%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_105 = "<dkey><wait 250></dkey>\n"
  "<p>\n"
  "%s dodges the stink!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_106 = "<p>\n"
  "%s gathers\n"
  "strength!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_107 = "<p>\n"
  "Tippi says, \"Mario! Noooo!\"\n"
  "<wait 250>\n"
  "Mario falls in battle...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_108 = "<p>\n"
  "Bowser says, \"Not again!\"\n"
  "<wait 250>\n"
  "Bowser falls in battle...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_109 = "<p>\n"
  "Luigi says, \"Bro! Forgive me...\"\n"
  "<wait 250>\n"
  "Luigi falls in battle...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_110 = "<p>\n"
  "%s falls in battle...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_111 = "<p>\n"
  "But the Life Shroom restores\n"
  "5 HP!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_112 = "<p>\n"
  "%s is taking\n"
  "a nice nap...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_113 = "<p>\n"
  "%s is looking\n"
  "for %s...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_114 = "<p>\n"
  "%s is dizzy...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_115 = "<p>\n"
  "%s wakes up!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_116 = "<p>\n"
  "%s\n"
  "regains consciousness!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_117 = "<p>\n"
  "%s recovers 1 HP!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_118 = "<p>\n"
  "The ability to recover HP\n"
  "gradually wears off!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_119 = "<p>\n"
  "Yuuuuurk! Poison...\n"
  "<wait 150>\n"
  "%s takes %d damage!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_120 = "<p>\n"
  "%s recovers from\n"
  "poison!\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_121 = "<p>\n"
  "%s's attack power\n"
  "returns to normal.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_122 = "<p>\n"
  "%s loses the ability\n"
  "to shock enemies on contact.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_123 = "<p>\n"
  "%s's defense returns\n"
  "to normal.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124 = "<p>\n"
  "%s loses the power\n"
  "of invincibility.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124_01 = "<p>\n"
  "%s is frozen...\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124_02 = "<p>\n"
  "%s is freed from\n"
  "the ice.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124_03 = "<p>\n"
  "%s is now free\n"
  "from the curse.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124_04 = "<p>\n"
  "%s is now free\n"
  "from the tech curse.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124_05 = "<p>\n"
  "%s is now free\n"
  "from the heavy curse.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_124_06 = "<p>\n"
  "%s is now free\n"
  "from the reverse curse.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_125 = "<p>\n"
  "%s turns back\n"
  "frontways.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_126 = "<p>\n"
  "%s steps down off\n"
  "Carrie.\n"
  "<k>\n"
  "<o>\n";

  const char * stg7_2_133_2_127 = "<p>\n"
  "%s returns to normal\n"
  "size.\n"
  "<k>\n"
  "<o>\n";

  const char * brobot_toxic_serum = "<p>\n"
  "Brobot spews out\n"
  "a toxic serum!\n"
  "<wait 350>\n"
  "<o>\n";

  const char * peach_heal = "<p>\n"
  "Peach calls upon\n"
  "the pure hearts...\n"
  "<dkey><wait 250></dkey>\n"
  "<o>\n";

  const char * peach_heal_success = "<p>\n"
  "Success!\n"
  "Peach heals %d HP!\n"
  "<k>\n"
  "<o>\n";

  const char * grab_fp = "<p>\n"
  "Success!\n"
  "You stole 2 FP!\n"
  "<k>\n"
  "<o>\n";

  const char * peach_special = "Heal";

  const char wang_cmd_1[] = {
    "Attack"
  };
  const char wang_cmd_2[] = {
    "Technique"
  };
  const char wang_cmd_3[] = {
    "Pixl"
  };
  const char wang_cmd_4[] = {
    "Item"
  };
  const char wang_cmd_5[] = {
    "Switch"
  };
  const char wang_cmd_6[] = {
    "Escape"
  };
  const char wang_special_1[] = {
    "Flip"
  };
  const char wang_special_2[] = {
    "Flame"
  };
  const char wang_special_3[] = {
    "Super Jump"
  };
  const char wang_level[] = {
    "LV."
  };
  const char wang_hp[] = {
    "HP"
  };

  const char * testCharacterStrings[] = {
    wang_special_1,
    wang_special_2,
    wang_special_3,
    peach_special
  };

  const char * newMsgSearch(const char * msgName) {
    //wii::os::OSReport("%s\n", msgName);
    if (msl::string::strcmp(msgName, "stg7_2_133_2_001") == 0)
      //Override intro
      return rpgStart;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_002") == 0)
      //Replace message
      return stg7_2_133_2_002;
    else if (msl::string::strcmp(msgName, "peach_heal") == 0)
        //Replace message
      return peach_heal;
    else if (msl::string::strcmp(msgName, "peach_heal_success") == 0)
          //Replace message
      return peach_heal_success;
    else if (msl::string::strcmp(msgName, "wang_cmd_1") == 0)
      //Replace message
      return wang_cmd_1;
    else if (msl::string::strcmp(msgName, "wang_cmd_2") == 0)
      //Replace message
      return wang_cmd_2;
    else if (msl::string::strcmp(msgName, "wang_cmd_3") == 0)
      //Replace message
      return wang_cmd_3;
    else if (msl::string::strcmp(msgName, "wang_cmd_4") == 0)
      //Replace message
      return wang_cmd_4;
    else if (msl::string::strcmp(msgName, "wang_cmd_5") == 0)
      //Replace message
      return wang_cmd_5;
    else if (msl::string::strcmp(msgName, "wang_cmd_6") == 0)
      //Replace message
      return wang_cmd_6;
    else if (msl::string::strcmp(msgName, "wang_special_1") == 0)
      //Replace message
      return wang_special_1;
    else if (msl::string::strcmp(msgName, "wang_special_2") == 0)
      //Replace message
      return wang_special_2;
    else if (msl::string::strcmp(msgName, "wang_special_3") == 0)
      //Replace message
      return wang_special_3;
    else if (msl::string::strcmp(msgName, "peach_special") == 0)
        //Replace message
      return peach_special;
    else if (msl::string::strcmp(msgName, "wang_level") == 0)
      //Replace message
      return wang_level;
    else if (msl::string::strcmp(msgName, "wang_hp") == 0)
      //Replace message
      return wang_hp;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_008") == 0)
      //Replace message
      return stg7_2_133_2_008;
    else if (msl::string::strcmp(msgName, "wang_wang_r") == 0)
      //Replace message
      return getNpcName(rpgTribeID[0]);
    else if (msl::string::strcmp(msgName, "wang_wang_b") == 0)
      //Replace message
      return getNpcName(rpgTribeID[1]);
    else if (msl::string::strcmp(msgName, "wang_wang_y") == 0)
      //Replace message
      return getNpcName(rpgTribeID[2]);
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_003") == 0)
      //Replace message
      return stg7_2_133_2_003;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_004") == 0)
      //Replace message
      return stg7_2_133_2_004;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_005") == 0)
      //Replace message
      return stg7_2_133_2_005;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_006") == 0)
      //Replace message
      return stg7_2_133_2_006;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_007") == 0)
      //Replace message
      return stg7_2_133_2_007;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_009") == 0)
      //Replace message
      return stg7_2_133_2_009;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_009_01") == 0)
      return stg7_2_133_2_009_01;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_009_02") == 0)
      return stg7_2_133_2_009_02;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_010") == 0)
      return stg7_2_133_2_010;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_011") == 0)
      return stg7_2_133_2_011;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_012") == 0)
      return stg7_2_133_2_012;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_013") == 0)
      return stg7_2_133_2_013;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_014") == 0)
      return stg7_2_133_2_014;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_015") == 0)
      return stg7_2_133_2_015;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_016") == 0)
      return stg7_2_133_2_016;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_017") == 0)
      return stg7_2_133_2_017;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_018") == 0)
      return stg7_2_133_2_018;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_019") == 0)
      return stg7_2_133_2_019;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_020") == 0)
      return stg7_2_133_2_020;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_021") == 0)
      return stg7_2_133_2_021;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_022") == 0)
      return stg7_2_133_2_022;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_023") == 0)
      return stg7_2_133_2_023;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_024") == 0)
      return stg7_2_133_2_024;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_025") == 0)
      return stg7_2_133_2_025;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_026") == 0)
      return stg7_2_133_2_026;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_027") == 0)
      return stg7_2_133_2_027;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_028") == 0)
      return stg7_2_133_2_028;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_029") == 0)
      return stg7_2_133_2_029;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_031") == 0)
      return stg7_2_133_2_031;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_032") == 0)
      return stg7_2_133_2_032;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_033") == 0)
      return stg7_2_133_2_033;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_034") == 0)
      return stg7_2_133_2_034;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_035") == 0)
      return stg7_2_133_2_035;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_036") == 0)
      return stg7_2_133_2_036;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_037") == 0)
      return stg7_2_133_2_037;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_038") == 0)
      return stg7_2_133_2_038;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_039") == 0)
      return stg7_2_133_2_039;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_040") == 0)
      return stg7_2_133_2_040;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_042") == 0)
      return stg7_2_133_2_042;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_043") == 0)
      return stg7_2_133_2_043;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_044") == 0)
      return stg7_2_133_2_044;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_045") == 0)
      return stg7_2_133_2_045;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_046") == 0)
      return stg7_2_133_2_046;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_047") == 0)
      return stg7_2_133_2_047;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_048") == 0)
      return stg7_2_133_2_048;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_050") == 0)
      return stg7_2_133_2_050;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_051") == 0)
      return stg7_2_133_2_051;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_052") == 0)
      return stg7_2_133_2_052;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_053") == 0)
      return stg7_2_133_2_053;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_054") == 0)
      return stg7_2_133_2_054;

    else if (msl::string::strcmp(msgName, "stg7_2_133_2_055") == 0)
      return stg7_2_133_2_055;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_056") == 0)
      return stg7_2_133_2_056;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_056_01") == 0)
      return stg7_2_133_2_056_01;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_056_02") == 0)
      return stg7_2_133_2_056_02;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_057") == 0)
      return stg7_2_133_2_057;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_058") == 0)
      return stg7_2_133_2_058;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_059") == 0)
      return stg7_2_133_2_059;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_060") == 0)
      return stg7_2_133_2_060;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_061") == 0)
      return stg7_2_133_2_061;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_062") == 0)
      return stg7_2_133_2_062;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_063") == 0)
      return stg7_2_133_2_063;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_064") == 0)
      return stg7_2_133_2_064;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_065") == 0)
      return stg7_2_133_2_065;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_066") == 0)
      return stg7_2_133_2_066;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_067") == 0)
      return stg7_2_133_2_067;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_068") == 0)
      return stg7_2_133_2_068;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_069") == 0)
      return stg7_2_133_2_069;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_070") == 0)
      return stg7_2_133_2_070;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_071") == 0)
      return stg7_2_133_2_071;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_072") == 0)
      return stg7_2_133_2_072;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_073") == 0)
      return stg7_2_133_2_073;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_074") == 0)
      return stg7_2_133_2_074;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_075") == 0)
      return stg7_2_133_2_075;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_076") == 0)
      return stg7_2_133_2_076;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_077") == 0)
      return stg7_2_133_2_077;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_078") == 0)
      return stg7_2_133_2_078;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_079") == 0)
      return stg7_2_133_2_079;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_080") == 0)
      return stg7_2_133_2_080;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_081") == 0)
      return stg7_2_133_2_081;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_082") == 0)
      return stg7_2_133_2_082;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_083") == 0)
      return stg7_2_133_2_083;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_084") == 0)
      return stg7_2_133_2_084;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085") == 0)
      return stg7_2_133_2_085;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085_01") == 0)
      return stg7_2_133_2_085_01;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085_02") == 0)
      return stg7_2_133_2_085_02;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085_03") == 0)
      return stg7_2_133_2_085_03;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085_04") == 0)
      return stg7_2_133_2_085_04;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085_05") == 0)
      return stg7_2_133_2_085_05;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_085_06") == 0)
      return stg7_2_133_2_085_06;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_086") == 0)
      return stg7_2_133_2_086;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_087") == 0)
      return stg7_2_133_2_087;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_088") == 0)
      return stg7_2_133_2_088;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_089") == 0)
      return stg7_2_133_2_089;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_090") == 0)
      return stg7_2_133_2_090;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_092") == 0)
      return stg7_2_133_2_092;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_093") == 0)
      return stg7_2_133_2_093;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_094") == 0)
      return stg7_2_133_2_094;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_095") == 0)
      return stg7_2_133_2_095;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_096") == 0)
      return stg7_2_133_2_096;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_097") == 0)
      return stg7_2_133_2_097;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_098") == 0)
      return stg7_2_133_2_098;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_099") == 0)
      return stg7_2_133_2_099;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_100") == 0)
      return stg7_2_133_2_100;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_101") == 0)
      return stg7_2_133_2_101;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_102") == 0)
      return stg7_2_133_2_102;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_103") == 0)
      return stg7_2_133_2_103;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_104") == 0)
      return stg7_2_133_2_104;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_105") == 0)
      return stg7_2_133_2_105;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_106") == 0)
      return stg7_2_133_2_106;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_107") == 0)
      return stg7_2_133_2_107;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_108") == 0)
      return stg7_2_133_2_108;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_109") == 0)
      return stg7_2_133_2_109;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_110") == 0)
      return stg7_2_133_2_110;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_111") == 0)
      return stg7_2_133_2_111;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_112") == 0)
      return stg7_2_133_2_112;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_113") == 0)
      return stg7_2_133_2_113;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_114") == 0)
      return stg7_2_133_2_114;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_115") == 0)
      return stg7_2_133_2_115;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_116") == 0)
      return stg7_2_133_2_116;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_117") == 0)
      return stg7_2_133_2_117;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_118") == 0)
      return stg7_2_133_2_118;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_119") == 0)
      return stg7_2_133_2_119;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_120") == 0)
      return stg7_2_133_2_120;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_121") == 0)
      return stg7_2_133_2_121;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_122") == 0)
      return stg7_2_133_2_122;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_123") == 0)
      return stg7_2_133_2_123;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124") == 0)
      return stg7_2_133_2_124;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124_01") == 0)
      return stg7_2_133_2_124_01;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124_02") == 0)
      return stg7_2_133_2_124_02;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124_03") == 0)
      return stg7_2_133_2_124_03;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124_04") == 0)
      return stg7_2_133_2_124_04;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124_05") == 0)
      return stg7_2_133_2_124_05;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_124_06") == 0)
      return stg7_2_133_2_124_06;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_125") == 0)
      return stg7_2_133_2_125;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_126") == 0)
      return stg7_2_133_2_126;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_127") == 0)
      return stg7_2_133_2_127;
    else if (msl::string::strcmp(msgName, "stg7_2_133_2_091") == 0)
      return stg7_2_133_2_091;
    else if (msl::string::strcmp(msgName, "brobot_toxic_serum") == 0)
      return brobot_toxic_serum;
    else if (msl::string::strcmp(msgName, "grab_fp") == 0)
      return grab_fp;
    else if (msl::string::strcmp(msgName, "no_fp") == 0)
      return no_fp;
    else
    //wii::os::OSReport("%s\n", msgName);
      return msgSearch(msgName);

  }

  s32 compareStrings(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        char *mapName = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        wii::os::OSReport("%s name\n", mapName);
        char *comparison = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        int compstrReturn = 1;
        char *result = msl::string::strstr(mapName, comparison);
        if (result != 0)
        {
            compstrReturn = 1;
        } else
        {
            compstrReturn = 0;
        }
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], compstrReturn);
        return 2;
      }

  spm::effdrv::EffEntry * newEffNiceEntry(double param_1, double param_2, double param_3, double param_4, int param_5) {

    //wii::os::OSReport("%d %d %d %d %d\n", param_1, param_2, param_3, param_4, param_5);
    return effNiceEntry(param_1, param_2, param_3, param_4, param_5);

  }

  /*s32 newMarioCalcDamageToEnemy(s32 damageType, s32 tribeId) {
      wii::os::OSReport("%d, %d\n", damageType, tribeId);
      if (rpgInProgress != true) {
      rpgTribeID[1] = tribeId;
      spm::evtmgr::evtEntry(parentOfBeginRPG, 1, 0);
    }
      s32 damage = marioCalcDamageToEnemy(damageType, tribeId);
      if (tribeId == 295) damage = damage + 4;
      return damage;
  } */

  bool new_spsndBGMOn(u32 flags,
    const char * name) {

    wii::os::OSReport("%s\n", name);
    return spsndBGMOn(flags, name);

  }

  s32 new_spsndSFXOn(const char * name) {

    wii::os::OSReport("%s\n", name);
    return spsndSFXOn(name);

  }

  s32 new_evt_rpg_calc_damage_to_enemy(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 index = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
    s32 damageType = args[1];
    s32 damage = spm::mario::marioCalcDamageToEnemy(damageType, rpgTribeID[index]);
    if (rpgTribeID[1] == 296) damage = damage + 4;
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], damage);
    if (firstRun == false) {}
    return 2;
  }

  s32 new_evt_rpg_calc_mario_damage(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    
    s32 attackStrength = spm::an2_08::an2_08_wp.rpgNpcInfo[evtEntry->uw[0]].attackStrength;
    if (attackStrength == 0) attackStrength = 1;
    if ((spm::an2_08::an2_08_wp.unk_54 & 0x40U) != 0) {
      if (0 < attackStrength) {
        attackStrength = attackStrength / 2;
      }
      if (attackStrength == 0) {
        attackStrength = 1;
      }
    }
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], attackStrength);
    if (firstRun == false) {}
    return 2;
  }

  void patchWangSpecial() {
    spm::an2_08::lbl_80def2c8[1] = peach_special;
  }

  static inline void setUpPreset(s32 tribeId) {
    switch(tribeId)
    {
      case 0:
        rpgIsActive[0] = true;
        rpgIsActive[1] = true;
        rpgIsActive[2] = true;
        rpgTribeID[0] = 0;
        rpgTribeID[1] = 0;
        rpgTribeID[2] = 0;
      break;
      case 125:
        rpgIsActive[0] = true;
        rpgIsActive[1] = true;
        rpgIsActive[2] = true;
        rpgTribeID[0] = 0;
        rpgTribeID[1] = 125;
        rpgTribeID[2] = 0;
      break;
      case 529: // Doopliss
        rpgIsActive[0] = false;
        rpgIsActive[1] = true;
        rpgIsActive[2] = false;
        rpgTribeID[0] = 0;
        rpgTribeID[1] = 529;
        rpgTribeID[2] = 0;
      break;
    }
  }

  s32 turnBasedCombatOverrideFunc(spm::mario::MarioWork *marioWork, spm::npcdrv::NPCPart *npcPart, s32 lastAttackedDefenseType, s32 defenseType, s32 power, u8 param_6){
    s32 tribeId = npcPart->owner->tribeId;
    if (rpgInProgress != true) {
    setUpPreset(tribeId);
    spm::evtmgr::EvtEntry* battle = spm::evtmgr::evtEntry(parentOfBeginRPG, 1, 0);
    battle->lw[0] = 1;
    spm::evtmgr::EvtEntry* entry = spm::evtmgr::evtEntry(deleteAttackedEnemy, 1, 0);
    entry->lw[0] = npcPart->owner->name;
    return 1;
    }
  return 4;
  }

  void turnBasedCombatOverrideInit() {
    turnBasedCombatOverride[0].type = 20;
    turnBasedCombatOverride[0].value = turnBasedCombatOverrideFunc;
    turnBasedCombatOverride[1].type = 0;
    turnBasedCombatOverride[1].value = nullptr;
    spm::npcdrv::npcEnemyTemplates[2].unkDefinitionTable = turnBasedCombatOverride;
    spm::npcdrv::npcEnemyTemplates[250].unkDefinitionTable = turnBasedCombatOverride;
  }

  void deleteUnderchompTextures() {
    writeWord( & spm::an2_08::rpg_screen_draw, 0x204, 0x38600000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x208, 0x60000000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x20C, 0x60000000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x210, 0x60000000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x2AC, 0x60000000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x2C0, 0x60000000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x2D0, 0x60000000);
    writeWord( & spm::an2_08::rpg_screen_draw, 0x310, 0x60000000);
  }

  void new_rpg_screen_draw()
  {
    if (screenOn)
    {
      rpg_screen_draw();
      drawStuff();
    }
  }

  spm::mario::MarioWork* patchTechniques()
  {
    spm::mario::MarioWork* mwp = spm::mario::marioGetPtr();
    if (mwp->character == 0 || mwp->character == 3)
    {
      asm("li 12, 0x3");
    }
    return mwp;
  }

  void hookEvent() {
    patch::hookFunction(spm::an2_08::evt_rpg_calc_damage_to_enemy, new_evt_rpg_calc_damage_to_enemy);
    patch::hookFunction(spm::an2_08::evt_rpg_calc_mario_damage, new_evt_rpg_calc_mario_damage);
    rpg_screen_draw = patch::hookFunction(spm::an2_08::rpg_screen_draw, new_rpg_screen_draw);
    patchWangSpecial();

    //marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy, newMarioCalcDamageToEnemy);

    //spsndBGMOn = patch::hookFunction(spm::spmario_snd::spsndBGMOn, new_spsndBGMOn);

    //spsndSFXOn = patch::hookFunction(spm::spmario_snd::spsndSFXOn, new_spsndSFXOn);

    msgSearch = patch::hookFunction(spm::msgdrv::msgSearch, newMsgSearch);

    writeBranchLink( & spm::an2_08::rpgHandleMenu, 0x1BC, returnCharacterTechnique);
    writeBranchLink( & spm::an2_08::evt_rpg_npctribe_handle, 0x94, returnTribe);
    //writeBranchLink( & spm::an2_08::rpg_screen_draw, 0x2D8, setTextureIndex);
    writeBranchLink( & spm::acdrv::acMain, 0x49C, setNewFloat);
    writeBranchLink( & spm::an2_08::evt_rpg_choice_handler, 0x764, patchTechniques);
    writeWord( & spm::an2_08::evt_rpg_choice_handler, 0x768, 0x60000000);
    writeWord( & spm::an2_08::evt_rpg_choice_handler, 0x76C, 0x2C0C0003);
    //writeWord( & spm::an2_08::evt_rpg_choice_handler, 0x76C, 0x2C000001);
    //writeWord( & spm::an2_08::evt_rpg_npctribe_handle, 0xA0, 0x3B9C0004);
    //writeWord( & spm::an2_08::evt_rpg_npctribe_handle, 0x8C, 0x3BA00018);
    writeWord( & spm::an2_08::evt_rpg_npctribe_handle, 0x2BC, 0x60000000);
    writeWord(& spm::mario::marioCalcDamageToEnemy, 0x16C, 0x57FF003E);
    deleteUnderchompTextures();
    turnBasedCombatOverrideInit();
    //writeWord( & spm::an2_08::evt_rpg_npctribe_handle, 0x2BC, 0x60000000);
    //writeWord( & spm::acdrv::acMain, 0x49C, 0x60000000);
  }

  /*
      Custom USER FUNCs
  */

  s32 npcEntryFromTribeId(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::npcdrv::NPCWork * npcWork = spm::npcdrv::npcGetWorkPtr();
    spm::evtmgr::EvtVar * evtVariables = evtEntry -> pCurData;
    s32 id = spm::evtmgr_cmd::evtGetValue(evtEntry, * evtVariables);
    for (int i = 0; i < 535; i++) {
      if (npcWork -> entries[i].tribeId == id) {
        spm::evtmgr_cmd::evtSetValue(evtEntry, evtEntry -> lw[0], (s32) npcWork -> entries[i].name);
      }
    }
    if (firstRun == false) {}
    return 2;
  }

  s32 increaseAttack(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 newStrength = args[0];
    s8 npcIndex = evtEntry->uw[0];
    s32 strength = spm::an2_08::an2_08_wp.rpgNpcInfo[npcIndex].attackStrength;
    spm::an2_08::an2_08_wp.rpgNpcInfo[npcIndex].attackStrength = strength + newStrength;
    //spm::evtmgr_cmd::evtSetValue(evtEntry, (spm::evtmgr::EvtVar*)evtEntry->pCurData, 1);
    if (firstRun == false) {}
    return 2;
  }

  s32 rpg_npc_setup(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::camdrv::camPtrTbl[5] -> isOrthoToggle = 1;
    for (int i = 0; i < 3; i++) {
      if (rpgIsActive[i]) {
        spm::an2_08::an2_08_wp.rpgNpcInfo[i].attackStrength = spm::npcdrv::npcTribes[rpgTribeID[i]].attackStrength;
        spm::an2_08::an2_08_wp.rpgNpcInfo[i].maxHp = spm::npcdrv::npcTribes[rpgTribeID[i]].maxHp;
        spm::an2_08::an2_08_wp.rpgNpcInfo[i].killXp = spm::npcdrv::npcTribes[rpgTribeID[i]].killXp;
        spm::an2_08::an2_08_wp.rpgNpcInfo[i].flags = 0;
        spm::an2_08::an2_08_wp.rpgNpcInfo[i].unk_4 = 0;
        spm::an2_08::an2_08_wp.rpgNpcInfo[i].unk_10 = 0xff;
      }
    }
    rpgInProgress = true;
    fp = 5;
    patchTpl(116, 0, (wii::tpl::TPLHeader *)spm::icondrv::icondrv_wp->wiconTpl->sp->data, myTplHeader, "./a/n_mg_flower-", false);
    if (firstRun == false) {}
    if (evtEntry -> flags == 0) {}
    return 2;
  }

  s32 start_boss_fight(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    setUpPreset(args[0]);
    spm::evtmgr::evtEntry(parentOfBeginRPG, 1, 0);
    return 2;
  }

  s32 enable_disable_rpg_menu(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
  {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    screenOn = args[0];
    return 2;
  }

  s32 getFP(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], fp);
    if (firstRun == false) {}
    return 2;
  }

  s32 setFP(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    fp = args[0];
    if (firstRun == false) {}
    return 2;
  }

  s32 addFP(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    fp = fp + args[0];
    if (firstRun == false) {}
    return 2;
  }

  s32 subtractFP(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    fp = fp - args[0];
    if (firstRun == false) {}
    return 2;
  }

  s32 osReportLW(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 lw = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
    wii::os::OSReport("%d\n", lw);
    if (firstRun == false) {}
    return 2;
  }


  s32 rpg_off(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::camdrv::camPtrTbl[5]->isOrthoToggle = 1;
    rpgInProgress = false;
    succeededActionCommand = false;
    if (firstRun == false) {}
    if (evtEntry->flags == 0) {}
    return 2;
  }

  s32 calc_peach_heal(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    s32 gsw = 5;
    if (spm::spmario::gp->gsw0 >= 170) gsw = 20;
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], gsw);
    if (firstRun == false) {}
    return 2;
  }

  s32 check_pressed_2_ac(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
    if (pressed & 0x100) {
      wii::os::OSReport("Succeeded action command!\n");
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
    } else {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
    }
    return 2;
  }

  s32 check_pressed_a_ac(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
    if (pressed & 0x800) {
      wii::os::OSReport("Succeeded action command!\n");
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
    } else {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
    }
    return 2;
  }

  s32 check_pressed_b_ac(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    u32 pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
    if (pressed & 0x400) {
      wii::os::OSReport("Succeeded action command!\n");
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
    } else {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
    }
    return 2;
  }

  s32 check_ac_success(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    if (succeededActionCommand == true) {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
    } else {
      spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
    }
    return 2;
  }

  s32 ac_success_toggle(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
      if (succeededActionCommand == false) {
        succeededActionCommand = true;
      } else {
        succeededActionCommand = false;
      }
    return 2;
  }

  s32 superguard_toggle(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    if (superGuard == false) {
      superGuard = true;
    } else {
      superGuard = false;
    }
  return 2;
}

s32 check_superguard_success(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  if (superGuard == true) {
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
  } else {
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
  }
  return 2;
}

  s32 ac_success_reset(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    succeededActionCommand = false;
    superGuard = false;
    return 2;
  }

  s32 displayDamage(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    f32 x = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[0]);
    f32 y = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[1]);
    f32 z = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[2]);
    s32 damage = spm::evtmgr_cmd::evtGetValue(evtEntry, args[3]);
    spm::effdrv::damageEffect(x, y, z, 0, damage);
    return 2;
  }

  void patchBrobot() {
    spm::npcdrv::npcTribes[295].maxHp = 1;
    spm::npcdrv::npcTribes[295].killXp = 100;
    spm::npcdrv::npcTribes[296].maxHp = 130;
  }

  void patchTpl(u32 destId, u32 srcId, wii::tpl::TPLHeader *destTpl, wii::tpl::TPLHeader *srcTpl, const char *filePath = nullptr, bool free = false) {

    // Loads the tpl if not already loaded by the stated filePath
    if (srcTpl == nullptr){
    spm::filemgr::FileEntry * srcFile = spm::filemgr::fileAllocf(4, filePath);
    s32 tplSize = srcFile->length;
    srcTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAIN, tplSize);
    msl::string::memcpy(srcTpl, srcFile->sp->data, tplSize);
    spm::filemgr::fileFree(srcFile);
    }

    // Patches the destination tpl with the one given by the mod.rel
    destTpl->imageTable[destId] = srcTpl->imageTable[srcId];

    // Free the memory of the tpl loaded from mod.rel to prevent a leak
    if (free == true){
    spm::memory::__memFree(spm::memory::Heap::HEAP_MAIN, srcTpl);
    }
    return;
  }

  void main() {
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
    evtpatch::evtmgrExtensionInit();
    hookEvent();
    patchBrobot();
    npc_rpgdrv_main();
  }

}
