#include "stdafx.h"
#include "Neighbors.h"

#include "Symmetry.h"

using namespace std;

typedef int Perm[27];

namespace NSymmetry6 {
    namespace NPerm {
        Perm perm_flp[NUM_FLIP_X];      // flipX
        Perm perm_rot[NUM_ROTATE_XY];   // rotateXY
        Perm perm_dir[NUM_DIRECT_Z];    // directZ
        Perm perm_frt[NUM_SYM_XY];      // flipX + rotateXY
        Perm perm_all[NUM_SYM_XYZ];     // flipX + rotateXY + directZ

        // 2 flip along x-axis
        void flipX( Perm flp[NUM_FLIP_X] ) {
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

        // 4 rotation around z-axis
        void rotateXY( Perm rot[NUM_ROTATE_XY] ) {
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

        // 6 directions of cube faces
        void directZ( Perm dir[NUM_DIRECT_Z] ) {
            for (int z = -1; z <= 1; ++z) {
                for (int y = -1; y <= 1; ++y) {
                    for (int x = -1; x <= 1; ++x) {
                        const int idx = getIndex( x, y, z );
                        dir[0][idx] = getIndex(  x,  y,  z );   // U
                        dir[1][idx] = getIndex(  x, -y, -z );   // D
                        dir[2][idx] = getIndex(  x,  z, -y );   // N
                        dir[3][idx] = getIndex(  x, -z,  y );   // S
                        dir[4][idx] = getIndex(  z,  y, -x );   // E
                        dir[5][idx] = getIndex( -z,  y,  x );   // W
                    }
                }
            }
        }

        // init perm tables
        void InitTables() {
            // basic symmetries
            flipX( perm_flp );
            rotateXY( perm_rot );
            directZ( perm_dir );
            // perm_frt (SymXY)
            for (size_t j = 0; j < NUM_ROTATE_XY; ++j) {
                for (size_t i = 0; i < NUM_FLIP_X; ++i) {
                    int* p = perm_frt[j*NUM_FLIP_X+i];
                    for (size_t n = 0; n < 27; ++n) {
                        p[n] = perm_rot[j][perm_flp[i][n]];
                    }
                }
            }
            // perm_all (SymXYZ)
            for (size_t k = 0; k < NUM_DIRECT_Z; ++k) {
                for (size_t j = 0; j < NUM_ROTATE_XY; ++j) {
                    for (size_t i = 0; i < NUM_FLIP_X; ++i) {
                        int* p = perm_all[(k*NUM_ROTATE_XY+j)*NUM_FLIP_X+i];
                        for (size_t n = 0; n < 27; ++n) {
                            p[n] = perm_dir[k][perm_rot[j][perm_flp[i][n]]];
                        }
                    }
                }
            }
        }
    }

    namespace NImpl {
        template<int M>
        void Transform( int bits, int* syms, Perm (&perms)[M] ) {
            {// init
                syms[0] = bits;
                for (int j = 1; j < M; ++j) {
                    syms[j] = 0;
                }
            }
            for (int i = 0; i < 27; ++i) {
                if ((bits & (1 << i)) == 0) continue;
                for (int j = 1; j < M; ++j) {
                    syms[j] |= 1 << perms[j][i];
                }
            }
        }

        int Transform( int bits, const Perm& perm ) {
            int sym = 0;
            for (int i = 0; i < 27; ++i) {
                if (bits & (1 << i)) {
                    sym |= 1 << perm[i];
                }
            }
            return sym;
        }

        typedef void (*FPermutater)( int bits, int* sym );

        template<int M>
        int FindPerm( int bits, FPermutater func ) {
            int syms[M];
            (*func)( bits, syms );
            for (int j = 1; j < M; ++j) {
                if (bits > syms[j]) {
                    return syms[j];
                }
            }
            return -1;
        }
    }

    void MakeSymsFlipX   ( int bits, int syms[NUM_FLIP_X]    ) { NImpl::Transform( bits, syms, NPerm::perm_flp ); }
    void MakeSymsRotateXY( int bits, int syms[NUM_ROTATE_XY] ) { NImpl::Transform( bits, syms, NPerm::perm_rot ); }
    void MakeSymsDirectZ ( int bits, int syms[NUM_DIRECT_Z]  ) { NImpl::Transform( bits, syms, NPerm::perm_dir ); }
    void MakeSymsXY      ( int bits, int syms[NUM_SYM_XY]    ) { NImpl::Transform( bits, syms, NPerm::perm_frt ); }
    void MakeSymsXYZ     ( int bits, int syms[NUM_SYM_XYZ]   ) { NImpl::Transform( bits, syms, NPerm::perm_all ); }

    int MakeSymDirectZ( int bits, EDir6 dir ) {
        return NImpl::Transform( bits, NPerm::perm_dir[(int)dir] );
    }

    int FindSymInXYZ( int bits ) { return NImpl::FindPerm<48>( bits, MakeSymsXYZ ); }

    void CreatePrimaryTable( vector<uint8_t>& table ) {
        table.clear();
        table.resize( 1 << 27, EP_XYZ | EP_XY );
        int syms_xyz[NUM_SYM_XYZ];
        int syms_xy[NUM_SYM_XY];
        for (int bits = 0; bits < (1 << 27); ++bits) {
            int val = table[bits];
            if (val & EP_XYZ) {
                MakeSymsXYZ( bits, syms_xyz );
                for each (int sym in syms_xyz) {
                    if (sym > bits) {
                        table[sym] &= ~EP_XYZ;
                    }
                }
            }
            if (val & EP_XY) {
                MakeSymsXY( bits, syms_xy );
                for each (int sym in syms_xy) {
                    if (sym > bits) {
                        table[sym] &= ~EP_XY;
                    }
                }
            }
            table[bits] = val;
        }
    }

    void CreatePredicateTable( vector<uint8_t>& table, bool (*predicate)( int, EDir6 ) ) {
        table.clear();
        table.resize( 1 << 27, 0 );
        for (int bits = 0; bits < (1 << 27); ++bits) {
            if ((bits & N0_bits) == 0) continue;
            uint8_t val = 0;
            for (int dir = 0; dir < ED_NUM; ++dir) {
                if (predicate( bits, (EDir6) dir )) {
                    val |= 1 << dir;
                }
            }
            table[bits] = val;
        }
    }

    void Initialize() {
        NPerm::InitTables();
    }
}

namespace NSymmetry6 {

    void makeSymMasks( const Mask& tmpl, vector<Mask>& syms, function<void ( int, int* )> make_syms, size_t num_syms ) {
        syms.clear();
        vector<int> masks( num_syms );
        vector<int> values( num_syms );
        vector<int> nzeros1( num_syms );
        vector<int> nzeros2( num_syms );
        make_syms( tmpl.mask, &masks[0] );
        make_syms( tmpl.value, &values[0] );
        make_syms( tmpl.nzero1, &nzeros1[0] );
        make_syms( tmpl.nzero2, &nzeros2[0] );
        for (size_t n = 0; n < num_syms; ++n) {
            syms.push_back( Mask( masks[n], values[n], nzeros1[n], nzeros2[n] ) );
        }
    }

    Mask makeSymDirectZ( const Mask& tmpl, EDir6 dir ) {
        return Mask(
            MakeSymDirectZ( tmpl.mask, dir ),
            MakeSymDirectZ( tmpl.value, dir ),
            MakeSymDirectZ( tmpl.nzero1, dir ),
            MakeSymDirectZ( tmpl.nzero2, dir )
            );
    }

    // collect symmetries
    void instantiateMasks( const Mask* templates, size_t num_templates, vector<vector<Mask>>& table, function<void ( int, int* )> make_syms, size_t num_syms ) {
        table.clear();
        for (size_t n = 0; n < num_templates; ++n) {
            vector<Mask> masks;
            vector<Mask> syms;
            makeSymMasks( templates[n], syms, make_syms, num_syms );
            for each (const Mask& sym in syms) {
                if (find( masks.begin(), masks.end(), sym ) == masks.end()) {
                    masks.push_back( sym );
                }
            }
            table.push_back( masks );
        }
    }

    void InstantiateUDirMasks( const Mask* templates, size_t num_templates, vector<vector<Mask>>& table ) {
        instantiateMasks( templates, num_templates, table, MakeSymsXY, NUM_SYM_XY );
    }

    void InstantiateOmniDirMasks( const Mask* templates, size_t num_templates, vector<vector<Mask>>& table ) {
        instantiateMasks( templates, num_templates, table, MakeSymsXYZ, NUM_SYM_XYZ );
    }

    void fprint_mask( FILE* fp, const Mask& mask ) {
        fprintf( fp, "Mask( 0x%07x", mask.mask );
        fprintf( fp, (mask.value) ? ", 0x%07x" : ", %d", mask.value );
        if (mask.nzero1) fprintf( fp, ", 0x%07x", mask.nzero1 );
        if (mask.nzero2) fprintf( fp, ", 0x%07x", mask.nzero2 );
        fprintf( fp, " )" );
    }

    void GenerateDirHeader( const vector<vector<Mask>>& table, const string& path ) {
        FILE* fp = fopen( path.c_str(), "w" );
        if (fp == NULL) return;
        size_t cnt = 0;
        for each (const vector<Mask>& syms in table) {
            cnt += syms.size();
        }
        fprintf( fp, "#include \"stdafx.h\"\n\n" );
        fprintf( fp, "#include \"Mask.h\"\n\n" );
        fprintf( fp, "const Mask mask_tables[][%d] = {\n", cnt );
        for (int i = 0; i < ED_NUM; ++i) {
            EDir6 dir = (EDir6) i;
            fprintf( fp, "    {// dir = %d\n", i );
            for (size_t m = 0; m < table.size(); ++m) {
                fprintf( fp, "        // M%d\n", m+1 );
                for each (const Mask& sym in table[m]) {
                    fprintf( fp, "        " );
                    fprint_mask( fp, makeSymDirectZ( sym, dir ) );
                    fprintf( fp, ",\n" );
                }
            }
            fprintf( fp, "    },\n" );
        }
        fprintf( fp, "};\n" );
        fclose( fp );
    }

    void GenerateOmniDirHeader( const vector<vector<Mask>>& table, const string& path ) {
        FILE* fp = fopen( path.c_str(), "w" );
        if (fp == NULL) return;
        size_t cnt = 0;
        for each (const vector<Mask>& syms in table) {
            cnt += syms.size();
        }
        fprintf( fp, "#include \"stdafx.h\"\n\n" );
        fprintf( fp, "#include \"Mask.h\"\n\n" );
        fprintf( fp, "const Mask mask_table[%d] = {\n", cnt );
        for (size_t m = 0; m < table.size(); ++m) {
            fprintf( fp, "    // M%d\n", m+1 );
            for each (const Mask& sym in table[m]) {
                fprintf( fp, "    " );
                fprint_mask( fp, sym );
                fprintf( fp, ",\n" );
            }
        }
        fprintf( fp, "};\n" );
        fclose( fp );
    }
}
