#pragma once

#include "Util.h"

// Mask to match bit patterns (such as Palagyi M1 - M6)
//   - Masked bits should be equal to `value'
//   - If `nzero' is not zero, checks if any bit where nzero is one is not zero
struct Mask {
    Mask( int _mask, int _value, int _nzero = 0 ) : mask( _mask ), value( _value ), nzero( _nzero ) {}
    int mask;
    int value;
    int nzero;
    bool Match( int bits ) const {
        if (nzero != 0 && (bits & nzero) == 0) return false;
        return ((mask & bits) == value);
    }
    friend static bool operator==( const Mask& a, const Mask& b) {
        return (a.mask == b.mask && a.value == b.value && a.nzero == b.nzero);
    }
};

// Mask to match bit pattern where one background component separates multiple foregrounds components
//   - Masked value should be zero
//   - Two different non-zero masks
struct MaskZero {
    MaskZero( int _mask, int _nzero1, int _nzero2 ) : mask( _mask ), nzero1( _nzero1 ), nzero2( _nzero2 ) {}
    int mask;
    int nzero1, nzero2;
    bool Match( int bits ) const {
        return ((mask & bits) == 0 && (nzero1 & bits) && (nzero2 & bits));
    }
    friend static bool operator==( const MaskZero& a, const MaskZero& b ) {
        return (a.mask == b.mask && ((a.nzero1 == b.nzero1 && a.nzero2 == b.nzero2) || (a.nzero1 == b.nzero2 && a.nzero2 == b.nzero1)));
    }
    void Dump() const { DumpMask( mask, 0, nzero1, nzero2 ); }
};

void DumpMask( const Mask& mask );
void DumpMask( const MaskZero& mask );

namespace NMask6 {
    void InstantiateUDirMasks( const Mask* templates, size_t num_templates, std::vector<std::vector<Mask>>& table );
    void InstantiateUDirMasks( const MaskZero* templates, size_t num_templates, std::vector<std::vector<MaskZero>>& table );
    void InstantiateOmniDirMasks( const Mask* templates, size_t num_templates, std::vector<std::vector<Mask>>& table );
    void InstantiateOmniDirMasks( const MaskZero* templates, size_t num_templates, std::vector<std::vector<MaskZero>>& table );

    void GenerateDirHeader( const std::vector<std::vector<Mask>>& table, const std::string& path );
    void GenerateDirHeader( const std::vector<std::vector<MaskZero>>& table, const std::string& path );
    void GenerateOmniDirHeader( const std::vector<std::vector<MaskZero>>& table, const std::string& path );
}
