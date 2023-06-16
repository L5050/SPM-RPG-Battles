#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_fade)

// evt_fade_end_wait(s32)
EVT_DECLARE_USER_FUNC(evt_fade_end_wait, 1)
//evt_fade_entry(int transitionType, everything else is unknown ints)
EVT_DECLARE_USER_FUNC(evt_fade_entry, 6)
UNKNOWN_FUNCTION(func_800e715c);
UNKNOWN_FUNCTION(func_800e71dc);
UNKNOWN_FUNCTION(func_800e720c);
UNKNOWN_FUNCTION(func_800e7268);

CPP_WRAPPER_END()
