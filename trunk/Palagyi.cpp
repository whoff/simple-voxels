#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"

#include "Palagyi_table.h"

using namespace std;

namespace NPalagyi {

    const Mask Templates[6] = {
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(7,5,7), BP(7,5,7) ) ),   // M1
        Mask( BV( BP(7,7,0), BP(0,2,2), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,2), BP(0,2,0) ) ),   // M2
        Mask( BV( BP(6,6,0), BP(0,3,2), BP(0,2,0) ), BV( BP(0,0,0), BP(0,3,2), BP(0,2,0) ) ),   // M3
        Mask( BV( BP(7,7,7), BP(0,2,1), BP(0,2,0) ), BV( BP(0,0,1), BP(0,2,1), BP(0,2,0) ) ),   // M4
        Mask( BV( BP(7,7,7), BP(7,2,0), BP(7,2,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,2) ), BV( BP(0,0,0), BP(0,5,7), BP(0,5,5) ) ),   // M5
        Mask( BV( BP(7,7,7), BP(6,6,0), BP(6,7,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,1,2) ) ),   // M6
    };

    bool IsDeletable( int bits, EDir6 dir ) {
        for each (const Mask& mask in palagyi_tables[(int)dir]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
