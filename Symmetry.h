#pragma once

#include "Mask.h"

// 6 face directions
enum EFaceDir {
    ED_U, ED_D, // Up, Down     (-z, +z)
    ED_N, ED_S, // North, South (-y, +y)
    ED_W, ED_E, // West, East   (-x, +x)
    ED_NUM_FACE_DIR,
};

// 12 edge directions (in the order proposed by Palagyi)
enum EEdgeDir {
    ED_UN, ED_SW, ED_DE,
    ED_NW, ED_UE, ED_DS,
    ED_NE, ED_US, ED_DW,
    ED_SE, ED_UW, ED_DN,
    ED_NUM_EDGE_DIR,
};

// 8 corner directions
enum ECornerDir {
    ED_UNW, ED_DSE,
    ED_UNE, ED_DSW,
    ED_USE, ED_DNW,
    ED_USW, ED_DNE,
    ED_NUM_CORNER_DIR,
};

enum EPrimary {
    EP_ALL          = 0x01,
    EP_FACE_U       = 0x02,
    EP_EDGE_UN      = 0x04,
    EP_CORNER_UNW   = 0x08,
};

// symmetries for U-direction of 6-subiteration
namespace NSymmetry {
    // init internal tables
    void Initialize();

    enum {
        NUM_FLIP        = 2,    // any flmip
        NUM_ROTATE_U    = 4,    // rotate U-face
        NUM_ROTATE_UN   = 2,    // rotate UN-edgne
        NUM_ROTATE_UNW  = 3,    // rotate UNW-corner
        NUM_DIRECT_U    = 6,    // U --> U, D, N, S, W, E
        NUM_DIRECT_UN   = 12,   // UN --> { EEdgeDir }
        NUM_DIRECT_UNW  = 8,    // UNW --> { ECornerDir }
        NUM_SYM_U       = NUM_FLIP * NUM_ROTATE_U,
        NUM_SYM_UN      = NUM_FLIP * NUM_ROTATE_UN,
        NUM_SYM_UNW     = NUM_FLIP * NUM_ROTATE_UNW,
        NUM_SYM_ALL     = NUM_SYM_U * NUM_DIRECT_U, 
    };

    struct Perm {
        int m_idx[27];
        int& operator[]( size_t n ) { return m_idx[n]; }
        const int& operator[]( size_t n ) const { return m_idx[n]; }
    };

    namespace NPerm {
        extern Perm PermIdent[1];
        extern Perm PermFlipX[NUM_FLIP];
        extern Perm PermFlipXY[NUM_FLIP];
        extern Perm PermFlipXYZ[NUM_FLIP];
        extern Perm PermRotateU[NUM_ROTATE_U];
        extern Perm PermRotateUN[NUM_ROTATE_UN];
        extern Perm PermRotateUNW[NUM_ROTATE_UNW];
        extern Perm PermDirectU[NUM_DIRECT_U];
        extern Perm PermDirectUN[NUM_DIRECT_UN];
        extern Perm PermDirectUNW[NUM_DIRECT_UNW];
        extern Perm PermSymU[NUM_SYM_U];            // FlipX * RotateU
        extern Perm PermSymUN[NUM_SYM_UN];          // FlipX * RotateUN
        extern Perm PermSymUNW[NUM_SYM_UNW];        // FlipYZ * RotateUNW
        extern Perm PermSymAll[NUM_SYM_ALL];        // FlipX * RotateU * DirectU
    }

    int TransformBits( int bits, const Perm& perm );
    Mask TransformMask( const Mask& mask, const Perm& perm );

    template<size_t N>
    void TransformBitsN( int bits, int* syms, const Perm (&perms)[N] ) {
        for (size_t n = 0; n < N; ++n) {
            syms[n] = TransformBits( bits, perms[n] );
        }
    }

    // basic transformations
    inline void RotateBitsU( int bits, int syms[NUM_ROTATE_U] ) { TransformBitsN( bits, syms, NPerm::PermRotateU ); }
    inline void RotateBitsUN( int bits, int syms[NUM_ROTATE_UN] ) { TransformBitsN( bits, syms, NPerm::PermRotateUN ); }
    inline void RotateBitsUNW( int bits, int syms[NUM_ROTATE_UNW] ) { TransformBitsN( bits, syms, NPerm::PermRotateUNW ); }

    // combinations
    inline void TransformBitsU( int bits, int syms[NUM_SYM_U] ) { TransformBitsN( bits, syms, NPerm::PermSymU ); }
    inline void TransformBitsUN( int bits, int syms[NUM_SYM_UN] ) { TransformBitsN( bits, syms, NPerm::PermSymUN ); }
    inline void TransformBitsUNW( int bits, int syms[NUM_SYM_UNW] ) { TransformBitsN( bits, syms, NPerm::PermSymUNW ); }
    inline void TransformBitsAll( int bits, int syms[NUM_SYM_ALL] ) { TransformBitsN( bits, syms, NPerm::PermSymAll ); }
}

namespace NSymmetry {
    void CreatePrimaryTable( std::vector<std::uint8_t>& table );
}

namespace NSymmetry {
    template<size_t N, size_t M>
    void InstantiateTemplates( std::vector<std::vector<Mask>>& table, const Mask (&tmpls)[N], const Perm (&perms)[M] ) {
        for each (const Mask tmpl in tmpls) {
            std::vector<Mask> masks;
            for each (const Perm& perm in perms) {
                const Mask sym = TransformMask( tmpl, perm );
                if (find( masks.begin(), masks.end(), sym ) == masks.end()) {
                    masks.push_back( sym );
                }
            }
            table.push_back( masks );
        }
    }

    template<size_t M>
    void TransformMaskTable( std::vector<std::vector<std::vector<Mask>>>& tables, const std::vector<std::vector<Mask>>& table, const Perm (&perms)[M] ) {
        for each (const Perm& perm in perms) {
            std::vector<std::vector<Mask>> _table;
            for each (const std::vector<Mask>& masks in table) {
                std::vector<Mask> _masks;
                for each (const Mask& mask in masks) {
                    _masks.push_back( TransformMask( mask, perm ) );
                }
                _table.push_back( _masks );
            }
            tables.push_back( _table );
        }
    }

    template<size_t N, size_t MSYM, size_t MDIR>
    void CreateMaskTables( std::vector<std::vector<std::vector<Mask>>>& tables, const Mask (&tmpls)[N], const Perm (&perms_sym)[MSYM], const Perm (&perms_dir)[MDIR] ) {
        std::vector<std::vector<Mask>> table;
        InstantiateTemplates( table, tmpls, perms_sym );
        TransformMaskTable( tables, table, perms_dir );
    }

    template<size_t N>
    void CreateMaskTablesU( std::vector<std::vector<std::vector<Mask>>>& tables, const Mask (&tmpls)[N] ) {
        CreateMaskTables( tables, tmpls, NPerm::PermSymU, NPerm::PermDirectU );
    }

    template<size_t N>
    void CreateMaskTablesUN( std::vector<std::vector<std::vector<Mask>>>& tables, const Mask (&tmpls)[N] ) {
        CreateMaskTables( tables, tmpls, NPerm::PermSymUN, NPerm::PermDirectUN );
    }

    template<size_t N>
    void CreateMaskTablesAll( std::vector<std::vector<std::vector<Mask>>>& tables, const Mask (&tmpls)[N] ) {
        CreateMaskTables( tables, tmpls, NPerm::PermSymAll, NPerm::PermIdent );
    }
}
