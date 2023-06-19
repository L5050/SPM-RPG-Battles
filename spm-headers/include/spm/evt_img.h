#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_img)

EVT_DECLARE_USER_FUNC(evt_img_battle_start, 1)
EVT_DECLARE_USER_FUNC(evt_img_pose_handle, 2)
EVT_DECLARE_USER_FUNC(evt_img_pose_handle2, 2)
EVT_DECLARE_USER_FUNC(evt_img_properties_set, 8)
EVT_DECLARE_USER_FUNC(evt_img_battle_state_set, 2)
EVT_DECLARE_USER_FUNC(evt_img_property_check, 1)
EVT_DECLARE_USER_FUNC(evt_img_battle_setup, 1)

UNKNOWN_FUNCTION(func_800ec30c);
UNKNOWN_FUNCTION(func_800ec87c);
UNKNOWN_FUNCTION(func_800ec998);
UNKNOWN_FUNCTION(func_800eca64);
UNKNOWN_FUNCTION(func_800ecae0);
UNKNOWN_FUNCTION(func_800ecb5c);
UNKNOWN_FUNCTION(func_800ecbd8);

CPP_WRAPPER_END()
