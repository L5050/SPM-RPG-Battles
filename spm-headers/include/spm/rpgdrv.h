#pragma once

#include <common.h>

CPP_WRAPPER(spm::rpgdrv)

typedef struct rpgNPC {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 maxHp;
    /* 0x0C */ s32 attackStrength;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 killXp;
} rpgUnk;
SIZE_ASSERT(rpgNPC, 0x18);

typedef struct rpgMenu {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ char unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ void* unk_08;
    /* 0x0C */ rpgNPC rpgNpcInfo[3]; /* 0x18 * 3 in size */
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ s32 unk_58[15];
    /* 0x94 */ char unk_94[4];
} rpgMenu; //sizeof 0x98
SIZE_ASSERT(rpgMenu, 0x98);

typedef struct physicalMenu {
    /* 0x00 */ char * option_1;
    /* 0x04 */ char * unk_4;
    /* 0x08 */ char * option_2;
    /* 0x0c */ char * unk_c;
    /* 0x10 */ char * option_3;
    /* 0x14 */ char * unk_14;
    /* 0x18 */ char * option_4;
    /* 0x1c */ char * unk_1c;
    /* 0x20 */ char * option_5;
    /* 0x24 */ char * unk_24;
    /* 0x28 */ char * option_6;
    /* 0x2c */ char * unk_2c;
    /* 0x30 */ char * unk_30;
} physicalMenu;
SIZE_ASSERT(physicalMenu, 0x34);

u8 rpg_handle_menu(int param_1, physicalMenu menu);

CPP_WRAPPER_END()