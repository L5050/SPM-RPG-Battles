#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"

#include <patch.h>
#include <spm/evtmgr.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod {

EVT_DECLARE(browser_edging_attack)
EVT_DECLARE(browser_safarshit_onspawn)
EVT_DECLARE(browser_chromium_onhit)
NPCTribeAnimDef * getAnimsBrowserFirefoxExtension();
void browser_main();

}
