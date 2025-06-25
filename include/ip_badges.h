/*
    Infinite Pit: Badges
    TTYD/64 style badges
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>

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
    const spm::evtmgr::EvtScriptCode* badgeScript;
};

extern BadgeDef badgeDefs[NUM_BADGES];

const spm::evtmgr::EvtScriptCode* getBadgeScriptById(s32 id);
EVT_DECLARE_USER_FUNC(get_badge_script_by_id, 2)
EVT_DECLARE_USER_FUNC(get_badge_script_by_technique, 2)


EVT_DECLARE(power_bounce)

}
