#include "stdafx.h"
#include "Neighbors.h"
#include "SimpleDefs.h"

using namespace std;
using namespace std::placeholders;

void LoadOrCreateTable( vector<uint8_t>& table, const string& name, function<void( vector<uint8_t>& )> creator ) {
    printf( "[%s]\n", name.c_str() );
    string path = name;
    path.append( ".dat" );
    if (NByteTable::LoadTable( table, path.c_str() ) == false) {
        creator( table );
        NByteTable::SaveTable( table, path.c_str() );
    }
}

template<typename TMask>
void GenerateDirHeader( const TMask* masks, int num_masks, const char* filename ) {
    vector<vector<TMask>> table;
    NMask6::InstantiateUDirMasks( masks, num_masks, table );
    NMask6::GenerateDirHeader( table, filename );
}

template<typename TMask>
void GenerateOmniDirHeader( const TMask* masks, int num_masks, const char* filename ) {
    vector<vector<TMask>> table;
    NMask6::InstantiateOmniDirMasks( masks, num_masks, table );
    NMask6::GenerateOmniDirHeader( table, filename );
}

int _tmain( int argc, _TCHAR* argv[] )
{
    {// initialization (should be in this order)
        NSymmetry6::Initialize();
        NNeighbors::Initialize();
    }
    vector<uint8_t> compcount;
    {// component count table
        LoadOrCreateTable( compcount, "Component", bind( &NComponent::CreateCountTable, _1 ) );
        NComponent::ShowCountStat( compcount );
    }
    vector<uint8_t> primary;
    {// primary bit patterns mask
        LoadOrCreateTable( primary, "Primary", bind( &NSymmetry6::CreatePrimaryTable, _1 ) );
        NByteTable::ShowBitStat( primary );
    }
    vector<uint8_t> palagyi;
    if (true) {// Palagyi
        LoadOrCreateTable( palagyi, "Palagyi", bind( &NSymmetry6::CreatePredicateTable, _1, &NPalagyi::IsDeletable ) );
        NByteTable::ShowBitStat( palagyi );
        GenerateDirHeader( NRaynal::Templates, _countof( NRaynal::Templates ), "Palagyi.h" );
    }
    vector<uint8_t> raynal;
    if (true) {// Raynal
        LoadOrCreateTable( raynal, "Raynal", bind( &NSymmetry6::CreatePredicateTable, _1, &NRaynal::IsDeletable ) );
        NByteTable::ShowBitStat( raynal );
        GenerateDirHeader( NRaynal::Templates, _countof( NRaynal::Templates ), "Raynal.h" );
    }
    if (true) {// Show Raynal's U-masks that are not contained in Palagyi's
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
    if (true) {// Isthmus (also by Raynal)
        vector<uint8_t> isthmus;
        NComponent::FilterByCount( compcount, isthmus, 2, INT_MAX, 0, INT_MAX );
        NIsthmus::Veryfy_IsIsthmus( isthmus );
        GenerateOmniDirHeader( NIsthmus::Templates, _countof( NIsthmus::Templates ), "Isthmus.h" );
    }
    vector<uint8_t> pxcond, pxsimple;
    if (true) {// Lohou, P2x simple
        LoadOrCreateTable( pxcond, "PxCondition", bind( &NPxSimple::CheckConditionsUDir, _1 ) );
        NByteTable::ShowBitStat( pxcond );
        NPxSimple::Verify_IsP2xSimpleU( pxcond );
        LoadOrCreateTable( pxsimple, "PxSimple", bind( &NSymmetry6::CreatePredicateTable, _1, &NPxSimple::IsP2xSimple ) ); 
        NByteTable::ShowBitStat( pxsimple );
        GenerateDirHeader( NPxSimple::TemplatesT26Rx, _countof( NPxSimple::TemplatesT26Rx ), "PxSimple.h" );
        //NPxSimple::Verify_CalcP2x();
        //NPxSimple::Verify_IsP2xSimple();
    }
    if (true) {// Show Raynal's U-masks that are not contained in Lohou's
        printf( "({Raynal} - {Lohou}) & U-direction\n" );
        if (!raynal.empty() && !pxsimple.empty()) {
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
    system( "pause" );
    return 0;
}
