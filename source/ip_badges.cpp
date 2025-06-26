#include "mod.h"
#include "ip_badges.h"
#include "ip_badgepouch.h"

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/msgdrv.h>

namespace ip {

BadgeDef badgeDefs[NUM_BADGES] = {
    /*
        nameMsg
        descMsg
        iconId
        bpCost
        badgeScript
    */
    // HP Plus
    {
        "bn_test_1",
        "bd_test_1",
        0,
        3,
        power_bounce
    },
    // Power Jump
    {
        "bn_test_2",
        "bd_test_2",
        1,
        1,
        nullptr
    },
    // BADGE_TEST_3
    {
        "bn_test_3",
        "bd_test_3",
        2,
        0,
        nullptr
    },
};

const spm::evtmgr::EvtScriptCode* getBadgeScriptById(s32 id)
{
  for (s32 i = 0; i < NUM_BADGES; i++)
  {
    if (badgeDefs[i].iconId == i) {
      return badgeDefs[i].badgeScript;
    }
  }
  return nullptr;
}

s32 getBadgeIdByTechnique(s32 index)
{
  s32 badgeCount = ip::pouchCountBadges();
  ip::PouchBadgeInfo *badgeInfo = ip::pouchGetBadgeInfo(0);
  s32 badgeArray[pouchCountBadges()];
  badgeArray[0] = 0; //this is always tattle so we need to exclude it from the badge calculation
  s32 badgeArrayIndex = 1;
  for (int i = 0; i < badgeCount; i++)
  {
    if (badgeInfo[i].equipped && mod::checkBadgeTechnique(badgeInfo[i].id))
    {
      ip::BadgeDef *badgeDef = ip::pouchGetBadgeDef(i);
      badgeArray[badgeArrayIndex] = badgeDef->iconId;
      badgeArrayIndex++;
    }
  }
  return badgeArray[index];
}

s32 get_badge_script_by_id(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  s32 index = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], (s32)getBadgeScriptById(index));
  return 2;
}

s32 get_badge_script_by_technique(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  s32 index = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  index = getBadgeIdByTechnique(index);
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], (s32)getBadgeScriptById(index));
  return 2;
}

}
