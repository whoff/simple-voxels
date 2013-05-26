#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"
#include "Util.h"

#include "Palagyi_table.h"

using namespace std;

namespace NPalagyi {

    const Mask TemplatesU[6] = {
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(7,5,7), BP(7,5,7) ) ),   // M1
        Mask( BV( BP(0,7,7), BP(2,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,2,0), BP(0,2,0) ) ),   // M2
        Mask( BV( BP(0,3,3), BP(2,6,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,6,0), BP(0,2,0) ) ),   // M3
        Mask( BV( BP(7,7,7), BP(4,2,0), BP(0,2,0) ), BV( BP(4,0,0), BP(4,2,0), BP(0,2,0) ) ),   // M4
        Mask( BV( BP(7,7,7), BP(0,2,7), BP(2,2,7) ), BV( BP(0,0,0), BP(0,2,0), BP(2,0,0) ), BV( BP(0,0,0), BP(7,5,0), BP(5,5,0) ) ),   // M5
        Mask( BV( BP(7,7,7), BP(0,3,3), BP(2,7,3) ), BV( BP(0,0,0), BP(0,2,0), BP(2,4,0) ) ),   // M6
    };

    void GenerateHeader( const char* filename ) {
        vector<vector<vector<Mask>>> tables;
        NSymmetry::CreateMaskTablesU( tables, TemplatesU );
        NUtil::GenerateHeader( tables, filename );
    }

    bool IsDeletable( int bits, EFaceDir dir ) {
        for each (const Mask& mask in palagyi_tables[(int)dir]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
