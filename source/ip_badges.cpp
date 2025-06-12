#include "ip_badges.h"

#include <common.h>

namespace ip {

BadgeDef badgeDefs[NUM_BADGES] = {
    /*
        nameMsg
        descMsg
        iconId
        bpCost
    */
    // BADGE_TEST_1
    {
        "bn_test_1",
        "bd_test_1",
        0,
        3
    },
    // BADGE_TEST_2
    {
        "bn_test_2",
        "bd_test_2",
        1,
        1
    },
    // BADGE_TEST_3
    {
        "bn_test_3",
        "bd_test_3",
        2,
        0
    },
};

}
