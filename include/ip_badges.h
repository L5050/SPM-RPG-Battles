/*
    Infinite Pit: Badges
    TTYD/64 style badges
*/

#pragma once

#include <common.h>

namespace ip {

#define NUM_BADGES 3

enum BadgeId : s8
{
    BADGEID_NONE = -1,
    BADGEID_HP_PLUS,
    BADGEID_POWER_BOUNCE,
    BADGEID_TEST_3
};

struct BadgeDef
{
    const char * nameMsg;
    const char * descMsg;
    s8 iconId;
    s8 bpCost;
};

extern BadgeDef badgeDefs[NUM_BADGES];

}
