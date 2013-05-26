#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"
#include "Util.h"

#include "PxSimple_table.h"

using namespace std;

const int MaskW = BV( BP(3,3,3), BP(3,3,3), BP(3,3,3) );    // West
const int MaskE = BV( BP(6,6,6), BP(6,6,6), BP(6,6,6) );    // East
const int MaskN = BV( BP(7,7,0), BP(7,7,0), BP(7,7,0) );    // North
const int MaskS = BV( BP(0,7,7), BP(0,7,7), BP(0,7,7) );    // South
//const int MaskU = BV( BP(7,7,7), BP(7,7,7), BP(0,0,0) );    // Up
//const int MaskD = BV( BP(0,0,0), BP(7,7,7), BP(7,7,7) );    // Down

namespace NPxSimple {// Conditions

    // leave the foreground voxels that are P2x-simple (reference implementation)
    Neibors CalcP2x_ref( Neibors neib ) {
        Neibors Px;
        for (int z = -1; z <= 1; ++z) {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    const int idx = getIndex( x, y, z );
                    if (neib.Read( idx ) == false) continue;
                    if (z >= 0) {
                        if (neib.Read( idx - DZ )) continue;
                        if (x >= 0 && neib.Read( idx - DX ) == false && neib.Read( idx - DX - DZ )) continue;
                        if (x <= 0 && neib.Read( idx + DX ) == false && neib.Read( idx + DX - DZ )) continue;
                        if (y >= 0 && neib.Read( idx - DY ) == false && neib.Read( idx - DY - DZ )) continue;
                        if (y <= 0 && neib.Read( idx + DY ) == false && neib.Read( idx + DY - DZ )) continue;
                    }
                    Px.Write( idx, true );
                }
            }
        }
        return Px;
    }

    // leave the foreground voxels that are P2x-simple
    int CalcP2x( int bits ) {
        const int top = bits << DZ;
        const int mid = top & ~bits;
        const int rx = top | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX) | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY);
        return bits & ~rx;
    }

    void Verify_CalcP2x() {
        for (int bits = 0; bits < (1 << 27); ++bits) {
            if ((bits & N0_bits) == 0) continue;
            const int ref = CalcP2x_ref( Neibors( bits ) ).bits;
            const int opt = CalcP2x( bits );
            if (ref != opt) {
                printf( "ERROR!\n" );
                DumpBits( bits );
                printf( "Ref P2x\n" );
                DumpBits( ref );
                printf( "Opt P2x\n" );
                DumpBits( opt );
                return;
            }
        }
        printf( "CalcP2x() is OK\n" );
    }

    uint8_t CheckConditions( int bits ) {
        uint8_t ret = 0;
        const Neibors neib( bits );
        Neibors Px = CalcP2x_ref( neib );
        if (Px.Read( Coord( 0, 0, 0 ) )) {
            ret |= ELC_P2;
        }
        const Neibors Rx( bits & ~Px.bits );
        // Conditions 1: T_26(x, Rx) = 1
        if (NComponent::CountForeCompsN26( Rx.bits ) == 1) {
            ret |= ELC_T26;
        }
        // Conditions 2: T_6(x, X~) = 1
        if (NComponent::CountBackCompsN6( bits ) == 1) {
            ret |= ELC_T6;
        }
        {// Condition 3: For all y in N_26* & Px, there exists z in Rx such that z is 26-adjacent to x and to y
            bool pass = true;
            for each (const Coord& y in N26) {
                if (Px.Read( y ) == false) continue;
                bool hit = false;
                for each (const Coord& d in N26) {
                    const Coord t = y + d;
                    if (t.IsValid() == false) continue;
                    if (t.x == 0 && t.y == 0 && t.z == 0) continue;
                    if (Rx.Read( t )) {
                        hit = true;
                        break;
                    }
                }
                if (hit == false) {
                    pass = false;
                    break;
                }
            }
            if (pass) {
                ret |= ELC_C26;
            }
        }
        {// Condition 4: For all y in N_6* & Px, there exist z and t in X~ such that {x, y, z, t} is a unit square
            bool pass = true;
            for each (const Coord& y in N6) {
                if (Px.Read( y ) == false) continue;
                bool hit = false;
                for each (const Coord& z in N6) {
                    if (neib.Read( z )) continue;
                    const Coord t = y + z;
                    if (t.IsValid() == false) continue;
                    if (t.x == 0 && t.y == 0 && t.z == 0) continue;
                    if (neib.Read( t ) == false) {
                        hit = true;
                        break;
                    }
                }
                if (hit == false) {
                    pass = false;
                    break;
                }
            }
            if (pass) {
                ret |= ELC_C6;
            }
        }
        if ((ret & ELC_P2) && (ret & ELC_T26) && (ret & ELC_T6) && (ret & ELC_C26) && (ret & ELC_C6)) {
            ret |= ELC_Simple;
        }
        return ret;
    }

    void CheckConditionsUDir( vector<uint8_t>& pxcond ) {
        pxcond.resize( 1 << 27, 0 );
        int syms[NSymmetry::NUM_SYM_U];
        for (int bits = 0; bits < (1 << 27); ++bits) {
            if ((bits & N0_bits) == 0) continue;
            if (pxcond[bits]) continue;   // already set
            const uint8_t ret = CheckConditions( bits );
            NSymmetry::TransformBitsU( bits, syms );
            for each (int sym in syms) {
                pxcond[sym] = ret;
            }
        }
    }
}

namespace NPxSimple {// P2x-simple

    const int MaskT6N4 = BV( BP(0,0,0), BP(2,5,2), BP(0,0,0) );
    const int MaskBtm  = BV( BP(0,0,0), BP(0,0,0), BP(0,2,0) );

    //const Mask TemplatesT6[2] = {
    //    Mask( BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ) ),
    //    Mask( BV( BP(0,0,0), BP(2,2,0), BP(2,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,0) ) ),
    //};

    const Mask TemplatesT26Rx[2] = {
        Mask( BV( BP(0,0,0), BP(2,0,2), BP(2,2,2) ), 0, BV( BP(0,0,0), BP(1,1,1), BP(1,1,1) ), BV( BP(0,0,0), BP(4,4,4), BP(4,4,4) ) ),
        Mask( BV( BP(0,0,0), BP(2,1,0), BP(2,3,0) ), 0, BV( BP(0,0,0), BP(1,0,0), BP(1,0,0) ), BV( BP(0,0,0), BP(4,4,7), BP(4,4,7) ) ),
    };

    void GenerateHeader( const char* filename ) {
        vector<vector<vector<Mask>>> tables;
        NSymmetry::CreateMaskTablesU( tables, TemplatesT26Rx );
        NUtil::GenerateHeader( tables, filename );
    }

    bool IsP2xSimpleU_ref( int bits ) {
        const int px = CalcP2x( bits );
        {// x is P2
            if ((px & N0_bits) == 0) return false;
        }
        const bool WhiteBottom = ((bits & MaskBtm) == 0);
        {// T6
            if (WhiteBottom) {
                const int n4 = ~(bits | (bits >> DZ));
                if ((n4 & MaskT6N4) == 0) return false;
            }
        }
        const int rx = bits & ~px;
        {// C26
            int rn = rx;    // N26( rx )
            rn = rn | ((rn & MaskE) >> DX) | ((rn & MaskW) << DX);
            rn = rn | ((rn & MaskS) >> DY) | ((rn & MaskN) << DY);
            rn = rn | (rn >> DZ) | (rn << DZ);
            if (~rn & px & ~N0_bits) return false;
        }
        {// T26
            if (rx == 0) return false;
            if (WhiteBottom) {
                int syms[NSymmetry::NUM_SYM_U];
                NSymmetry::TransformBitsU( rx, syms );
                for each (const Mask& mask in TemplatesT26Rx) {
                    for each (int sym in syms) {
                        if (mask.Match( sym )) return false;
                    }
                }
            }
        }
        return true;
    }

    bool IsP2xSimpleU( int bits ) {
        const int top = (bits << DZ);
        const int mid = (bits << DZ) & ~bits;
        const int rx = bits & (top | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX) | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY));
        const int px = bits & ~rx;
        if (rx == 0) return false;              // T26
        if ((px & N0_bits) == 0) return false;  // x is P2?
        {// C26
            int rn = rx;    // N26(Rx)
            rn = rn | ((rn & MaskE) >> DX) | ((rn & MaskW) << DX);
            rn = rn | ((rn & MaskS) >> DY) | ((rn & MaskN) << DY);
            rn = rn | (rn >> DZ) | (rn << DZ);
            if (~rn & px & ~N0_bits) return false;
        }
        if ((bits & MaskBtm) == 0) {
            {// T6
                const int n4 = ~(bits | (bits >> DZ));
                if ((n4 & MaskT6N4) == 0) return false;
            }
            {// T26
                int syms[NSymmetry::NUM_ROTATE_U];
                NSymmetry::RotateBitsU( rx, syms );
                for each (const Mask& mask in TemplatesT26Rx) {
                    for each (int sym in syms) {
                        if (mask.Match( sym )) return false;
                    }
                }
            }
        }
        return true;
    }

    void Verify_IsP2xSimpleU( const vector<uint8_t>& pxcond ) {
        int count = 0;
        for (int bits = 0; bits < (1 << 27); ++bits) {
            if ((bits & N0_bits) == 0) continue;
            const uint8_t cond = pxcond[bits];
            const bool ref = IsP2xSimpleU_ref( bits );
            if (ref != (cond & ELC_Simple)) {
                printf( "ERROR! cond = %d, ref = 0x%02x\n", (int) cond, ref );
                DumpBits( bits );
                return;
            }
            const bool test = IsP2xSimpleU( bits );
            if (ref != test) {
                printf( "ERROR! ref = %d, test = %d\n", (int) ref, (int) test );
                DumpBits( bits );
                return;
            }
            if (ref) {
                count++;
            }
        }
        printf( "%s: PASS (count = %d)\n", __FUNCTION__, count );
    }

    bool IsDeletable( int bits, EFaceDir dir ) {
        int top, btm;
        int neib;
        int maskT6N4;
        if (dir == ED_U || dir == ED_D) {
            top = (dir == ED_U) ? (bits << DZ) : (bits >> DZ);
            btm = (dir == ED_U) ? (bits >> DZ) : (bits << DZ);
            const int mid = top & ~bits;
            neib = top | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX) | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY);
            maskT6N4 = BV( BP(0,0,0), BP(2,5,2), BP(0,0,0) );
        } else if (dir == ED_N || dir == ED_S) {
            top = (dir == ED_N) ? ((bits & MaskN) << DY) : ((bits & MaskS) >> DY);
            btm = (dir == ED_N) ? ((bits & MaskS) >> DY) : ((bits & MaskN) << DY);
            const int mid = top & ~bits;
            neib = top | (mid >> DZ) | (mid << DZ) | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX);
            maskT6N4 = BV( BP(0,2,0), BP(0,5,0), BP(0,2,0) );
        } else if (dir == ED_W || dir == ED_E) {
            top = (dir == ED_W) ? ((bits & MaskW) << DX) : ((bits & MaskE) >> DX);
            btm = (dir == ED_W) ? ((bits & MaskE) >> DX) : ((bits & MaskW) << DX);
            const int mid = top & ~bits;
            neib = top | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY) | (mid >> DZ) | (mid << DZ);
            maskT6N4 = BV( BP(0,2,0), BP(2,0,2), BP(0,2,0) );
        } else {
            return false;
        }
        const int rx = bits & neib;
        if (rx == 0) return false;              // T26
        const int px = bits & ~rx;
        if ((px & N0_bits) == 0) return false;  // x is P2?
        {// C26
            int rn = rx;    // N26(Rx)
            rn = rn | ((rn & MaskE) >> DX) | ((rn & MaskW) << DX);
            rn = rn | ((rn & MaskS) >> DY) | ((rn & MaskN) << DY);
            rn = rn | (rn >> DZ) | (rn << DZ);
            if (~rn & px & ~N0_bits) return false;
        }
        if ((btm & N0_bits) == 0) {
            {// T6
                const int n4 = ~(bits | btm);
                if ((n4 & maskT6N4) == 0) return false;
            }
            {// T26
                for each (const Mask& mask in pxsimple_T26Rx_tables[dir]) {
                    if (mask.Match( rx )) return false;
                }
            }
        }
        return true;
    }
}
