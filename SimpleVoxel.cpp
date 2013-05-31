#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"
#include "Util.h"

using namespace std;
using namespace std::placeholders;

namespace {
    template<typename T, size_t N>
    vector<function<bool( int )>> CreatePredicates( bool (*pred)( int, T ) ) {
        vector<function<bool( int )>> ret;
        for (size_t n = 0; n < N; ++n) {
            ret.push_back( bind( pred, _1, (T) n ) );
        }
        return ret;
    }

    void CreatePredicateTable( vector<uint8_t>& table, const vector<function<bool( int )>>& preds ) {
        table.clear();
        table.resize( 1 << 27, 0 );
        const size_t num_preds = std::min<size_t>( preds.size(), 8 );
        for (int bits = 0; bits < (1 << 27); ++bits) {
            if ((bits & N0_bits) == 0) continue;
            uint8_t val = 0;
            for (size_t n = 0; n < num_preds; ++n) {
                if (preds[n]( bits )) {
                    val |= 1 << n;
                }
            }
            table[bits] = val;
        }
    }

    void LoadOrCreateTable( vector<uint8_t>& table, const string& name, function<void( vector<uint8_t>& )> creator ) {
        printf( "[%s]\n", name.c_str() );
        string path = name;
        path.append( ".dat" );
        if (NByteTable::LoadTable( table, path.c_str() ) == false) {
            creator( table );
            //NByteTable::SaveTable( table, path.c_str() );
        }
    }
}

int _tmain( int argc, _TCHAR* argv[] )
{
    {// initialization (should be in this order)
        NSymmetry::Initialize();
        NNeighbors::Initialize();
    }
    vector<uint8_t> compcount;
    {// component count table
        LoadOrCreateTable( compcount, "Component", bind( &NComponent::CreateCountTable, _1 ) );
        NComponent::ShowCountStat( compcount );
    }
    vector<uint8_t> primary;
    {// primary bit patterns mask
        LoadOrCreateTable( primary, "Primary", bind( &NSymmetry::CreatePrimaryTable, _1 ) );
        NByteTable::ShowBitStat( primary );
    }
    vector<uint8_t> palagyi;
    if (false) {// Palagyi
        LoadOrCreateTable( palagyi, "Palagyi", bind( &CreatePredicateTable, _1, CreatePredicates<EFaceDir, ED_NUM_FACE_DIR>( &NPalagyi::IsDeletable ) ) );
        NByteTable::ShowBitStat( palagyi );
        NPalagyi::GenerateHeader( "Palagyi.h" );
    }
    vector<uint8_t> raynal;
    if (false) {// Raynal
        LoadOrCreateTable( raynal, "Raynal", bind( &CreatePredicateTable, _1, CreatePredicates<EFaceDir, ED_NUM_FACE_DIR>( &NRaynal::IsDeletable ) ) );
        NByteTable::ShowBitStat( raynal );
        NRaynal::GenerateHeader( "Raynal.h" );
    }
    if (false) {// Show Raynal's U-masks that are not contained in Palagyi's
        printf( "({Raynal} - {Palagyi}) & U-direction\n" );
        if (!palagyi.empty() && !raynal.empty()) {
            const int U_bit = (1 << ED_U);
            int cnt = 0;
            for (int bits = 0; bits < (1 << 27); ++bits) {
                if ((palagyi[bits] & U_bit) == 0 && (raynal[bits] & U_bit) == U_bit) {
                    DumpBits( bits );
                    cnt++;
                }
            }
            printf( "count = %d\n", cnt );
        }
    }
    if (false) {// Isthmus (also by Raynal)
        vector<uint8_t> isthmus;
        NComponent::FilterByCount( compcount, isthmus, 2, INT_MAX, 0, INT_MAX );
        printf( "[Isthmus]\n" );
        NByteTable::ShowBitStat( isthmus );
        NIsthmus::GenerateHeader( "Isthmus.h" );
        if (true) {
            NIsthmus::NDevelop::Veryfy_IsIsthmus( isthmus );
            NIsthmus::NDevelop::Verify_Templates( primary, isthmus );
        }
    }
    vector<uint8_t> pxcond, pxsimple;
    if (false) {// Lohou, P2x simple
        LoadOrCreateTable( pxcond, "PxCondition", bind( &NPxSimple::CheckConditionsU, _1 ) );
        NByteTable::ShowBitStat( pxcond );
        LoadOrCreateTable( pxsimple, "PxSimple", bind( &CreatePredicateTable, _1, CreatePredicates<EFaceDir, ED_NUM_FACE_DIR>( &NPxSimple::IsDeletable ) ) );
        NByteTable::ShowBitStat( pxsimple );
        NPxSimple::GenerateHeader( "PxSimple.h" );
        if (true) {
            NPxSimple::NDevelop::Verify_CalcP2xU();
            NPxSimple::NDevelop::Verify_IsP2xSimpleU( pxcond );
        }
    }
    if (false) {// Show Raynal's U-masks that are not contained in Lohou's
        if (!raynal.empty() && !pxsimple.empty()) {
            printf( "({Raynal} - {P2xSimple}) & U-direction\n" );
            const int U_bit = (1 << ED_U);
            int cnt = 0;
            for (int bits = 0; bits < (1 << 27); ++bits) {
                if ((raynal[bits] & U_bit) == U_bit && (pxsimple[bits] & U_bit) == 0) {
                    DumpBits( bits );
                    cnt++;
                }
            }
            printf( "count = %d\n", cnt );
        }
    }
    if (false) {// Show PxSimple U-masks may remove upper diagonal configurations
        if (!pxsimple.empty()) {
            printf( "{P2xSimple} & U-direction & Upper Diag\n" );
            const Mask diag( BV( BP(3,3,0), BP(3,3,0), BP(0,0,0) ), BV( BP(1,0,0), BP(0,2,0), BP(0,0,0) ) );
            DumpMask( diag );
            const int U_bit = (1 << ED_U);
            int cnt = 0;
            for (int bits = 0; bits < (1 << 27); ++bits) {
                if ((pxsimple[bits] & U_bit) == U_bit && diag.Match( bits )) {
                    DumpBits( bits );
                    cnt++;
                }
            }
            printf( "count = %d\n", cnt );
        }
    }
    vector<uint8_t> pxsurf;
    if (true) {// Lohou, 12-subiteration surf
        for each (const Mask& mask in NPxSimpleSurface::TemplatesUN) {
            DumpMask( mask );
        }
        LoadOrCreateTable( pxsurf, "PxSimpleSurf", bind( &CreatePredicateTable, _1, CreatePredicates<EEdgeDir, ED_NUM_EDGE_DIR>( &NPxSimpleSurface::IsDeletable ) ) );
        NByteTable::ShowBitStat( pxsurf );
        NPxSimpleSurface::GenerateHeader( "PxSimpleSurf.h" );
    }
    vector<uint8_t> nemeth;
    if (true) {// Nemeth, smoothing
        LoadOrCreateTable( nemeth, "Nemeth", bind( &CreatePredicateTable, _1, CreatePredicates<int, 2>( &NNemeth::IsDeletable ) ) );
        NByteTable::ShowBitStat( nemeth );
        NNemeth::GenerateHeader( "Nemeth.h" );
    }
    printf( "\n" );
    system( "pause" );
    return 0;
}
