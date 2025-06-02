#pragma once

#include <nw4r/snd.hpp>
#include <spm/spmario_snd.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os/OSError.h>
#include <nw4r/snd.hpp>
#include "evt_cmd.h"

namespace mod::sndpatch
{
  EVT_DECLARE_USER_FUNC(evt_sndpatch_bgm_fadein, 3)

  #define SNDPATCH_BGM_FADEIN(setPlayer, name, getPlayer, fade) \
    USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, setPlayer, name, fade) \
      WAIT_FRM(3) \
        USER_FUNC(mod::sndpatch::evt_sndpatch_bgm_fadein, setPlayer, name, getPlayer)

}
