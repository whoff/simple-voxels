#pragma once

#include "Mask.h"

// 6-subiteration directions
enum EDir6 {
    ED_U,   // Up
    ED_D,   // Down
    ED_N,   // North
    ED_S,   // South
    ED_E,   // East
    ED_W,   // West
    ED_NUM,
};

enum EPrimary6 {
    EP_XYZ = 0x01,
    EP_XY  = 0x02,
};

// symmetries for U-direction of 6-subiteration
namespace NSymmetry6 {
    // init internal tables
    void Initialize();

    enum {
        NUM_FLIP_X    = 2,
        NUM_ROTATE_XY = 4,
        NUM_DIRECT_Z  = 6,
        NUM_SYM_XY    = NUM_FLIP_X * NUM_ROTATE_XY,
        NUM_SYM_XYZ   = NUM_FLIP_X * NUM_ROTATE_XY * NUM_DIRECT_Z, 
    };

    // basic transformations
    void MakeSymsFlipX   ( int bits, int syms[NUM_FLIP_X] );
    void MakeSymsRotateXY( int bits, int syms[NUM_ROTATE_XY] );
    void MakeSymsDirectZ ( int bits, int syms[NUM_DIRECT_Z] );
    // combinations
    void MakeSymsXY      ( int bits, int syms[NUM_SYM_XY] );
    void MakeSymsXYZ     ( int bits, int syms[NUM_SYM_XYZ] );

    // direct z-axis --> +z (U), -z (D), +y (N), -y (S), +x (E), -x (W)
    int MakeSymDirectZ( int bits, EDir6 dir );

    // find the minimum (as a value) symmetry in SymsXYZ
    int FindSymInXYZ( int bits );

    void CreatePrimaryTable( std::vector<std::uint8_t>& table );
    void CreatePredicateTable( std::vector<std::uint8_t>& table, bool (*predicate)( int, EDir6 ) );

    // mask creation
    void GenerateDirHeader( const std::vector<std::vector<Mask>>& table, const std::string& path );
    void GenerateOmniDirHeader( const std::vector<std::vector<Mask>>& table, const std::string& path );

    void InstantiateUDirMasks( const Mask* templates, size_t num_templates, std::vector<std::vector<Mask>>& table );
    void InstantiateOmniDirMasks( const Mask* templates, size_t num_templates, std::vector<std::vector<Mask>>& table );
}
