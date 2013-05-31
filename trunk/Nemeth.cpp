#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"
#include "Util.h"

#include "Nemeth_table.h"

using namespace std;
using namespace std::placeholders;

namespace NNemeth {

    const Mask TemplatesU[3] = {
        Mask( BV( BP(0,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(0,2,0), BP(0,2,0), BP(0,0,0) ), BV( BP(7,5,7), BP(0,0,0), BP(0,0,0) ) ),   // U0
        Mask( BV( BP(1,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(1,2,0), BP(1,2,0), BP(0,0,0) ), BV( BP(6,5,7), BP(0,0,0), BP(0,0,0) ) ),   // U1
        Mask( BV( BP(2,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(2,2,0), BP(2,2,0), BP(0,0,0) ), BV( BP(5,5,7), BP(0,0,0), BP(0,0,0) ) ),   // U2
    };

    const Mask TemplatesUN[1] = {
        Mask( BV( BP(2,0,7), BP(0,7,7), BP(7,7,7) ), BV( BP(2,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(5,7,0), BP(7,0,0), BP(0,0,0) ) ),   // UN
    };

    const Mask TemplatesUNW[1] = {
        Mask( BV( BP(5,4,7), BP(4,6,7), BP(7,7,7) ), BV( BP(1,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(2,3,0), BP(3,1,0), BP(0,0,0) ) ),   // UNW
    };

    void GenerateHeader( const char* filename ) {
        using namespace NSymmetry;
        vector<vector<Mask>> table;
        {
            const EFaceDir dirs[] = { ED_U, ED_N, ED_W };
            vector<vector<Mask>> _table;
            InstantiateTemplates( _table, TemplatesU, NPerm::PermSymU );
            for each (const vector<Mask>& _masks in _table) {
                vector<Mask> masks;
                for each (const Mask& tmpl in _masks) {
                    for each (EFaceDir dir in dirs) {
                        const Mask mask = TransformMask( tmpl, NPerm::PermDirectU[dir] );
                        if (std::find( masks.begin(), masks.end(), mask ) == masks.end()) {
                            masks.push_back( mask );
                        }
                    }
                }
                table.push_back( masks );
            }
        }
        for each (const Mask& tmpl in TemplatesUN) {
            const EEdgeDir dirs[] = { ED_UN, ED_UE, ED_US, ED_UW, ED_NW, ED_NE };
            vector<Mask> masks;
            for each (EEdgeDir dir in dirs) {
                const Mask mask = TransformMask( tmpl, NPerm::PermDirectUN[dir] );
                if (std::find( masks.begin(), masks.end(), mask ) == masks.end()) {
                    masks.push_back( mask );
                }
            }
            table.push_back( masks );
        }
        for each (const Mask& tmpl in TemplatesUNW) {
            const ECornerDir dirs[] = { ED_UNW, ED_UNE, ED_USE, ED_USW };
            vector<Mask> masks;
            for each (ECornerDir dir in dirs) {
                const Mask mask = TransformMask( tmpl, NPerm::PermDirectUNW[dir] );
                if (std::find( masks.begin(), masks.end(), mask ) == masks.end()) {
                    masks.push_back( mask );
                }
            }
            table.push_back( masks );
        }
        vector<vector<vector<Mask>>> tables;
        TransformMaskTable( tables, table, NPerm::PermFlipXYZ );
        NUtil::GenerateHeader( tables, "Nemeth.h" );
    }

    bool IsDeletable( int bits, int idx ) {
        for each (const Mask& mask in nemeth_tables[(int)idx]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
