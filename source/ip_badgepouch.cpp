#include "ip_badges.h"
#include "ip_badgepouch.h"
#include "patch.h"
#include "util.h"

#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <msl/string.h>
#include <common.h>

namespace ip
{

  /*
      Temporary stubs
  */

  static PouchBadgeInfo *badgePouch;

  PouchBadgeInfo *pouchGetBadgeInfo(s32 slot)
  {
    return &badgePouch[slot];
  }

  BadgeDef *pouchGetBadgeDef(s32 slot)
  {
    return badgeDefs + badgePouch[slot].id;
  }

  s32 pouchCountBadges()
  {
    u32 i;
    for (i = 0; i < POUCH_BADGE_COUNT; i++)
    {
      if (pouchGetBadgeInfo(i)->id == BADGEID_NONE)
        break;
    }
    return i;
  }

  s32 pouchCountEquippedBadges()
  {
    s32 n = 0;
    for (s32 i = 0; i < POUCH_BADGE_COUNT; i++)
    {
      if (badgePouch[i].id == BADGEID_NONE)
        break;
      if (badgePouch[i].equipped)
        n++;
    }
    return n;
  }

  bool pouchAddBadge(BadgeId badge)
  {
    s32 i;
    for (i = 0; i < POUCH_BADGE_COUNT; i++)
    {
      if (badgePouch[i].id == BADGEID_NONE)
        break;
    }

    if (i < POUCH_BADGE_COUNT)
    {
      badgePouch[i].id = badge;
      return true;
    }
    else
    {
      return false;
    }
  }

  bool pouchRemoveBadge(BadgeId badge)
  {
    // Find badge
    for (s32 i = 0; i < POUCH_BADGE_COUNT; i++)
    {
      if (pouchGetBadgeInfo(i)->id == badge)
      {
        // Move forward badges after
        for (s32 j = i; j < POUCH_BADGE_COUNT - 1; j++)
          badgePouch[j] = badgePouch[j + 1];

        // Add empty slot at the back
        badgePouch[POUCH_BADGE_COUNT - 1] = {BADGEID_NONE, false};

        // Signal success
        return true;
      }
    }

    // Signal failure
    return false;
  }

  void badgePouchInit()
  {
    // Set all to empty and unequipped
    if (badgePouch[0].id == 0 && badgePouch[1].id == 0)
    {
      for (s32 i = 0; i < POUCH_BADGE_COUNT; i++)
        badgePouch[i] = {BADGEID_NONE, false};
      badgePouch[0].id = BADGEID_TEST_1;
    }
  }

  void badgePouchPatch(s32 gsw)
  {
    // Get badge pouch location
    badgePouch = (PouchBadgeInfo *)(&spm::spmario::gp->gsw[gsw]);
  }

}
