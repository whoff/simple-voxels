#include "stdafx.h"

#include "Mask.h"

const Mask pxsimple_T26Rx_tables[][6] = {
    {// dir = 0
        // M1
        Mask( BV( BP(0,0,0), BP(2,0,2), BP(2,2,2) ), 0, BV( BP(0,0,0), BP(1,1,1), BP(1,1,1) ), BV( BP(0,0,0), BP(4,4,4), BP(4,4,4) ) ),
        Mask( BV( BP(0,0,0), BP(0,5,0), BP(0,7,0) ), 0, BV( BP(0,0,0), BP(7,0,0), BP(7,0,0) ), BV( BP(0,0,0), BP(0,0,7), BP(0,0,7) ) ),
        // M2
        Mask( BV( BP(0,0,0), BP(2,1,0), BP(2,3,0) ), 0, BV( BP(0,0,0), BP(1,0,0), BP(1,0,0) ), BV( BP(0,0,0), BP(4,4,7), BP(4,4,7) ) ),
        Mask( BV( BP(0,0,0), BP(2,4,0), BP(2,6,0) ), 0, BV( BP(0,0,0), BP(4,0,0), BP(4,0,0) ), BV( BP(0,0,0), BP(1,1,7), BP(1,1,7) ) ),
        Mask( BV( BP(0,0,0), BP(0,4,2), BP(0,6,2) ), 0, BV( BP(0,0,0), BP(0,0,4), BP(0,0,4) ), BV( BP(0,0,0), BP(7,1,1), BP(7,1,1) ) ),
        Mask( BV( BP(0,0,0), BP(0,1,2), BP(0,3,2) ), 0, BV( BP(0,0,0), BP(0,0,1), BP(0,0,1) ), BV( BP(0,0,0), BP(7,4,4), BP(7,4,4) ) ),
    },
    {// dir = 1
        // M1
        Mask( BV( BP(2,2,2), BP(2,0,2), BP(0,0,0) ), 0, BV( BP(1,1,1), BP(1,1,1), BP(0,0,0) ), BV( BP(4,4,4), BP(4,4,4), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,0), BP(0,5,0), BP(0,0,0) ), 0, BV( BP(0,0,7), BP(0,0,7), BP(0,0,0) ), BV( BP(7,0,0), BP(7,0,0), BP(0,0,0) ) ),
        // M2
        Mask( BV( BP(0,3,2), BP(0,1,2), BP(0,0,0) ), 0, BV( BP(0,0,1), BP(0,0,1), BP(0,0,0) ), BV( BP(7,4,4), BP(7,4,4), BP(0,0,0) ) ),
        Mask( BV( BP(0,6,2), BP(0,4,2), BP(0,0,0) ), 0, BV( BP(0,0,4), BP(0,0,4), BP(0,0,0) ), BV( BP(7,1,1), BP(7,1,1), BP(0,0,0) ) ),
        Mask( BV( BP(2,6,0), BP(2,4,0), BP(0,0,0) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(0,0,0) ), BV( BP(1,1,7), BP(1,1,7), BP(0,0,0) ) ),
        Mask( BV( BP(2,3,0), BP(2,1,0), BP(0,0,0) ), 0, BV( BP(1,0,0), BP(1,0,0), BP(0,0,0) ), BV( BP(4,4,7), BP(4,4,7), BP(0,0,0) ) ),
    },
    {// dir = 2
        // M1
        Mask( BV( BP(0,2,2), BP(0,0,2), BP(0,2,2) ), 0, BV( BP(0,1,1), BP(0,1,1), BP(0,1,1) ), BV( BP(0,4,4), BP(0,4,4), BP(0,4,4) ) ),
        Mask( BV( BP(0,0,0), BP(0,5,7), BP(0,0,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,7,7) ), BV( BP(0,7,7), BP(0,0,0), BP(0,0,0) ) ),
        // M2
        Mask( BV( BP(0,0,0), BP(0,1,3), BP(0,2,2) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,1,1) ), BV( BP(0,7,7), BP(0,4,4), BP(0,4,4) ) ),
        Mask( BV( BP(0,0,0), BP(0,4,6), BP(0,2,2) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,4,4) ), BV( BP(0,7,7), BP(0,1,1), BP(0,1,1) ) ),
        Mask( BV( BP(0,2,2), BP(0,4,6), BP(0,0,0) ), 0, BV( BP(0,4,4), BP(0,0,0), BP(0,0,0) ), BV( BP(0,1,1), BP(0,1,1), BP(0,7,7) ) ),
        Mask( BV( BP(0,2,2), BP(0,1,3), BP(0,0,0) ), 0, BV( BP(0,1,1), BP(0,0,0), BP(0,0,0) ), BV( BP(0,4,4), BP(0,4,4), BP(0,7,7) ) ),
    },
    {// dir = 3
        // M1
        Mask( BV( BP(2,2,0), BP(2,0,0), BP(2,2,0) ), 0, BV( BP(1,1,0), BP(1,1,0), BP(1,1,0) ), BV( BP(4,4,0), BP(4,4,0), BP(4,4,0) ) ),
        Mask( BV( BP(0,0,0), BP(7,5,0), BP(0,0,0) ), 0, BV( BP(7,7,0), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,0), BP(0,0,0), BP(7,7,0) ) ),
        // M2
        Mask( BV( BP(2,2,0), BP(3,1,0), BP(0,0,0) ), 0, BV( BP(1,1,0), BP(0,0,0), BP(0,0,0) ), BV( BP(4,4,0), BP(4,4,0), BP(7,7,0) ) ),
        Mask( BV( BP(2,2,0), BP(6,4,0), BP(0,0,0) ), 0, BV( BP(4,4,0), BP(0,0,0), BP(0,0,0) ), BV( BP(1,1,0), BP(1,1,0), BP(7,7,0) ) ),
        Mask( BV( BP(0,0,0), BP(6,4,0), BP(2,2,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(4,4,0) ), BV( BP(7,7,0), BP(1,1,0), BP(1,1,0) ) ),
        Mask( BV( BP(0,0,0), BP(3,1,0), BP(2,2,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(1,1,0) ), BV( BP(7,7,0), BP(4,4,0), BP(4,4,0) ) ),
    },
    {// dir = 4
        // M1
        Mask( BV( BP(0,0,0), BP(6,4,6), BP(0,0,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(6,6,6) ), BV( BP(6,6,6), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,6,0), BP(0,4,0), BP(0,6,0) ), 0, BV( BP(6,0,0), BP(6,0,0), BP(6,0,0) ), BV( BP(0,0,6), BP(0,0,6), BP(0,0,6) ) ),
        // M2
        Mask( BV( BP(0,0,0), BP(6,4,0), BP(0,6,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(6,0,0) ), BV( BP(6,6,6), BP(0,0,6), BP(0,0,6) ) ),
        Mask( BV( BP(0,6,0), BP(6,4,0), BP(0,0,0) ), 0, BV( BP(6,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,6), BP(0,0,6), BP(6,6,6) ) ),
        Mask( BV( BP(0,6,0), BP(0,4,6), BP(0,0,0) ), 0, BV( BP(0,0,6), BP(0,0,0), BP(0,0,0) ), BV( BP(6,0,0), BP(6,0,0), BP(6,6,6) ) ),
        Mask( BV( BP(0,0,0), BP(0,4,6), BP(0,6,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,0,6) ), BV( BP(6,6,6), BP(6,0,0), BP(6,0,0) ) ),
    },
    {// dir = 5
        // M1
        Mask( BV( BP(0,0,0), BP(3,1,3), BP(0,0,0) ), 0, BV( BP(3,3,3), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,0), BP(0,0,0), BP(3,3,3) ) ),
        Mask( BV( BP(0,3,0), BP(0,1,0), BP(0,3,0) ), 0, BV( BP(3,0,0), BP(3,0,0), BP(3,0,0) ), BV( BP(0,0,3), BP(0,0,3), BP(0,0,3) ) ),
        // M2
        Mask( BV( BP(0,3,0), BP(3,1,0), BP(0,0,0) ), 0, BV( BP(3,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,3), BP(0,0,3), BP(3,3,3) ) ),
        Mask( BV( BP(0,0,0), BP(3,1,0), BP(0,3,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(3,0,0) ), BV( BP(3,3,3), BP(0,0,3), BP(0,0,3) ) ),
        Mask( BV( BP(0,0,0), BP(0,1,3), BP(0,3,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,0,3) ), BV( BP(3,3,3), BP(3,0,0), BP(3,0,0) ) ),
        Mask( BV( BP(0,3,0), BP(0,1,3), BP(0,0,0) ), 0, BV( BP(0,0,3), BP(0,0,0), BP(0,0,0) ), BV( BP(3,0,0), BP(3,0,0), BP(3,3,3) ) ),
    },
};
