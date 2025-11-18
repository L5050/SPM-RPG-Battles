#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "evtpatch.h"
#include "map_data/mi1_08.h"
#include "map_data/map_data_main.h"
#include "npc_rpgdrv.h"

#include <patch.h>
#include <spm/evt_mobj.h>
#include <spm/map_data.h>
#include <spm/evtmgr.h>
#include <spm/seqdef.h>


namespace mod {

void map_data_main()
{
  mi1_08_main();
}

}