#include "stdafx.h"
#include "Coord.h"
#include "Mask.h"
#include "Symmetry.h"

#include "Util.h"

using namespace std;

int CountBits( int bits ) {
    bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
    return (bits & 0x0000ffff) + (bits >>16 & 0x0000ffff);
}

void fprintd( FILE* fp, uint32_t value ) {
    int digits[10];
    {// decimal digits
        int dec = value;
        for (int n = 0; n < _countof( digits ); ++n) {
            digits[n] = dec % 10;
            dec /= 10;
        }
    }
    bool spc = true;
    for (int n = _countof( digits ) - 1; n >= 0; --n) {
        if (digits[n] || n == 0) spc = false;
        if (spc) {
            fputc( ' ', fp );
        } else {
            fputc( '0' + digits[n], fp );
        }
        if (n && (n % 3) == 0) {
            fputc( (spc) ? ' ' : ',', fp );
        }
    }
}

namespace {
    void dumpLine( char ch, const int* bits = NULL ) {
        for (int z = -1; z <= 1; ++z) {
            printf( " " );
            for (int i = 0; i < 9; ++i) {
                printf( "%c", ch );
            }
            printf( "   " );
        }
        if (bits) {
            printf( "bits = %07x", *bits );
        }
        printf( "\n" );
    }

    void dumpMask( int mask, int value, int nzero1, int nzero2 ) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                printf( "|" );
                for (int x = -1; x <= 1; ++x) {
                    const int idx = getIndex( x, y, z );
                    const int bit = 1 << idx;
                    char ch;
                    if (mask & bit) {
                        ch = (value & bit) ? 'o' : '.';
                    } else {
                        ch = (nzero1 & bit) ? 'x' : (nzero2 & bit) ? '+' : ' ';
                    }
                    printf( " %c ", ch );
                }
                printf( "|  " );
            }
            printf( "\n" );
        }
    }

    void fprint_mask_bits( FILE* fp, int bits ) {
        if (bits == 0) {
            fprintf( fp, "0" );
            return;
        }
        fprintf( fp, "BV( " );
        for (int j = 0; j < 3; ++j) {
            const int v0 = bits & 7; bits >>= 3;
            const int v1 = bits & 7; bits >>= 3;
            const int v2 = bits & 7; bits >>= 3;
            fprintf( fp, "BP(%d,%d,%d)", v0, v1, v2 );
            fprintf( fp, (j < 2) ? ", " : " )" );
        }
    }
}

void DumpMask( int mask, int value, int nzero1, int nzero2 ) {
    dumpLine( '_', &value );
    dumpMask( mask, value, nzero1, nzero2 );
    dumpLine( '-' );
}

void DumpMask( const Mask& mask ) {
    DumpMask( mask.mask, mask.value, mask.nzero1, mask.nzero2 );
}

void fprint_mask( FILE* fp, const Mask& mask ) {
    fprintf( fp, "Mask( " );
    fprint_mask_bits( fp, mask.mask );
    fprintf( fp, ", " );
    fprint_mask_bits( fp, mask.value );
    if (mask.nzero1) { fprintf( fp, ", " ); fprint_mask_bits( fp, mask.nzero1 ); }
    if (mask.nzero2) { fprintf( fp, ", " ); fprint_mask_bits( fp, mask.nzero2 ); }
    fprintf( fp, " )" );
}

namespace NByteTable {
    bool LoadTable( vector<uint8_t>& table, const char* path ) {
        FILE* fp = fopen( path, "rb" );
        if (fp == NULL) return false;
        table.resize( 1 << 27 );
        const bool ret = (fread( &table[0], table.size(), 1, fp ) == 1);
        fclose( fp );
        return ret;
    }

    bool SaveTable( const vector<uint8_t>& table, const char* path ) {
        if (table.size() != (1 << 27)) return false;
        FILE* fp = fopen( path, "wb" );
        if (fp == NULL) return false;
        const bool ret = (fwrite( &table[0], table.size(), 1, fp ) == 1);
        fclose( fp );
        return ret;
    }

    void ShowBitStat( const vector<uint8_t>& table ) {
        int total = 0;
        int cnt[8] = { 0 };
        int xcnt[8][8] = { 0 };
        for each (uint8_t val in table) {
            if (val == 0) continue;
            total++;
            for (size_t n = 0; n < _countof( cnt ); ++n) {
                if ((val & (1 << n)) == 0) continue;
                cnt[n]++;
                for (size_t m = 0; m <= n; ++m) {
                    if (val & (1 << m)) {
                        xcnt[n][m]++;
                    }
                }
            }
        }
        printf( "Non-Zero = " );
        printd( total );
        printf( "\n\n" );

        size_t rows = 0;
        for (size_t n = 0; n < _countof( cnt ); ++n) {
            if (cnt[n]) {
                rows = n;
            }
        }
        rows++;

        printf( "(Occurrences)\n" );
        for (size_t n = 0; n < rows; ++n) {
            printf( "bit%d|", n );
            printd( cnt[n] );
            printf( "\n" );
        }
        printf( "\n" );
        printf( "(Co-occurrences, 100%% = Non-Zero = %d)\n", total );
        {
            printf( "    " );
            for (size_t m = 0; m < rows; ++m) {
                printf( "|      bit%d     ", m );
            }
            printf( "\n" );
            printf( "----" );
            for (size_t m = 0; m < rows; ++m) {
                printf( "+---------------", m );
            }
            printf( "\n" );
        }
        for (size_t n = 0; n < rows; ++n) {
            printf( "bit%d", n );
            for (size_t m = 0; m <= n; ++m) {
                const int val = xcnt[n][m];
                if (val == 0) {
                    printf( "|        0      " );
                } else {
                    printf( "|%8d (%3.0f%%)", val, 100.0 * val / total );
                }
            }
            printf( "|\n" );
        }
        printf( "\n" );
    }
}

namespace NUtil {
    void GenerateHeader( const std::vector<std::vector<std::vector<Mask>>>& tables, const std::string& path ) {
        FILE* fp = fopen( path.c_str(), "w" );
        if (fp == NULL) return;
        const bool bMulti = (tables.size() >= 2);
        size_t cnt = 0;
        for each (const vector<Mask>& syms in tables.front()) {
            cnt += syms.size();
        }
        fprintf( fp, "#include \"stdafx.h\"\n\n" );
        fprintf( fp, "#include \"Mask.h\"\n\n" );
        fprintf( fp, "const Mask mask_table%s[%d] = {\n", (bMulti) ? "[]" : "", cnt );
        int dir = 0;
        for each (const vector<vector<Mask>>& table in tables) {
            if (bMulti) {
                fprintf( fp, "    {// dir = %d\n", dir++ );
            }
            for (size_t m = 0; m < table.size(); ++m) {
                fprintf( fp, "    %s// M%d\n", (bMulti) ? "    " : "", m+1 );
                for each (const Mask& sym in table[m]) {
                    fprintf( fp, "    %s", (bMulti) ? "    " : "" );
                    fprint_mask( fp, sym );
                    fprintf( fp, ",\n" );
                }
            }
            if (bMulti) {
                fprintf( fp, "    },\n" );
            }
        }
        fprintf( fp, "};\n" );
        fclose( fp );
    }
}
