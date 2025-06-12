#define INCLUDE_ASSET(name, filename, section, permission) \
        ".section ."#section"."#name", \""#permission"\"\n" \
        ".balign 32\n" \
        ".global "#name"\n" \
    #name":\n" \
        ".incbin \"../assets/"#filename"\"\n" \
        ".balign 4\n" \
    #name"_end:\n" \
        ".global "#name"_size\n" \
        ".balign 4\n" \
    #name"_size:\n" \
        ".int "#name"_end - "#name"\n"

__asm__ (

INCLUDE_ASSET(allTex, alltex.bin, data, wa)
INCLUDE_ASSET(badgeTex, badgetex.bin, rodata, a)
INCLUDE_ASSET(equippedTex, equippedTex.bin, rodata, a)
//INCLUDE_ASSET(badgeIconTpl, badgeicon.tpl, data, wa)
);
