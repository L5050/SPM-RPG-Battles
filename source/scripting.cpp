PTR("BGM_EVT_STG7_RPG1")PTR("BGM_MAP_STG7")#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <spm/evtmgr.h>
#include <spm/evt_msg.h>
#include <spm/evt_fade.h>
#include <evt_snd.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/item_data.h>
#include <spm/item_event_data.h>
#include <wii/os/OSError.h>
#include <patch.h>
#include <string>
using namespace std;
namespace mod {
//functions in eu0, I need to port them to all versions
EVT_BEGIN(beginRPG)
USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_STG7_RPG1"))
USER_FUNC(0x800d34b8, 0)
USER_FUNC(0x8010c600, PTR("OFF_house_02"))
USER_FUNC(0x800ec2c8, PTR("img"))
USER_FUNC(0x800ec3ac, PTR("img"), PTR("OFF_house_02"))
USER_FUNC(0x800ec458, PTR("img"), PTR("A_2"))
USER_FUNC(0x800ec4ec, PTR("img"), 0, 0, 1, 0, 0, 608, 480)
USER_FUNC(0x800ec7e0, PTR("img"), 1)
USER_FUNC(0x800ec920, PTR("img"))
USER_FUNC(0x800e7050, 2, 0, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
INLINE_EVT()
    USER_FUNC(0x80c6d584)
END_INLINE()
USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
USER_FUNC(0x800e7050, 65554, 0, 0, 0, 0, 255)
USER_FUNC(0x800ec7e0, PTR("img"), 0)
USER_FUNC(0x800ec8c4, PTR("img"))
USER_FUNC(0x8010c660, PTR("OFF_house_02"))
USER_FUNC(0x800ee55c, 0)
USER_FUNC(0x800e7050, 1, 300, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
USER_FUNC(0x800fbc64, 0, PTR("stg7_2_133_2_001"))
WAIT_MSEC(2000)
DO()
    USER_FUNC(0x80c72680, LW(0))
    IF_FLAG(LW(0), 0x1000)
        USER_FUNC(0x800d46d8, 0.5)
    END_IF()
    USER_FUNC(0x80c72680, LW(0))
    IF_NOT_FLAG(LW(0), 0x600)
        USER_FUNC(0x80c6d894, LW(0), LW(1), LW(2), LW(3), LW(4))
        SWITCH(LW(0))
            CASE_EQUAL(0)
                RUN_CHILD_EVT(0x80def500)
            CASE_EQUAL(1)
                RUN_CHILD_EVT(0x80def738)
            CASE_EQUAL(2)
                RUN_CHILD_EVT(0x80def9d8)
            CASE_EQUAL(3)
                RUN_CHILD_EVT(0x80df0500)
            CASE_EQUAL(4)
                RUN_CHILD_EVT(0x80df1860)
            CASE_EQUAL(5)
                SET(LF(0), 0)
                RUN_CHILD_EVT(0x80df1958)
                IF_EQUAL(LF(0), 1)
                    DO_BREAK()
                END_IF()
                END_SWITCH()
END_IF()
USER_FUNC(0x800d46d8, 1.0)
RUN_CHILD_EVT(0x80df1b70)
RUN_CHILD_EVT(0x80df1eb0)
RUN_CHILD_EVT(0x80df21f0)
SET(LF(0), 0)
RUN_CHILD_EVT(0x80df2530)
IF_EQUAL(LF(0), 1)
   DO_BREAK()
END_IF()
USER_FUNC(spm::evt_msg::evt_msg_continue)
USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<dq><once_stop>"), 0, 0)
WHILE()
USER_FUNC(0x800e7050, 2, 500, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
USER_FUNC(spm::evt_msg::evt_msg_continue)
USER_FUNC(0x80102eb8, 324, 1)
USER_FUNC(0x80102eb8, 325, 1)
USER_FUNC(0x80102eb8, 326, 1)
USER_FUNC(0x800e71dc, 1)
USER_FUNC(0x80c6d87c)
USER_FUNC(0x800e0430, 0, 11)
USER_FUNC(0x800f1954, PTR("S_1"), 0)
USER_FUNC(0x800ee55c, 1)
USER_FUNC(0x800d46d8, 1.0)
USER_FUNC(0x800e7050, 1, 500, 0, 0, 0, 255)
USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)
IF_EQUAL(LW(0), 0)
USER_FUNC(0x800d21f8, 0)
USER_FUNC(0x800d218c, 0, PTR("BGM_MAP_STG7"), 1000)
USER_FUNC(0x800d3464, 0, PTR("ENV_AN2_08"), 1000)
SET(LW(0), 0)
END_EVT()
END_IF()
IF_EQUAL(LW(0), 1)
SET(LW(0), 1)
END_EVT()
END_IF()
END_EVT()
END_SCRIPT()

}
