#include "stdafx.h"

#include "Mask.h"

const MaskZero pxsimple_T26Rx_tables[6][6] = {
    {// dir = 0
        // M1
        MaskZero( 0x2490400, 0x1249200, 0x4924800 ),
        MaskZero( 0x0e05000, 0x01c0e00, 0x7038000 ),
        // M2
        MaskZero( 0x0681400, 0x0040200, 0x793c800 ),
        MaskZero( 0x0c84400, 0x0100800, 0x7279200 ),
        MaskZero( 0x2c14000, 0x4020000, 0x13c9e00 ),
        MaskZero( 0x2611000, 0x1008000, 0x49e4e00 ),
    },
    {// dir = 1
        // M1
        MaskZero( 0x0010492, 0x0009249, 0x0024924 ),
        MaskZero( 0x0005038, 0x00381c0, 0x0000e07 ),
        // M2
        MaskZero( 0x0011098, 0x0008040, 0x0024f27 ),
        MaskZero( 0x00140b0, 0x0020100, 0x0009e4f ),
        MaskZero( 0x0004432, 0x0000804, 0x00393c9 ),
        MaskZero( 0x000141a, 0x0000201, 0x003c9e4 ),
    },
    {// dir = 2
        // M1
        MaskZero( 0x2410090, 0x1209048, 0x4824120 ),
        MaskZero( 0x003d000, 0x7e00000, 0x00001f8 ),
        // M2
        MaskZero( 0x2419000, 0x1200000, 0x48241f8 ),
        MaskZero( 0x2434000, 0x4800000, 0x12091f8 ),
        MaskZero( 0x0034090, 0x0000120, 0x7e09048 ),
        MaskZero( 0x0019090, 0x0000048, 0x7e24120 ),
    },
    {// dir = 3
        // M1
        MaskZero( 0x0480412, 0x0241209, 0x0904824 ),
        MaskZero( 0x0005e00, 0x000003f, 0x0fc0000 ),
        // M2
        MaskZero( 0x0001612, 0x0000009, 0x0fc4824 ),
        MaskZero( 0x0004c12, 0x0000024, 0x0fc1209 ),
        MaskZero( 0x0484c00, 0x0900000, 0x024123f ),
        MaskZero( 0x0481600, 0x0240000, 0x090483f ),
    },
    {// dir = 4
        // M1
        MaskZero( 0x0034c00, 0x6d80000, 0x00001b6 ),
        MaskZero( 0x0c04030, 0x0180c06, 0x6030180 ),
        // M2
        MaskZero( 0x0c04c00, 0x0180000, 0x60301b6 ),
        MaskZero( 0x0004c30, 0x0000006, 0x6db0180 ),
        MaskZero( 0x0034030, 0x0000180, 0x6d80c06 ),
        MaskZero( 0x0c34000, 0x6000000, 0x0180db6 ),
    },
    {// dir = 5
        // M1
        MaskZero( 0x0019600, 0x00000db, 0x36c0000 ),
        MaskZero( 0x0601018, 0x00c0603, 0x30180c0 ),
        // M2
        MaskZero( 0x0001618, 0x0000003, 0x36d80c0 ),
        MaskZero( 0x0601600, 0x00c0000, 0x30180db ),
        MaskZero( 0x0619000, 0x3000000, 0x00c06db ),
        MaskZero( 0x0019018, 0x00000c0, 0x36c0603 ),
    },
};
