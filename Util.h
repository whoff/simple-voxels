#pragma once

// count the number of `1' bits
int CountBits( int bits );

// print a decimal number with commas
void fprintd( FILE* fp, std::uint32_t value );
inline void printd( int value ) { fprintd( stdout, value ); }
void fprint_mask( FILE* fp, const struct Mask& mask );

// dump bits / mask
void DumpMask( int mask, int value, int nzero1 = 0, int nzero2 = 0 );
void DumpMask( const struct Mask& mask );
inline void DumpBits( int bits ) { DumpMask( -1, bits ); }

// table utility
namespace NByteTable {
    // load / save
    bool LoadTable( std::vector<std::uint8_t>& table, const char* path );
    bool SaveTable( const std::vector<std::uint8_t>& table, const char* path );

    // count non-zero patterns
    void ShowBitStat ( const std::vector<std::uint8_t>& table );
}

namespace NUtil {
    // tables: direction / symmetries / templates
    void GenerateHeader( const std::vector<std::vector<std::vector<Mask>>>& tables, const std::string& path );
}
