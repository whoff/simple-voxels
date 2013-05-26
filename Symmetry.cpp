#include "stdafx.h"
#include "Neighbors.h"
#include "Util.h"

#include "Symmetry.h"

using namespace std;
using namespace std::placeholders;

namespace NSymmetry {
    namespace NPerm {
        Perm PermIdent[1];
        Perm PermFlipX[NUM_FLIP];
        Perm PermFlipXY[NUM_FLIP];
        Perm PermFlipXYZ[NUM_FLIP];
        Perm PermRotateU[NUM_ROTATE_U];
        Perm PermRotateUN[NUM_ROTATE_UN];
        Perm PermRotateUNW[NUM_ROTATE_UNW];
        Perm PermDirectU[NUM_DIRECT_U];
        Perm PermDirectUN[NUM_DIRECT_UN];
        Perm PermDirectUNW[NUM_DIRECT_UNW];
        Perm PermSymU[NUM_SYM_U];
        Perm PermSymUN[NUM_SYM_UN];
        Perm PermSymUNW[NUM_SYM_UNW];
        Perm PermSymAll[NUM_SYM_ALL];

        // flip along x-axis
        void flipX( Perm flp[NUM_FLIP] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        flp[0][idx] = getIndex(  x, y, z );
                        flp[1][idx] = getIndex( -x, y, z );
                    }
                }
            }
        }

        // flip about x = y plane
        void flipXY( Perm flp[NUM_FLIP] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        flp[0][idx] = getIndex( x, y, z );
                        flp[1][idx] = getIndex( y, x, z );
                    }
                }
            }
        }

        // flip all axis
        void flipXYZ( Perm flp[NUM_FLIP] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        flp[0][idx] = getIndex(  x,  y,  z );
                        flp[1][idx] = getIndex( -x, -y, -z );
                    }
                }
            }
        }

        // 4 rotation around U face
        void rotateU( Perm rot[NUM_ROTATE_U] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        rot[0][idx] = getIndex(  x,  y, z );
                        rot[1][idx] = getIndex( -y,  x, z );
                        rot[2][idx] = getIndex( -x, -y, z );
                        rot[3][idx] = getIndex(  y, -x, z );
                    }
                }
            }
        }

        // 2 rotation around UN edge
        void rotateUN( Perm rot[NUM_ROTATE_UN] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        rot[0][idx] = getIndex(  x, y, z );
                        rot[1][idx] = getIndex( -x, z, y );
                    }
                }
            }
        }

        // 3 rotation around UNW corner (x + y + z = const)
        void rotateUNW( Perm rot[NUM_ROTATE_UNW] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        rot[0][idx] = getIndex( x, y, z );
                        rot[1][idx] = getIndex( y, z, x );
                        rot[2][idx] = getIndex( z, x, y );
                    }
                }
            }
        }

        // 6 directions of faces
        void directU( Perm dir[NUM_DIRECT_U] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        dir[0][idx] = getIndex(  x,  y,  z );   // U( 0, 0,-1)
                        dir[1][idx] = getIndex(  x, -y, -z );   // D( 0, 0, 1)
                        dir[2][idx] = getIndex(  x,  z, -y );   // N( 0,-1, 0)
                        dir[3][idx] = getIndex(  x, -z,  y );   // S( 0, 1, 0)
                        dir[4][idx] = getIndex(  z,  y, -x );   // W(-1, 0, 0)
                        dir[5][idx] = getIndex( -z,  y,  x );   // E( 1, 0, 0)
                    }
                }
            }
        }

        // 12 directions of edges
        void directUN( Perm dir[NUM_DIRECT_UN] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        dir[ 0][idx] = getIndex(  x,  y,  z );   // UN( 0,-1,-1)
                        dir[ 1][idx] = getIndex(  x, -y, -z );   // DS( 0, 1, 1)
                        dir[ 2][idx] = getIndex( -y,  x,  z );   // UE( 1, 0,-1) = Rz(270)
                        dir[ 3][idx] = getIndex(  y,  x, -z );   // DW(-1, 0, 1) + flip 'y' & 'z'
                        dir[ 4][idx] = getIndex( -x, -y,  z );   // US( 0, 1,-1) = Rz(180)
                        dir[ 5][idx] = getIndex( -x,  y, -z );   // DN( 0,-1, 1)
                        dir[ 6][idx] = getIndex(  y, -x,  z );   // UW(-1, 0,-1) = Rz(90)
                        dir[ 7][idx] = getIndex( -y, -x, -z );   // DE( 1, 0, 1)
                        dir[ 8][idx] = getIndex(  z,  y, -x );   // NW(-1,-1, 0) = Ry(270)
                        dir[ 9][idx] = getIndex( -z, -y, -x );   // SE( 1, 1, 0)
                        dir[10][idx] = getIndex( -z,  y,  x );   // NE( 1,-1, 0) = Ry(90)
                        dir[11][idx] = getIndex(  z, -y,  x );   // SW(-1, 1, 0)
                    }
                }
            }
        }

        // 8 directions of corners
        void directUNW( Perm dir[NUM_DIRECT_UNW] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        dir[0][idx] = getIndex(  x,  y,  z );   // UNW(-1,-1,-1)
                        dir[1][idx] = getIndex( -x, -z, -y );   // DSE( 1, 1, 1) = Rx(90) * Rz(180)
                        dir[2][idx] = getIndex( -y,  x,  z );   // UNE( 1,-1,-1) = Rz(90)
                        dir[3][idx] = getIndex(  y, -z, -x );   // DSW(-1, 1, 1) = Rx(90) * Rz(270)
                        dir[4][idx] = getIndex( -x, -y,  z );   // USE( 1, 1,-1) = Rz(180)
                        dir[5][idx] = getIndex(  x,  z, -y );   // DNW(-1,-1, 1) = Rx(270)
                        dir[6][idx] = getIndex(  y, -x,  z );   // USW(-1, 1,-1) = Rz(270)
                        dir[7][idx] = getIndex( -y,  z, -x );   // DNE( 1,-1, 1) = Rx(270) * Rz(90)
                    }
                }
            }
        }

        // init perm tables
        void InitTables() {
            for (int i = 0; i < 27; ++i) {
                PermIdent[0][i] = i;
            }
            // basic symmetries
            flipX( PermFlipX );
            flipXY( PermFlipXY );
            flipXYZ( PermFlipXYZ );
            rotateU( PermRotateU );
            rotateUN( PermRotateUN );
            rotateUNW( PermRotateUNW );
            directU( PermDirectU );
            directUN( PermDirectUN );
            directUNW( PermDirectUNW );
            // PermSymU
            for (size_t j = 0; j < NUM_ROTATE_U; ++j) {
                for (size_t i = 0; i < NUM_FLIP; ++i) {
                    Perm& perm = PermSymU[j*NUM_FLIP+i];
                    for (size_t n = 0; n < 27; ++n) {
                        perm[n] = PermRotateU[j][PermFlipX[i][n]];
                    }
                }
            }
            // PermSymUN
            for (size_t j = 0; j < NUM_ROTATE_UN; ++j) {
                for (size_t i = 0; i < NUM_FLIP; ++i) {
                    Perm& perm = PermSymUN[j*NUM_FLIP+i];
                    for (size_t n = 0; n < 27; ++n) {
                        perm[n] = PermRotateUN[j][PermFlipX[i][n]];
                    }
                }
            }
            // PermSymUNW
            for (size_t j = 0; j < NUM_ROTATE_UNW; ++j) {
                for (size_t i = 0; i < NUM_FLIP; ++i) {
                    Perm& perm = PermSymUNW[j*NUM_FLIP+i];
                    for (size_t n = 0; n < 27; ++n) {
                        perm[n] = PermRotateUNW[j][PermFlipXY[i][n]];
                    }
                }
            }
            // PermSymAll (SymAll)
            for (size_t k = 0; k < NUM_DIRECT_U; ++k) {
                for (size_t j = 0; j < NUM_ROTATE_U; ++j) {
                    for (size_t i = 0; i < NUM_FLIP; ++i) {
                        Perm& perm = PermSymAll[(k*NUM_ROTATE_U+j)*NUM_FLIP+i];
                        for (size_t n = 0; n < 27; ++n) {
                            perm[n] = PermDirectU[k][PermRotateU[j][PermFlipX[i][n]]];
                        }
                    }
                }
            }
        }
    }

    void Initialize() {
        NPerm::InitTables();
        if (false) {
            const int bits = BV( BP(1,0,1), BP(6,0,0), BP(0,0,0) );
            int rot_un[NUM_ROTATE_UN];
            int rot_unw[NUM_ROTATE_UNW];
            int sym_un[NUM_SYM_UN];
            int sym_unw[NUM_SYM_UNW];
            RotateBitsUN( bits, rot_un );
            RotateBitsUNW( bits, rot_unw );
            TransformBitsUN( bits, sym_un );
            TransformBitsUNW( bits, sym_unw );
            for each (int sym in sym_unw) {
                DumpBits( sym );
            }
            system( "pause" );
        }
    }

    int TransformBits( int bits, const Perm& perm ) {
        int ret = 0;
        for (int i = 0; i < 27; ++i) {
            if (bits & (1 << i)) {
                ret |= 1 << perm[i];
            }
        }
        return ret;
    }

    Mask TransformMask( const Mask& mask, const Perm& perm ) {
        return Mask(
            TransformBits( mask.mask,   perm ),
            TransformBits( mask.value,  perm ),
            TransformBits( mask.nzero1, perm ),
            TransformBits( mask.nzero2, perm )
        );
    }
}

namespace NSymmetry {
    void CreatePrimaryTable( vector<uint8_t>& table ) {
        table.clear();
        table.resize( 1 << 27, EP_ALL | EP_FACE_U );
        int syms_all[NUM_SYM_ALL];
        int syms_u[NUM_SYM_U];
        for (int bits = 0; bits < (1 << 27); ++bits) {
            int val = table[bits];
            if (val & EP_ALL) {
                TransformBitsAll( bits, syms_all );
                for each (int sym in syms_all) {
                    if (sym > bits) {
                        table[sym] &= ~EP_ALL;
                    }
                }
            }
            if (val & EP_FACE_U) {
                TransformBitsU( bits, syms_u );
                for each (int sym in syms_u) {
                    if (sym > bits) {
                        table[sym] &= ~EP_FACE_U;
                    }
                }
            }
            table[bits] = val;
        }
    }
}
