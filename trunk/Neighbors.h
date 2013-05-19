#pragma once

#include "Coord.h"

// Neighbor coordinates
extern Coord N6[6];
extern Coord N18[18];
extern Coord N26[26];

// Neighbor bit patterns
enum {
    N0_bits  = BV( BP(0,0,0), BP(0,2,0), BP(0,0,0) ),
    N6_bits  = BV( BP(0,2,0), BP(2,5,2), BP(0,2,0) ),
    N18_bits = BV( BP(2,7,2), BP(7,5,7), BP(2,7,2) ),
    N26_bits = BV( BP(7,7,7), BP(7,5,7), BP(7,7,7) ),
    N27_bits = BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ),
};

// To traverse neighboring bits
struct Neibors {
    int bits;
    Neibors() : bits( 0 ) {};
    Neibors( int _bits ) : bits( _bits ) {}
    bool Read( int index ) const {
        return (((bits >> index) & 1) == 1);
    }
    void Write( int index, bool value ) {
        if (value) bits |=  (1 << index);
        else       bits &= ~(1 << index);
    }
    bool Read( const Coord& pos ) const { return Read( pos.GetIndex() ); }
    void Write( const Coord& pos, bool value ) { Write( pos.GetIndex(), value ); }
    friend static Neibors operator|( const Neibors& a, const Neibors& b ) { return Neibors( a.bits | b.bits ); }
};

namespace NNeighbors {
    // please call this before using the above functions
    void Initialize();
}

// count the number of connected components in N26
namespace NComponent {
    // count the number of 26-connected `1' components in N26
    int CountForeCompsN26( int bits );
    // count the number of 6-connected `0' components in N18
    int CountBackCompsN6( int bits );

    // compute the number of components for 2^26 patterns
    // table value contains:
    //   lower 4-bit:  the number of foreground components
    //   higher 4-bit: the number of background components
    void CreateCountTable( std::vector<std::uint8_t>& table );
    inline int GetForeCount( std::uint8_t val ) { return val & 0x0f; }
    inline int GetBackCount( std::uint8_t val ) { return val >> 4; }

    // compute and print the statistics of the foreground and the background count statics
    void ShowCountStat( const std::vector<std::uint8_t>& table );

    // set the mask to the following values:
    //   255: (fore_min <= #{fore} <= fore_max) && (back_min <= #{back} <= back_max}
    //     0: otherwise
    void FilterByCount( const std::vector<std::uint8_t>& count_table, std::vector<std::uint8_t>& table, int fore_min, int fore_max, int back_min, int back_max );
}
