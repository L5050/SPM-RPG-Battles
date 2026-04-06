#include "map_patch.h"
#include "patch.h"
#include <msl/string.h>
#include <spm/evtmgr_cmd.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <spm/mapdrv.h>
#include <spm/camdrv.h>
#include <wii/os.h>

namespace mod::map_patch {

using namespace spm;

static mapdrv::MapWork * mapdrv_wp;

static void( * mapUnload)();
static int ( * mapLoad)(const char * name);
static void ( * camLoadRoadNoRead)(void * file);

static void new_camLoadRoadNoRead(void * file)
{
  if (mapdrv_wp->activeGroup == 1)
  {
    return;
  }
  return camLoadRoadNoRead(file);
}

static void new_mapUnload()
{
  mapdrv_wp->activeGroup = 1;
  mapUnload();
  mapdrv_wp->activeGroup = 0;
  mapUnload();
}

static int new_mapLoad(const char * name)
{
  mapdrv_wp->activeGroup = 1;
  int fileLoaded = false;
  do
  {
    fileLoaded = mapLoad("bow_01");
  } while (!fileLoaded);
  
  mapdrv_wp->activeGroup = 0;
  fileLoaded = false;
  do
  {
    fileLoaded = mapLoad(name);
  } while (!fileLoaded);
  return 1;
}

void map_patch_main()
{
  mapdrv_wp = (mapdrv::MapWork *)mapdrv::mapGetWork();
  mapUnload = patch::hookFunction(mapdrv::mapUnload, new_mapUnload);
  mapLoad = patch::hookFunction(mapdrv::mapLoad, new_mapLoad); 
  camLoadRoadNoRead = patch::hookFunction(camdrv::camLoadRoadNoRead, new_camLoadRoadNoRead); 
  writeWord(mapdrv::mapUnload, 0x20, NOP);
  writeWord(mapdrv::mapLoad, 0x8c, NOP);
}

}
