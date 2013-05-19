#include "stdafx.h"

#include "Mask.h"

const MaskZero isthmus_tables[63] = {
    // M1
    MaskZero( 0x003de00, 0x00001ff, 0x7fc0000 ),
    MaskZero( 0x0e05038, 0x01c0e07, 0x70381c0 ),
    MaskZero( 0x2490492, 0x1249249, 0x4924924 ),
    // M2
    MaskZero( 0x0005e38, 0x0000007, 0x7ff81c0 ),
    MaskZero( 0x0034c92, 0x0000124, 0x7fc9249 ),
    MaskZero( 0x003d038, 0x00001c0, 0x7fc0e07 ),
    MaskZero( 0x0019692, 0x0000049, 0x7fe4924 ),
    MaskZero( 0x0e3d000, 0x7000000, 0x01c0fff ),
    MaskZero( 0x24b4c00, 0x4900000, 0x12493ff ),
    MaskZero( 0x0e05e00, 0x01c0000, 0x70381ff ),
    MaskZero( 0x2499600, 0x1240000, 0x49249ff ),
    MaskZero( 0x0c84432, 0x0100804, 0x72793c9 ),
    MaskZero( 0x068141a, 0x0040201, 0x793c9e4 ),
    MaskZero( 0x2c140b0, 0x4020100, 0x13c9e4f ),
    MaskZero( 0x2611098, 0x1008040, 0x49e4f27 ),
    // M3
    MaskZero( 0x0004c32, 0x0000004, 0x7ff93c9 ),
    MaskZero( 0x000161a, 0x0000001, 0x7ffc9e4 ),
    MaskZero( 0x00340b0, 0x0000100, 0x7fc9e4f ),
    MaskZero( 0x0019098, 0x0000040, 0x7fe4f27 ),
    MaskZero( 0x2c34000, 0x4000000, 0x13c9fff ),
    MaskZero( 0x2619000, 0x1000000, 0x49e4fff ),
    MaskZero( 0x0c84c00, 0x0100000, 0x72793ff ),
    MaskZero( 0x0681600, 0x0040000, 0x793c9ff ),
    // M4
    MaskZero( 0x003d632, 0x7fc0804, 0x00001c9 ),
    MaskZero( 0x003dc1a, 0x7fc0201, 0x00001e4 ),
    MaskZero( 0x001deb0, 0x7fe0100, 0x000004f ),
    MaskZero( 0x0035e98, 0x7fc8040, 0x0000127 ),
    MaskZero( 0x2c1de00, 0x40201ff, 0x13c0000 ),
    MaskZero( 0x2635e00, 0x10081ff, 0x49c0000 ),
    MaskZero( 0x0cbd600, 0x01009ff, 0x7240000 ),
    MaskZero( 0x06bdc00, 0x00403ff, 0x7900000 ),
    MaskZero( 0x0685c38, 0x79381c0, 0x0040207 ),
    MaskZero( 0x0c85638, 0x72781c0, 0x0100807 ),
    MaskZero( 0x0e05c1a, 0x70381e4, 0x01c0201 ),
    MaskZero( 0x0e05632, 0x70381c9, 0x01c0804 ),
    MaskZero( 0x0e35098, 0x01c0f27, 0x7008040 ),
    MaskZero( 0x0e1d0b0, 0x01c0e4f, 0x7020100 ),
    MaskZero( 0x2635038, 0x49c0e07, 0x10081c0 ),
    MaskZero( 0x2c1d038, 0x13c0e07, 0x40201c0 ),
    MaskZero( 0x2491698, 0x4924927, 0x1248040 ),
    MaskZero( 0x2611692, 0x49e4924, 0x1008049 ),
    MaskZero( 0x249941a, 0x49249e4, 0x1240201 ),
    MaskZero( 0x0699492, 0x7924924, 0x0040249 ),
    MaskZero( 0x2c14c92, 0x13c9249, 0x4020124 ),
    MaskZero( 0x2494cb0, 0x124924f, 0x4920100 ),
    MaskZero( 0x0cb4492, 0x7249249, 0x0100924 ),
    MaskZero( 0x24b4432, 0x12493c9, 0x4900804 ),
    // M5
    MaskZero( 0x2635632, 0x49c0804, 0x10081c9 ),
    MaskZero( 0x2c1dc1a, 0x13c0201, 0x40201e4 ),
    MaskZero( 0x069dcb0, 0x7920100, 0x004024f ),
    MaskZero( 0x0cb5698, 0x7248040, 0x0100927 ),
    // M6
    MaskZero( 0x06bd432, 0x7900804, 0x00403c9 ),
    MaskZero( 0x0cbd41a, 0x7240201, 0x01009e4 ),
    MaskZero( 0x0c9d6b0, 0x7260100, 0x010084f ),
    MaskZero( 0x2c1d632, 0x13c0804, 0x40201c9 ),
    MaskZero( 0x2c15e98, 0x13c8040, 0x4020127 ),
    MaskZero( 0x2615eb0, 0x49e0100, 0x100804f ),
    MaskZero( 0x2635c1a, 0x49c0201, 0x10081e4 ),
    MaskZero( 0x06b5c98, 0x7908040, 0x0040327 ),
    MaskZero( 0x249dc38, 0x49201c0, 0x1240207 ),
    MaskZero( 0x24b5638, 0x12481c0, 0x4900807 ),
    MaskZero( 0x0e35692, 0x7008049, 0x01c0924 ),
    MaskZero( 0x0e1dc92, 0x7020124, 0x01c0249 ),
};