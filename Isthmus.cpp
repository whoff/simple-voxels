#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"
#include "Util.h"

#include "Raynal_table.h"
#include "Isthmus_table.h"

using namespace std;

namespace NRaynal {

    const Mask TemplatesU[7] = {
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ) ),   // M1'
        Mask( BV( BP(0,7,7), BP(2,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,2,0), BP(0,2,0) ) ),   // M2'
        Mask( BV( BP(0,3,3), BP(2,6,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,6,0), BP(0,2,0) ) ),   // M3'
        Mask( BV( BP(7,7,7), BP(4,2,0), BP(0,2,0) ), BV( BP(4,0,0), BP(4,2,0), BP(0,2,0) ) ),   // M4'
        Mask( BV( BP(7,7,7), BP(0,2,7), BP(2,2,7) ), BV( BP(0,0,0), BP(0,2,0), BP(2,0,0) ) ),   // M5'
        Mask( BV( BP(7,7,7), BP(0,3,3), BP(2,7,3) ), BV( BP(0,0,0), BP(0,2,0), BP(2,4,0) ) ),   // M6'
        Mask( BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,1) ) ),   // M7'
    };

    void GenerateHeader( const char* filename ) {
        vector<vector<vector<Mask>>> tables;
        NSymmetry::CreateMaskTablesU( tables, TemplatesU );
        NUtil::GenerateHeader( tables, filename );
    }

    bool IsDeletable( int bits, EFaceDir dir ) {
        for each (const Mask& mask in raynal_tables[(int)dir]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}

namespace NIsthmus {

    const Mask TemplatesU[6] = {
        Mask( BV( BP(0,0,0), BP(7,5,7), BP(0,0,0) ), 0, BV( BP(7,7,7), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,0), BP(0,0,0), BP(7,7,7) ) ),
        Mask( BV( BP(0,7,0), BP(7,5,0), BP(0,0,0) ), 0, BV( BP(7,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,7), BP(0,0,7), BP(7,7,7) ) ),
        Mask( BV( BP(2,6,0), BP(6,4,0), BP(0,0,0) ), 0, BV( BP(4,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(1,1,7), BP(1,1,7), BP(7,7,7) ) ),
        Mask( BV( BP(2,6,0), BP(3,5,7), BP(0,0,0) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(7,7,7) ), BV( BP(1,1,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(2,6,0), BP(3,5,6), BP(0,3,2) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(7,4,4) ), BV( BP(1,1,7), BP(0,0,1), BP(0,0,1) ) ),
        Mask( BV( BP(2,6,0), BP(2,5,7), BP(2,3,0) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(4,4,7) ), BV( BP(1,1,7), BP(1,0,0), BP(1,0,0) ) ),
    };

    void GenerateHeader( const char* filename ) {
        vector<vector<vector<Mask>>> tables;
        NSymmetry::CreateMaskTablesAll( tables, TemplatesU );
        NUtil::GenerateHeader( tables, filename );
    }

    bool IsIsthmus( int bits ) {
        for each (const Mask& mask in isthmus_table) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}

namespace NIsthmus {
    namespace NDevelop {
        void Veryfy_IsIsthmus( const vector<uint8_t>& isthmus ) {
            for (int bits = 0; bits < (1 << 27); ++bits) {
                if ((bits & N0_bits) == 0) continue;
                const bool is_isthmus = (isthmus[bits] & 1);
                const bool hit = IsIsthmus( bits );
                if (hit != is_isthmus) {
                    printf( "ERROR!! hit = %d, is_isthmus = %d\n", (int) hit, (int) is_isthmus );
                    DumpBits( bits );
                    return;
                }
            }
            printf( __FUNCTION__ ": Pass!\n" );
        }

        void Verify_Templates( const vector<uint8_t>& primary, const vector<uint8_t>& isthmus ) {
            int cnt_hit = 0;
            int cnt_miss = 0;
            int syms[NSymmetry::NUM_SYM_ALL];
            int max_bits = 0;
            int max_numbits = 0;
            for (int bits = 0; bits < (1 << 27); ++bits) {
                if ((bits & N0_bits) == 0) continue;
                if ((primary[bits] & EP_ALL) == 0) continue;    // check only primary configurations
                const bool is_isthmus = (isthmus[bits] & 1);
                NSymmetry::TransformBitsAll( bits, syms );      // generate 48 symmetries
                bool hit = false;
                for each (int sym in syms) {
                    hit = IsIsthmus( sym );
                    if (hit) break;
                }
                if (hit && !is_isthmus) {
                    printf( "ERROR!! hit = %d, is_isthmus = %d\n", (int) hit, (int) is_isthmus );
                    DumpBits( bits & N26_bits );
                    return;
                }
                if (is_isthmus) {
                    if (hit) {
                        cnt_hit++;
                    } else {
                        cnt_miss++;
                        const int numbits = CountBits( bits );
                        if (max_numbits < numbits) {
                            max_numbits = numbits;
                            max_bits = bits;
                        }
                    }
                }
            }
            if (max_bits) {
                DumpBits( max_bits );
            }
            const int cnt_total = cnt_hit + cnt_miss;
            printf( __FUNCTION__ ": hit / total = %d / %d (%.3f%%), miss = %d\n", cnt_hit, cnt_total, 100.0 * cnt_hit / cnt_total, cnt_miss );
        }
    }
}
