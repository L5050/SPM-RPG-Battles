#include "mod_ui_base/colours.h"
#include "mod_ui_base/window.h"
#include "patch.h"
#include "util.h"

#include "ip_assets.h"
#include "ip_badges.h"
#include "ip_badgemenu.h"
#include "ip_badgepouch.h"

#include <spm/gxsub.h>
#include <spm/spmario_snd.h>
#include <spm/icondrv.h>
#include <spm/pausewin.h>
#include <spm/wpadmgr.h>
#include <msl/string.h>
#include <msl/stdio.h>
#include <wii/mtx.h>
#include <wii/wpad.h>
#include <wii/tpl.h>
#include <common.h>

namespace ip {
wii::tpl::TPLHeader* badgeIconTpl = nullptr;
using spm::icondrv::iconDispGx;
using spm::pausewin::PausewinEntry;
using spm::pausewin::PAUSETEX_NONE;
using spm::pausewin::PAUSETEX_CHAPTERS_BTN;
using spm::pausewin::PAUSETEX_CHAPTER_1_BTN;
using spm::pausewin::PAUSETEX_CHAPTER_2_BTN;
using spm::pausewin::PLUSWIN_BTN_CHAPTERS;
using spm::pausewin::PLUSWIN_BTN_BG;
using spm::pausewin::PLUSWIN_BTN_HELP;
using spm::pausewin::PLUSWIN_BTN_STATS;
using spm::pausewin::PLUSWIN_STATE_CHAPTERS;
using spm::pausewin::pausewin_pluswinWp;
using spm::pausewin::pausewin_wp;
using spm::pausewin::pausewinEntry;
using spm::pausewin::pausewinMoveTo;
using spm::pausewin::pausewinAppear;
using spm::pausewin::pausewinDisappear;
using spm::pausewin::pausewinDelete;
using spm::pausewin::pausewinSetMessage;
using spm::wpadmgr::wpadGetButtonsPressed;
using wii::mtx::Vec2;
using msl::stdio::sprintf;

#define PLUSWIN_BTN_BADGES PLUSWIN_BTN_CHAPTERS
#define PAUSETEX_BADGES_BTN PAUSETEX_CHAPTERS_BTN
#define PLUSWIN_STATE_BADGES PLUSWIN_STATE_CHAPTERS
#define PAUSETEX_BADGES_ALL PAUSETEX_CHAPTER_1_BTN
#define PAUSETEX_BADGES_EQUIPPED PAUSETEX_CHAPTER_2_BTN

#define MAIN_ENTRY(idx) (pausewin_wp->entries + pausewin_pluswinWp->entryIds[(idx)])
#define MAIN_ENTRY_ID(idx) (pausewin_pluswinWp->entryIds[(idx)])
#define BADGE_ENTRY(idx) (pausewin_wp->entries + pausewin_pluswinWp->submenuEntryIds[(idx)])
#define BADGE_ENTRY_ID(idx) (pausewin_pluswinWp->submenuEntryIds[(idx)])
#define CUR_BTN (pausewin_pluswinWp->submenuSelectedButton)

#define BADGE_LINE_HEIGHT 33.0f
#define BADGE_PAGE_SIZE 8

static struct
{
    s8 option; // index into badges on page (0 to BADGE_PAGE_SIZE - 1)
    s8 page; // currently selected page
    s8 pageCount; // number of pages
    s8 pageBadgeCount; // number of badges on page
    s16 badgeCount; // number of badges in all pages
    s16 pageStart; // index of first badge on page
    bool equippedOnly; // whether unequipped badges are displayed
} work;

enum BadgeSubmenuIdx
{
    BADGE_BTN_ALL = 0,
    BADGE_BTN_EQUIPPED = 1,
    // This position is requied, if deleted then this menu is considered closed by the game
    BADGE_BTN_MAIN = 9,
    // No PausewinEntry
    BADGE_BTN_BADGES = 10
};

static s32 menuIdxToBadgeSlot(s32 idx);
static s32 getSelectedIdx();
static s32 getSelectedBadgeSlot();
static void setHelpMessage(const char * msgName);
static void initPage(s32 page, bool equippedOnly, s32 option);
static void initElement(s32 id, s32 idx);
static void moveToBadge();
static void selectButton(s32 idx);
static void updateOnAll(u32 btn);
static void updateOnEquipped(u32 btn);
static void updateOnBadges(u32 btn);
static void menuMain(PausewinEntry * entry);
static void menuDisp(PausewinEntry * entry);
static void menuClose();
static void menuOpen();

/*
    Returns the badge inventory slot for an index into the displayed list
*/
static s32 menuIdxToBadgeSlot(s32 idx)
{
    if (work.equippedOnly)
    {
        // Return the index containing the idx'th equipped badge
        s32 i;
        for (i = 0; true; i++)
        {
            if (pouchGetBadgeInfo(i)->equipped)
            {
                if (--idx < 0)
                    break;
            }
        }
        return i;
    }
    else
    {
        // Index == slot
        return idx;
    }
}

/*
    Returns the index into the current list of the selected badge
*/
static s32 getSelectedIdx()
{
    return work.page * BADGE_PAGE_SIZE + work.option;
}

/*
    Returns the badge inventory slot of the currently selected badge
*/
static s32 getSelectedBadgeSlot()
{
    return menuIdxToBadgeSlot(getSelectedIdx());
}

/*
    Sets the message displayed in the help box at the bottom of the menu
*/
static void setHelpMessage(const char * msgName)
{
    pausewinSetMessage(MAIN_ENTRY(PLUSWIN_BTN_HELP), 0, msgName);
}

/*
    Calculate required values for this page of badges
*/
static void initPage(s32 page, bool equippedOnly, s32 option)
{
    // Back up parameters
    work.page = page;
    work.equippedOnly = equippedOnly;

    // Select initial value (bottom or top)
    work.option = option;

    // Calculate the badge number at the start of this page
    work.pageStart = page * BADGE_PAGE_SIZE;

    // Get count
    if (equippedOnly)
        work.badgeCount = pouchCountEquippedBadges();
    else
        work.badgeCount = pouchCountBadges();

    // Calculate number of pages (badge count divided by page size, rounded up)
    work.pageCount = (work.badgeCount + BADGE_PAGE_SIZE - 1) / BADGE_PAGE_SIZE;

    // If on the final page and the total count isn't a multiple of 8, the page
    // badge count is the badge count mod 8, otherwise it's just 8
    if (work.page + 1 == work.pageCount && work.badgeCount % BADGE_PAGE_SIZE != 0)
    {
        work.pageBadgeCount = work.badgeCount % BADGE_PAGE_SIZE;
    }
    else
    {
        // This doesn't apply when no badges are on the page
        if (work.badgeCount == 0)
            work.pageBadgeCount = 0;
        else
            work.pageBadgeCount = BADGE_PAGE_SIZE;
    }
}

/*
    Stores a new element's id and makes it appear
*/
static void initElement(s32 id, s32 idx)
{
    // Store id
    BADGE_ENTRY_ID(idx) = id;

    // Make it appear
    pausewinAppear(id);
}

/*
    Moves cursor to the selected badge in the list
*/
static void moveToBadge()
{
    // Update current button
    CUR_BTN = BADGE_BTN_BADGES;

    // Move cursor
    f32 lineY = 155.0f - (33.0f * work.option);
    pausewin_pluswinWp->cursorMoveDest.x = -120.0f;
    pausewin_pluswinWp->cursorMoveDest.y = lineY - 20.0f;

    // Update help message
    setHelpMessage(pouchGetBadgeDef(getSelectedBadgeSlot())->descMsg);
    spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE2");
}

/*
    Moves cursor to either the equipped or all badges button
    (must not be called for moving to a badge)
*/
static void selectButton(s32 idx)
{
    // Disable highlighting of current button if needed
    if (CUR_BTN == BADGE_BTN_ALL || CUR_BTN == BADGE_BTN_EQUIPPED)
        BADGE_ENTRY(CUR_BTN)->flags &= ~PAUSE_FLAG_HIGHLIGHT;

    // Update current button
    CUR_BTN = idx;

    // Move cursor and enable highlighting
    BADGE_ENTRY(CUR_BTN)->flags |= PAUSE_FLAG_HIGHLIGHT;
    pausewin_pluswinWp->cursorMoveDest.x = BADGE_ENTRY(CUR_BTN)->pos.x - 25.0f;
    pausewin_pluswinWp->cursorMoveDest.y = BADGE_ENTRY(CUR_BTN)->pos.y - 20.0f;

    // Update help message
    switch (idx)
    {
        case BADGE_BTN_ALL:
            setHelpMessage("menu_help_badges_all");
            break;
        case BADGE_BTN_EQUIPPED:
            setHelpMessage("menu_help_badges_equipped");
            break;
        default:
            break;
    }
}

/*
    Handles inputs when on the All Badges button
*/
static void updateOnAll(u32 btn)
{
    if (btn & WPAD_BTN_1)
    {
        // Close if 1 is pressed
        menuClose();
    }
    else if (btn & WPAD_BTN_LEFT)
    {
        // Move down to Equipped button
        initPage(0, true, 0);
        selectButton(BADGE_BTN_EQUIPPED);
    }
    else if ((btn & (WPAD_BTN_DOWN | WPAD_BTN_2)) && pouchCountBadges() > 0)
    {
        // Move right to badge list
        moveToBadge();
    }
}

/*
    Handles inputs when on the Equipped button
*/
static void updateOnEquipped(u32 btn)
{
    if (btn & WPAD_BTN_1)
    {
        // Close if 1 is pressed
        menuClose();
    }
    else if (btn & WPAD_BTN_RIGHT)
    {
        // Move up to All Badges button
        initPage(0, false, 0);
        selectButton(BADGE_BTN_ALL);
    }
    else if ((btn & (WPAD_BTN_DOWN | WPAD_BTN_2)) && pouchCountEquippedBadges() > 0)
    {
        // Move right to badge list
        moveToBadge();
    }
}

/*
    Handles inputs when in the badge list
*/
static void updateOnBadges(u32 btn)
{
    if (btn & WPAD_BTN_1)
    {
        spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CANCEL1");
        // Move back if 1 is pressed
        if (work.equippedOnly)
            selectButton(BADGE_BTN_EQUIPPED);
        else
            selectButton(BADGE_BTN_ALL);        
    }
    else if (btn & WPAD_BTN_2)
    {
        PouchBadgeInfo * info = pouchGetBadgeInfo(getSelectedBadgeSlot());
        if (info->equipped)
        {
          spm::spmario_snd::spsndSFXOn("SFX_SYS_FILE_MOJI_DELETE1");
        } else {
          spm::spmario_snd::spsndSFXOn("SFX_SYS_FILE_MOJI_SET1");
        }
        // Equip/Dequip badge
        info->equipped = !info->equipped;
        // TODO: bp

        // Special cases for equipped menu
        if (!info->equipped && work.equippedOnly)
        {
            if (work.badgeCount == 1)
            {
                // If this is the last badge, move back to the side button 
                selectButton(BADGE_BTN_EQUIPPED);
                initPage(0, true, 0);
            }
            else
            {
                if (work.pageBadgeCount == 1)
                {
                    // If this is the only badge on its page, move to previous page
                    initPage(work.page - 1, true, BADGE_PAGE_SIZE - 1);
                }
                else
                {
                    // Move up one badge if this is the last badge
                    if (getSelectedIdx() + 1 == work.badgeCount)
                        work.option -= 1;
                    
                    // Regenerate page
                    initPage(work.page, true, work.option);
                }
                moveToBadge();
            }
        }
    }
    else if (btn & WPAD_BTN_LEFT)
    {
        // Move down one badge
        if (++work.option == work.pageBadgeCount)
        {
            // Handle being at the bottom of the page

            if (work.page + 1 == work.pageCount)
                // Move back if it's the bottom badge of the last page
                --work.option;
            else
                // Move to top of the next page
                initPage(work.page + 1, work.equippedOnly, 0);
        }

        // Move to new badge
        moveToBadge();
    }
    else if (btn & WPAD_BTN_RIGHT)
    {
        // Move up one badge
        if (--work.option == -1)
        {
            // Handle being at the top of the page

            if (work.page == 0)
                // Move back if it's the top of the first page
                ++work.option;
            else
                // Move to the bottom of the previous page
                initPage(work.page - 1, work.equippedOnly, BADGE_PAGE_SIZE - 1);
        }

        // Move to new badge 
        moveToBadge();
    }
    else if (btn & WPAD_BTN_UP)
    {
        // Move to original button
        if (work.equippedOnly)
            selectButton(BADGE_BTN_EQUIPPED);
        else
            selectButton(BADGE_BTN_ALL);
    }
}

/*
    Updates the menu
*/
static void menuMain(PausewinEntry * entry)
{
    (void) entry;

    // Handle inputs based on button selected
    u32 btn = wpadGetButtonsPressed(0);
    switch (CUR_BTN)
    {
        case BADGE_BTN_ALL:
            updateOnAll(btn);
            break;

        case BADGE_BTN_EQUIPPED:
            updateOnEquipped(btn);
            break;

        case BADGE_BTN_BADGES:
            updateOnBadges(btn);
            break;

        default:
            break;
    }
}

/*
    Renders the badge list
*/
static void menuDisp(PausewinEntry * entry)
{
    f32 y = 155.0f;
    for (s32 i = work.pageStart; i < work.pageStart + work.pageBadgeCount; i++)
    {
        s32 slot = menuIdxToBadgeSlot(i);
        BadgeDef * def = pouchGetBadgeDef(slot);
        PouchBadgeInfo * info = pouchGetBadgeInfo(slot);
        
        mod::Window::drawTexture(badgeIconTpl, def->iconId, -105.0f, y, 0.7f, &colours::white);

        const wii::gx::GXColor * colour = info->equipped ? &colours::red : &colours::black;
        mod::Window::drawMessageSearch(def->nameMsg, -60.0f, y, colour, 0.7f);

        y -= BADGE_LINE_HEIGHT;
    }
}

/*
    Closes the badge menu
*/
static void menuClose()
{
    spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_CANCEL1");
    // Delete submenu elements
    for (u32 i = 0; i < ARRAY_SIZEOF(pausewin_pluswinWp->submenuEntryIds); i++)
    {
        if (BADGE_ENTRY_ID(i) != -1)
            pausewinDelete(BADGE_ENTRY_ID(i));
    }
    // Show main menu elements
    for (s32 i = 0; i < 8; i++)
    {
        if (i != PLUSWIN_BTN_BADGES)
            pausewinAppear(MAIN_ENTRY_ID(i));
    }
    pausewinAppear(MAIN_ENTRY_ID(PLUSWIN_BTN_STATS));

    // Restore badge button and cursor
    PausewinEntry * badgeBtn = MAIN_ENTRY(PLUSWIN_BTN_BADGES);
    badgeBtn->flags |= PAUSE_FLAG_OSCILLATE;
    f32 x = badgeBtn->originalPos.x;
    f32 y = badgeBtn->originalPos.y;
    pausewin_pluswinWp->cursorMoveDest.x = x - 25.0f;
    pausewin_pluswinWp->cursorMoveDest.y = y - 30.0f;
    pausewinMoveTo(MAIN_ENTRY_ID(PLUSWIN_BTN_BADGES), x, y);

    // Restore help message
    char str[64];
    sprintf(str, "menu_help_%03d", pausewin_pluswinWp->selectedButton);
    setHelpMessage(str);
}

/*
    Opens the badge menu
*/
static void menuOpen()
{
    badgePouchPatch(1700);
    badgePouchInit();
    // Hide main menu elements
    for (s32 i = 0; i < 8; i++)
    {
        if (i != PLUSWIN_BTN_BADGES)
            pausewinDisappear(MAIN_ENTRY_ID(i));
    }
    pausewinDisappear(MAIN_ENTRY_ID(PLUSWIN_BTN_STATS));

    // Clear element id array (I think this is left un-initialised?)
    for (u32 i = 0; i < ARRAY_SIZEOF(pausewin_pluswinWp->submenuEntryIds); i++)
        BADGE_ENTRY_ID(i) = -1;

    // Create submenu elements
    initElement(
        pausewinEntry(-250.0f, 130.0f, 0.0f, 0.0f, 0, PAUSETEX_BADGES_ALL, 0, nullptr, nullptr,
                      nullptr, nullptr, nullptr),
        BADGE_BTN_ALL
    );
    initElement(
        pausewinEntry(-250.0f, 80.0f, 0.0f, 0.0f, 0, PAUSETEX_BADGES_EQUIPPED, 0, nullptr, nullptr,
                      nullptr, nullptr, nullptr),
        BADGE_BTN_EQUIPPED
    );
    initElement(
        pausewinEntry(-120.0f, 170.0f, 375.0f, 280.0f, 1, PAUSETEX_NONE, 0, nullptr, menuMain,
                      menuDisp, nullptr, nullptr),
        BADGE_BTN_MAIN
    );

    // Select default button
    pausewin_pluswinWp->submenuSelectedButton = -1;
    selectButton(BADGE_BTN_ALL);

    // Move badge button to corner and stop oscillating
    PausewinEntry * bg = MAIN_ENTRY(PLUSWIN_BTN_BG);
    f32 x = bg->pos.x + 10.0f;
    f32 y = bg->pos.y - 10.0f;
    pausewinMoveTo(MAIN_ENTRY_ID(PLUSWIN_BTN_BADGES), x, y);
    MAIN_ENTRY(PLUSWIN_BTN_BADGES)->flags &= ~PAUSE_FLAG_OSCILLATE;

    // Init misc values
    pausewin_pluswinWp->submenuIsFullClose = false;
    initPage(0, false, 0);

    // From key item opening, some of these may be wanted:

    /*
    plusWinWp->field_0x16c = 0;
    plusWinWp->field_0x170 = 0;
    plusWinWp->keyItemEvt = NULL;
    plusWinWp->selectedItemId = 0;
    plusWinWp->submenuFlags = 0;
    */
}

/*
    Public function to apply all patches
*/
void badgeMenuPatch()
{
    spm::filemgr::FileEntry * srcFile = spm::filemgr::fileAllocf(4, "./mod/badgeicon.tpl");
    s32 tplSize = srcFile->length;
    badgeIconTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MEM1_UNUSED, tplSize);
    msl::string::memcpy(badgeIconTpl, srcFile->sp->data, tplSize);
    spm::filemgr::fileFree(srcFile);
    writeBranch(spm::pausewin::pluswinChapterWinOpen, 0, menuOpen);
}

}
