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

wii::tpl::TPLHeader* badgeMenuTpl = nullptr;

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
    u32 imageSize;
};
PauseTexOverride pauseOverrides[] =
{
    {PAUSETEX_CHAPTERS_BTN,  120, 40, IMG_FMT_RGB5A3, nullptr,    0}, // badgeTex
    {PAUSETEX_CHAPTER_1_BTN, 112, 32, IMG_FMT_CMPR,   nullptr,      0}, // allTex
    {PAUSETEX_CHAPTER_2_BTN, 112, 32, IMG_FMT_CMPR,   nullptr, 0}, // equippedTex
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
        SPM_ASSERT(verifyImageProperties(img, def), "Unsupported pause.tpl edit to image %d",
                def->imageId);

        // Copy custom texture in
        msl::string::memcpy(img->data, def->image, def->imageSize);
    }

    // Return modified file
    return file;
}

/*
    Apply pause.tpl texture patches
*/
void pauseTexPatch()
{
    spm::filemgr::FileEntry * srcFile = spm::filemgr::fileAllocf(0, "./mod/badgemenu.tpl");
    s32 tplSize = srcFile->length;
    badgeMenuTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAIN, tplSize);
    msl::string::memcpy(badgeMenuTpl, srcFile->sp->data, tplSize);
    spm::filemgr::fileFree(srcFile);
    wii::tpl::TPLBind(badgeMenuTpl);
    pauseOverrides[0].image = (const u8 *) badgeMenuTpl->imageTable[0].image->data;
    pauseOverrides[0].imageSize = 9600;//(badgeMenuTpl->imageTable[0].image->height * badgeMenuTpl->imageTable[0].image->width) * 2;
    pauseOverrides[1].image = (const u8 *) badgeMenuTpl->imageTable[1].image->data;
    pauseOverrides[1].imageSize = 1792;//(badgeMenuTpl->imageTable[1].image->height * badgeMenuTpl->imageTable[0].image->width) / 2;
    pauseOverrides[2].image = (const u8 *) badgeMenuTpl->imageTable[2].image->data;
    pauseOverrides[2].imageSize = 1792;//(badgeMenuTpl->imageTable[2].image->height * badgeMenuTpl->imageTable[0].image->width) / 2;
    writeBranchLink(& spm::pausewin::pausewinEntry, 0x154, pauseTplOverride);
}

}
