#include "ip_assets.h"
#include "patch.h"
#include "util.h"

#include "ip_pausetex.h"

#include <spm/pausewin.h>
#include <spm/system.h>
#include <msl/string.h>
#include <wii/tpl.h>
#include <common.h>

namespace ip {

using spm::filemgr::FileEntry;
using spm::filemgr::fileAllocf;
using spm::pausewin::PAUSETEX_CHAPTERS_BTN;
using spm::pausewin::PAUSETEX_CHAPTER_1_BTN;
using spm::pausewin::PAUSETEX_CHAPTER_2_BTN;
using wii::tpl::TPLHeader;
using wii::tpl::ImageHeader;
using wii::tpl::IMG_FMT_RGB5A3;
using wii::tpl::IMG_FMT_CMPR;

/*
    Define textures to replace
*/
struct PauseTexOverride
{
    u16 imageId;
    u16 expectedWidth;
    u16 expectedHeight;
    u16 expectedFormat;
    const u8 * image;
    const u32 * imageSize;
};
PauseTexOverride pauseOverrides[] =
{
    {PAUSETEX_CHAPTERS_BTN,  120, 40, IMG_FMT_RGB5A3, badgeTex,    &badgeTex_size},
    {PAUSETEX_CHAPTER_1_BTN, 112, 32, IMG_FMT_CMPR,   allTex,      &allTex_size},
    {PAUSETEX_CHAPTER_2_BTN, 112, 32, IMG_FMT_CMPR,   equippedTex, &equippedTex_size},
};

/*
    Check the user doesn't have incompatible file mods
*/
static bool verifyImageProperties(ImageHeader * img, PauseTexOverride * def)
{
    return img->width == def->expectedWidth &&
           img->height == def->expectedHeight &&
           img->format ==  def->expectedFormat;
}

/*
    Patch pause.tpl on load
*/
static FileEntry * pauseTplOverride(s32 filetype, const char * format, const char * dvdRoot,
                                    const char * lang)
{
    // Load file
    FileEntry * file = fileAllocf(filetype, format, dvdRoot, lang);   

    // Apply overrides
    for (u32 i = 0; i < ARRAY_SIZEOF(pauseOverrides); i++)
    {
        // Get def
        PauseTexOverride * def = pauseOverrides + i;

        // Get image
        TPLHeader * tpl = (TPLHeader *) file->sp->data;
        ImageHeader * img = tpl->imageTable[def->imageId].image;

        // Verify user mods don't interfere
        assertf(verifyImageProperties(img, def), "Unsupported pause.tpl edit to image %d",
                def->imageId);

        // Copy custom texture in
        msl::string::memcpy(img->data, def->image, *def->imageSize);
    }

    // Return modified file
    return file;
}

/*
    Apply pause.tpl texture patches
*/
void pauseTexPatch()
{
    writeBranchLink(& spm::pausewin::pausewinEntry, 0x154, pauseTplOverride);
}

}
