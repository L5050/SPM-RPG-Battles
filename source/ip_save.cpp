#include "consolewindow.h"
#include "evt_cmd.h"
#include "patch.h"
#include "ip_badgepouch.h"
#include "util.h"

#include <common.h>
#include <spm/evtmgr_cmd.h>
#include <spm/nandmgr.h>
#include <spm/seqdrv.h>
#include <spm/system.h>
#include <msl/string.h>
#include <msl/stdio.h>

namespace ip {

constexpr u32 newMapnameOffset = sizeof("title");

/*
    Makes it so the mapname written to the save is 'title', making it impossible
    for this save to be loaded in a vanilla game.
    The true map name is stored after this, and is what the mod will read.
*/
static void blockVanillaLoad(spm::nandmgr::SaveFile * save)
{
    char * name = save->spmarioGlobals.mapName;
    u32 nameSize = msl::string::strlen(name) + 1; // include null byte

    // Move back name so the mod can read it
    msl::string::memmove(name + newMapnameOffset, name, nameSize);

    // Replace the name at the start so the vanilla game won't load the save
    msl::string::strcpy(name, "title");
}

/*
    Makes it so the mapname read from the save is the real one, not 'title'
*/
static void undoVanillaBlock(spm::nandmgr::SaveFile * save)
{
    char * name = spm::spmario::gp->mapName;
    char * trueName = name + newMapnameOffset;
    u32 nameSize = msl::string::strlen(trueName) + 1; // include null byte

    // If the name wasn't title, this isn't an infinite pit save, return to title screen
    if (msl::string::strcmp(name, "title") != 0)
    {
        const wii::gx::GXColor errorColour = {0xff, 0x00, 0x00, 0xff};
        mod::ConsoleWindow::push("Vanilla saves can not be loaded!", &errorColour);
        msl::string::strcpy(name, "title");
        return;
    }

    // Move the true name into position so the game can read it
    msl::string::memmove(name, trueName, nameSize);
}

/*
    The game increments GSW(1), which indicates which pit room to load next, after the
    current room is loaded, so saving & reloading would put you into the next floor.
    This fixes that by decreasing GSW(1) in the save but not in the live game.
*/
static void handlePitSave(spm::nandmgr::SaveFile * save)
{
    if (msl::string::strncmp(spm::spmario::gp->mapName, "dan", 3) == 0)
        save->spmarioGlobals.gsw[1] -= 1;
}

/*
    Add an [IP] prefix to save file names
*/
static void addNamePrefix(char * gpSaveName, const char * chosenSaveName)
{
    msl::stdio::sprintf(gpSaveName, "[IP] %s", chosenSaveName);
}

/*

*/
static void initNewSave()
{
    // Change to room outside the pit
    spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_05", "elv1");

    // Set sequence position high enough to flip to 3d
    spm::evtmgr_cmd::evtSetValue(nullptr, GSW(0), 16);

    // Set first save flag
    spm::evtmgr_cmd::evtSetValue(nullptr, GSWF(2), 1);

    // Set first 3d flip flag
	spm::evtmgr_cmd::evtSetValue(nullptr, GSWF(386), 1);

	// Set first HP critical flag
	spm::evtmgr_cmd::evtSetValue(nullptr, GSWF(420), 1);

    // Set pit open flag
    spm::evtmgr_cmd::evtSetValue(nullptr, GSWF(501), 1);

    // Give the player Boomer to ensure all enemies can be defeated
    spm::mario_pouch::pouchAddItem(222);
    
    badgePouchInit();
}

/*
    Apply the above save-related patches
*/
static void (*nandUpdateSaveReal)(int saveId);

static void updateSavePatch()
{
    // This conflicts with a practice codes patch, so that's undone
    writeWord(spm::nandmgr::nandUpdateSave, 0, 0x9421ffe0); // stwu r1, -0x20 (r1)

    nandUpdateSaveReal = mod::patch::hookFunction(spm::nandmgr::nandUpdateSave,
        [](int saveId)
        {
            // Carry out vanilla behaviour
            nandUpdateSaveReal(saveId);

            // Extra mod tasks
            spm::nandmgr::SaveFile * save = spm::nandmgr::nandGetSaveFiles() + saveId;
            handlePitSave(save);
            blockVanillaLoad(save);

            // Update checksum after mod edits
            mod::updateSaveChecksum(save);
        }
    );

    // Override save name
    writeBranchLink(0x8017d95c, 0, addNamePrefix);
}

/*
    Apply the above load-related patches
*/
static void (*nandLoadSaveReal)(int saveId);

static void readSavePatch()
{
    nandLoadSaveReal = mod::patch::hookFunction(spm::nandmgr::nandLoadSave,
        [](int saveId)
        {
            // Carry out vanila behaviour
            nandLoadSaveReal(saveId);

            // Extra mod tasks
            spm::nandmgr::SaveFile * save = spm::nandmgr::nandGetSaveFiles() + saveId;
            undoVanillaBlock(save);
        }
    );

    // Run when creating a new save
    writeBranchLink(0x8017c1c8, 0, initNewSave);

    // Run when re-loading a new save
    writeBranchLink(0x8017c230, 0, initNewSave);
}

/*
    Public function to apply all patches
*/
void savePatch()
{
    updateSavePatch();
    readSavePatch();
}

}
