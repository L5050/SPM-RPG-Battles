#pragma once

#include <common.h>
#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "main_scripting.h"
#include "power_refresh.h"
#include "stack.hh"

#include <patch.h>
#include <spm/rel/an.h>
#include <spm/acdrv.h>
#include <spm/evt_ac.h>
#include <spm/evtmgr.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>

using namespace spm::npcdrv;
using namespace spm::evtmgr;

namespace mod
{

static Stack<spm::icondrv::IconEntry*> hpStack;
static Stack<spm::icondrv::IconEntry*> fpStack;
static Stack<spm::icondrv::IconEntry*> poisonStack;

void initFunc(spm::acdrv::AcEntry *entry)
{
  entry->flags = entry->flags | 4;
  PowerRefreshWork *wp = (PowerRefreshWork *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(PowerRefreshWork));
  entry->acDefEntry = (void *)wp;
}



}