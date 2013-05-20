#include "stdafx.h"

#include "Mask.h"

const Mask palagyi_tables[][21] = {
    {// dir = 0
        // M1
        Mask( 0x04021ff, 0x0402000, 0x7bfde00 ),
        // M2
        Mask( 0x041203f, 0x0412000 ),
        Mask( 0x04031b6, 0x0403000 ),
        Mask( 0x04025f8, 0x0402400 ),
        Mask( 0x04060db, 0x0406000 ),
        // M3
        Mask( 0x0413036, 0x0413000 ),
        Mask( 0x041601b, 0x0416000 ),
        Mask( 0x04035b0, 0x0403400 ),
        Mask( 0x04064d8, 0x0406400 ),
        // M4
        Mask( 0x040a1ff, 0x040a040 ),
        Mask( 0x04221ff, 0x0422100 ),
        Mask( 0x04023ff, 0x0402201 ),
        Mask( 0x04029ff, 0x0402804 ),
        // M5
        Mask( 0x25c2fff, 0x2002000, 0x5a3d000 ),
        Mask( 0x4f269ff, 0x0202000, 0x30d9600 ),
        Mask( 0x74ba1ff, 0x0082000, 0x0b45e00 ),
        Mask( 0x1e4b3ff, 0x0802000, 0x61b4c00 ),
        // M6
        Mask( 0x2f86dff, 0x2202000 ),
        Mask( 0x2ec37ff, 0x2802000 ),
        Mask( 0x6eb61ff, 0x0282000 ),
        Mask( 0x3e9b1ff, 0x0882000 ),
    },
    {// dir = 1
        // M1
        Mask( 0x7fc2010, 0x0002010, 0x003dfef ),
        // M2
        Mask( 0x7e02410, 0x0002410 ),
        Mask( 0x6d83010, 0x0003010 ),
        Mask( 0x0fd2010, 0x0012010 ),
        Mask( 0x36c6010, 0x0006010 ),
        // M3
        Mask( 0x6c03410, 0x0003410 ),
        Mask( 0x3606410, 0x0006410 ),
        Mask( 0x0d93010, 0x0013010 ),
        Mask( 0x06d6010, 0x0016010 ),
        // M4
        Mask( 0x7fc2210, 0x0042210 ),
        Mask( 0x7fc2810, 0x0102810 ),
        Mask( 0x7fca010, 0x100a010 ),
        Mask( 0x7fe2010, 0x4022010 ),
        // M5
        Mask( 0x7ffa1d2, 0x0002002, 0x0005e2d ),
        Mask( 0x7fe693c, 0x0002008, 0x00196c3 ),
        Mask( 0x7fc2e97, 0x0002080, 0x003d168 ),
        Mask( 0x7fcb279, 0x0002020, 0x0034d86 ),
        // M6
        Mask( 0x7ff61ba, 0x000200a ),
        Mask( 0x7fdb0fa, 0x0002022 ),
        Mask( 0x7fc6cbe, 0x0002088 ),
        Mask( 0x7fc36bb, 0x00020a0 ),
    },
    {// dir = 2
        // M1
        Mask( 0x01d2e07, 0x0012000, 0x7e2d1f8 ),
        // M2
        Mask( 0x01d2e10, 0x0012010 ),
        Mask( 0x0193c06, 0x0013000 ),
        Mask( 0x0412e07, 0x0412000 ),
        Mask( 0x00d6603, 0x0016000 ),
        // M3
        Mask( 0x0193c10, 0x0013010 ),
        Mask( 0x00d6610, 0x0016010 ),
        Mask( 0x0413c06, 0x0413000 ),
        Mask( 0x0416603, 0x0416000 ),
        // M4
        Mask( 0x01d2e0f, 0x0012009 ),
        Mask( 0x01d2e27, 0x0012024 ),
        Mask( 0x03d2e07, 0x0252000 ),
        Mask( 0x09d2e07, 0x0912000 ),
        // M5
        Mask( 0x7fd2e87, 0x0002080, 0x002d178 ),
        Mask( 0x49fef27, 0x000a000, 0x36010d8 ),
        Mask( 0x21d2fff, 0x2002000, 0x5e2d000 ),
        Mask( 0x13fbe4f, 0x0022000, 0x6c041b0 ),
        // M6
        Mask( 0x6dfee87, 0x000a080 ),
        Mask( 0x37fbe87, 0x0022080 ),
        Mask( 0x21fefb7, 0x200a000 ),
        Mask( 0x21fbedf, 0x2022000 ),
    },
    {// dir = 3
        // M1
        Mask( 0x703a5c0, 0x0002400, 0x0fc5a3f ),
        // M2
        Mask( 0x043a5c0, 0x0402400 ),
        Mask( 0x6033580, 0x0003400 ),
        Mask( 0x703a410, 0x0002410 ),
        Mask( 0x301e4c0, 0x0006400 ),
        // M3
        Mask( 0x0433580, 0x0403400 ),
        Mask( 0x041e4c0, 0x0406400 ),
        Mask( 0x6033410, 0x0003410 ),
        Mask( 0x301e410, 0x0006410 ),
        // M4
        Mask( 0x723a5c0, 0x1202400 ),
        Mask( 0x783a5c0, 0x4802400 ),
        Mask( 0x703a5c8, 0x0002448 ),
        Mask( 0x703a5e0, 0x0002520 ),
        // M5
        Mask( 0x70ba5ff, 0x0082000, 0x0f45a00 ),
        Mask( 0x793efe4, 0x0002200, 0x06c101b ),
        Mask( 0x7ffa5c2, 0x0002002, 0x0005a3d ),
        Mask( 0x727bfc9, 0x0002800, 0x0d84036 ),
        // M6
        Mask( 0x70beff6, 0x0082200 ),
        Mask( 0x70bbfdb, 0x0082800 ),
        Mask( 0x7dbefc2, 0x0002202 ),
        Mask( 0x76fbfc2, 0x0002802 ),
    },
    {// dir = 4
        // M1
        Mask( 0x124f249, 0x0006000, 0x6db0db6 ),
        // M2
        Mask( 0x0257209, 0x0016000 ),
        Mask( 0x040f249, 0x0406000 ),
        Mask( 0x120f448, 0x0006400 ),
        Mask( 0x124f210, 0x0006010 ),
        // M3
        Mask( 0x0417209, 0x0416000 ),
        Mask( 0x0257210, 0x0016010 ),
        Mask( 0x040f448, 0x0406400 ),
        Mask( 0x120f410, 0x0006410 ),
        // M4
        Mask( 0x324f249, 0x3006000 ),
        Mask( 0x124f2c9, 0x00060c0 ),
        Mask( 0x12cf249, 0x00c6000 ),
        Mask( 0x124f24b, 0x0006003 ),
        // M5
        Mask( 0x13efe4f, 0x0022000, 0x6c101b0 ),
        Mask( 0x1a4f3ff, 0x0802000, 0x65b0c00 ),
        Mask( 0x727fbc9, 0x0002800, 0x0d80436 ),
        Mask( 0x7fcf269, 0x0002020, 0x0030d96 ),
        // M6
        Mask( 0x1a6fe7f, 0x0822000 ),
        Mask( 0x1fefe69, 0x0022020 ),
        Mask( 0x1a7fbf9, 0x0802800 ),
        Mask( 0x7e7fa69, 0x0002820 ),
    },
    {// dir = 5
        // M1
        Mask( 0x4927924, 0x0003000, 0x36d86db ),
        // M2
        Mask( 0x0917824, 0x0013000 ),
        Mask( 0x4927810, 0x0003010 ),
        Mask( 0x4827520, 0x0003400 ),
        Mask( 0x0427924, 0x0403000 ),
        // M3
        Mask( 0x0917810, 0x0013010 ),
        Mask( 0x0417824, 0x0413000 ),
        Mask( 0x4827410, 0x0003410 ),
        Mask( 0x0427520, 0x0403400 ),
        // M4
        Mask( 0x49279a4, 0x0003180 ),
        Mask( 0x6927924, 0x6003000 ),
        Mask( 0x4927926, 0x0003006 ),
        Mask( 0x49a7924, 0x0183000 ),
        // M5
        Mask( 0x49eff27, 0x000a000, 0x36100d8 ),
        Mask( 0x7fe792c, 0x0002008, 0x00186d3 ),
        Mask( 0x793fbe4, 0x0002200, 0x06c041b ),
        Mask( 0x4b279ff, 0x0202000, 0x34d8600 ),
        // M6
        Mask( 0x4feff2c, 0x000a008 ),
        Mask( 0x4b2ff3f, 0x020a000 ),
        Mask( 0x7f3fb2c, 0x0002208 ),
        Mask( 0x4b3fbfc, 0x0202200 ),
    },
};
