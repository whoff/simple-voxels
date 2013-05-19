#include "stdafx.h"
#include "Symmetry.h"
#include "Util.h"

#include "Mask.h"

using namespace std;

void DumpMask( const Mask& mask )     { DumpMask( mask.mask, mask.value, mask.nzero ); }
void DumpMask( const MaskZero& mask ) { DumpMask( mask.mask, 0, mask.nzero1, mask.nzero2 );}

namespace NMask6 {

    void makeSymMasks( const Mask& tmpl, vector<Mask>& syms, function<void ( int, int* )> make_syms, size_t num_syms ) {
        syms.clear();
        vector<int> masks( num_syms );
        vector<int> values( num_syms );
        vector<int> nzeros( num_syms );
        make_syms( tmpl.mask, &masks[0] );
        make_syms( tmpl.value, &values[0] );
        make_syms( tmpl.nzero, &nzeros[0] );
        for (size_t n = 0; n < num_syms; ++n) {
            syms.push_back( Mask( masks[n], values[n], nzeros[n] ) );
        }
    }

    void makeSymMasks( const MaskZero& tmpl, vector<MaskZero>& syms, function<void ( int, int* )> make_syms, size_t num_syms ) {
        syms.clear();
        vector<int> masks( num_syms );
        vector<int> nzeros1( num_syms );
        vector<int> nzeros2( num_syms );
        make_syms( tmpl.mask, &masks[0] );
        make_syms( tmpl.nzero1, &nzeros1[0] );
        make_syms( tmpl.nzero2, &nzeros2[0] );
        for (size_t n = 0; n < num_syms; ++n) {
            syms.push_back( MaskZero( masks[n], nzeros1[n], nzeros2[n] ) );
        }
    }

    Mask makeSymDirectZ( const Mask& tmpl, EDir6 dir ) {
        return Mask(
            NSymmetry6::MakeSymDirectZ( tmpl.mask, dir ),
            NSymmetry6::MakeSymDirectZ( tmpl.value, dir ),
            NSymmetry6::MakeSymDirectZ( tmpl.nzero, dir )
            );
    }

    MaskZero makeSymDirectZ( const MaskZero& tmpl, EDir6 dir ) {
        return MaskZero(
            NSymmetry6::MakeSymDirectZ( tmpl.mask, dir ),
            NSymmetry6::MakeSymDirectZ( tmpl.nzero1, dir ),
            NSymmetry6::MakeSymDirectZ( tmpl.nzero2, dir )
            );
    }

    // collect symmetries
    template<typename TMask>
    void instantiateMasks( const TMask* templates, size_t num_templates, vector<vector<TMask>>& table, function<void ( int, int* )> make_syms, size_t num_syms ) {
        table.clear();
        for (size_t n = 0; n < num_templates; ++n) {
            vector<TMask> masks;
            vector<TMask> syms;
            makeSymMasks( templates[n], syms, make_syms, num_syms );
            for each (const TMask& sym in syms) {
                if (find( masks.begin(), masks.end(), sym ) == masks.end()) {
                    masks.push_back( sym );
                }
            }
            table.push_back( masks );
        }
    }

    void InstantiateUDirMasks( const Mask* templates, size_t num_templates, vector<vector<Mask>>& table ) {
        instantiateMasks( templates, num_templates, table, NSymmetry6::MakeSymsXY, NSymmetry6::NUM_SYM_XY );
    }

    void InstantiateUDirMasks( const MaskZero* templates, size_t num_templates, vector<vector<MaskZero>>& table ) {
        instantiateMasks( templates, num_templates, table, NSymmetry6::MakeSymsXY, NSymmetry6::NUM_SYM_XY );
    }

    void InstantiateOmniDirMasks( const Mask* templates, size_t num_templates, vector<vector<Mask>>& table ) {
        instantiateMasks( templates, num_templates, table, NSymmetry6::MakeSymsXYZ, NSymmetry6::NUM_SYM_XYZ );
    }

    void InstantiateOmniDirMasks( const MaskZero* templates, size_t num_templates, vector<vector<MaskZero>>& table ) {
        instantiateMasks( templates, num_templates, table, NSymmetry6::MakeSymsXYZ, NSymmetry6::NUM_SYM_XYZ );
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
        fprintf( fp, "const Mask mask_tables[6][%d] = {\n", cnt );
        for (int i = 0; i < ED_NUM; ++i) {
            EDir6 dir = (EDir6) i;
            fprintf( fp, "    {// dir = %d\n", i );
            size_t idx = 0;
            for (size_t m = 0; m < table.size(); ++m) {
                fprintf( fp, "        // M%d\n", m+1 );
                for each (const Mask& sym in table[m]) {
                    const Mask mask = makeSymDirectZ( sym, dir );
                    fprintf( fp, "        " );
                    fprintf( fp, "Mask( 0x%07x, 0x%07x", mask.mask, mask.value );
                    if (mask.nzero) {
                        fprintf( fp, ", 0x%07x", mask.nzero );
                    }
                    fprintf( fp, " ),\n" );
                }
            }
            fprintf( fp, "    },\n" );
        }
        fprintf( fp, "};\n" );
        fclose( fp );
    }

    void GenerateDirHeader( const vector<vector<MaskZero>>& table, const string& path ) {
        FILE* fp = fopen( path.c_str(), "w" );
        if (fp == NULL) return;
        size_t cnt = 0;
        for each (const vector<MaskZero>& syms in table) {
            cnt += syms.size();
        }
        fprintf( fp, "#include \"stdafx.h\"\n\n" );
        fprintf( fp, "#include \"Mask.h\"\n\n" );
        fprintf( fp, "const MaskZero mask_tables[6][%d] = {\n", cnt );
        for (int i = 0; i < ED_NUM; ++i) {
            EDir6 dir = (EDir6) i;
            fprintf( fp, "    {// dir = %d\n", i );
            size_t idx = 0;
            for (size_t m = 0; m < table.size(); ++m) {
                fprintf( fp, "        // M%d\n", m+1 );
                for each (const MaskZero& sym in table[m]) {
                    const MaskZero mask = makeSymDirectZ( sym, dir );
                    fprintf( fp, "        " );
                    fprintf( fp, "MaskZero( 0x%07x, 0x%07x, 0x%07x ),\n", mask.mask, mask.nzero1, mask.nzero2 );
                }
            }
            fprintf( fp, "    },\n" );
        }
        fprintf( fp, "};\n" );
        fclose( fp );
    }

    void GenerateOmniDirHeader( const vector<vector<MaskZero>>& table, const string& path ) {
        FILE* fp = fopen( path.c_str(), "w" );
        if (fp == NULL) return;
        size_t cnt = 0;
        for each (const vector<MaskZero>& syms in table) {
            cnt += syms.size();
        }
        fprintf( fp, "#include \"stdafx.h\"\n\n" );
        fprintf( fp, "#include \"Mask.h\"\n\n" );
        fprintf( fp, "const MaskZero mask_tables[%d] = {\n", cnt );
        for (size_t m = 0; m < table.size(); ++m) {
            fprintf( fp, "    // M%d\n", m+1 );
            for each (const MaskZero& sym in table[m]) {
                fprintf( fp, "    MaskZero( 0x%07x, 0x%07x, 0x%07x ),\n", sym.mask, sym.nzero1, sym.nzero2 );
            }
        }
        fprintf( fp, "};\n" );
        fclose( fp );
    }
}
