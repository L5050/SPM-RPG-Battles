#include "patch.h"
#include "util.h"

#include <common.h>
#include <spm/msgdrv.h>
#include <msl/string.h>

namespace ip {

/*
    Custom Message Data
*/
struct CustomMessage
{
    const char * name;
    const char * msg;
};

static CustomMessage msgs[]
{
    // UI
    {
        "menu_help_004",
        "Equip or dequip your badges and\n"
        "view their effects."
    },
    {
        "menu_help_badges_equipped",
        "View all of the badges you have\n"
        "in your inventory."
    },
    {
        "menu_help_badges_all",
        "View the badges that you have\n"
        "currently equipped."
    },

    // Badge Names
    {
        "bn_test_1",
        "HP Plus"
    },
    {
        "bn_test_2",
        "Power Jump"
    },
    {
        "bn_test_3",
        "Sleepy Stomp"
    },

    // Badge Descriptions
    {
        "bd_test_1",
        "Raises max HP by 5\n"
        "costs 3 BP"
    },
    {
        "bd_test_2",
        "Testing badge 2"
    },
    {
        "bd_test_3",
        "Testing badge 3"
    },
};

/*
    Public function to apply all patches
*/

const char * messagePatch(const char * msgName)
{
  for (u32 i = 0; i < ARRAY_SIZEOF(msgs); i++)
  {
    if (msl::string::strcmp(msgs[i].name, msgName) == 0)
      return msgs[i].msg;
  }
  return nullptr;
}

}
