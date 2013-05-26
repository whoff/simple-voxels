#include "stdafx.h"
#include "Symmetry.h"
#include "Util.h"

#include "Neighbors.h"

using namespace std;

Coord N6[6];
Coord N18[18];
Coord N26[26];

namespace NNeighbors {
    // init N6, N18, N26
    void InitNeiborsCoord() {
        int n6 = 0;
        int n18 = 0;
        int n26 = 0;
        for (int z = -1; z <= 1; ++z) {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    const int d = abs( x ) + abs( y ) + abs( z );
                    if (d == 0) continue;
                    if (d <= 1) N6[n6++].Set( x, y, z );
                    if (d <= 2) N18[n18++].Set( x, y, z );
                    if (d <= 3) N26[n26++].Set( x, y, z );
                }
            }
        }
    }

    void Initialize() {
        InitNeiborsCoord();
    }
}

namespace NComponent {
    namespace NImpl {
        // fixed size stack
        template<typename T, int N>
        struct array_stack {
            T stack[N];
            int pos;
            array_stack() : pos( -1 ) {};
            bool empty() const { return (pos == -1); }
            void push( const T& val ) { stack[++pos] = val; }
            void pop() { pos--; }
            T top() { return stack[pos]; }
        };

        // flood fill masked voxels (with the found voxels being unmasked)
        template<int M>
        bool Fill( const Coord (&Nx)[M], Neibors* pneib, const Coord& seed ) {
            Neibors neib = *pneib;
            if (neib.Read( seed ) == false) return false;
            neib.Write( seed, false );
            array_stack<Coord, 27> stack;
            Coord curr = seed;
            while (true) {
                for each (const Coord& delta in Nx) {
                    const Coord next = curr + delta;
                    if (next.IsValid() == false) continue;
                    const int idx = next.GetIndex();
                    if (neib.Read( idx )) {
                        neib.Write( idx, false );
                        stack.push( next );
                    }
                }
                if (stack.empty()) break;
                curr = stack.top();
                stack.pop();
            }
            *pneib = neib;
            return true;
        }

        __inline bool Fill_N6(  Neibors* pneib, const Coord& seed ) { return Fill( N6,  pneib, seed ); }
        __inline bool Fill_N26( Neibors* pneib, const Coord& seed ) { return Fill( N26, pneib, seed ); }
    }

    // count the number of foreground components (26-connected)
    int CountForeCompsN26( int bits ) {
        Neibors neib( bits & N26_bits );
        int cnt = 0;
        for each (const Coord& pos in N26) {
            if (NImpl::Fill_N26( &neib, pos )) {
                cnt++;
            }
        }
        return cnt;
    }

    // count the number of background components (6-connected)
    int CountBackCompsN6( int bits ) {
        Neibors neib( ~bits & N18_bits );
        int cnt = 0;
        for each (const Coord& pos in N6) {
            if (NImpl::Fill_N6( &neib, pos )) {
                cnt++;
            }
        }
        return cnt;
    }

    void CreateCountTable( vector<uint8_t>& table ) {
        const uint8_t INVALID = 0xff;
        table.resize( 1 << 27, INVALID );
        int syms[NSymmetry::NUM_SYM_ALL];
        for (int bits = 0; bits < (1 << 27); ++bits) {
            if ((bits % (1 << 18)) == 0) {
                printf( "creating table... %.0f%%\r", 100.0 * bits / (1 << 27) );
            }
            if (table[bits] != INVALID) continue;
            int val = 0;
            if (bits & N0_bits) {
                const int num_fore = NComponent::CountForeCompsN26( bits );
                const int num_back = NComponent::CountBackCompsN6( bits );
                val = (num_back << 4) | num_fore;
            }
            NSymmetry::TransformBitsAll( bits, syms );
            for each (int sym in syms) {
                table[sym] = val;
            }
        }
        printf( "done\n" );
    }

    void ShowCountStat( const vector<uint8_t>& table ) {
        vector<int> counts( 256, 0 );
        for each (uint8_t nums in table) {
            counts[nums]++;
        }
        for (size_t n = 0; n < counts.size(); ++n) {
            const int count = counts[n];
            if (count == 0) continue;
            const int num_fore = GetForeCount( n );
            const int num_back = GetBackCount( n );
            printf( "(%d, %d) = ", num_fore, num_back );
            printd( count );
            printf( "\n" );
        }
        printf( "\n" );
    }

    void FilterByCount( const vector<uint8_t>& count_table, vector<uint8_t>& table, int fore_min, int fore_max, int back_min, int back_max ) {
        table.resize( 1 << 27 );
        int count = 0;
        for (int bits = 0; bits < (1 << 27); ++bits) {
            bool hit = false;
            if (bits & N0_bits) {
                const int num_fore = GetForeCount( count_table[bits] );
                const int num_back = GetBackCount( count_table[bits] );
                hit = (fore_min <= num_fore && num_fore <= fore_max
                    && back_min <= num_back && num_back <= back_max);
            }
            table[bits] = (hit) ? 1 : 0;
            if (hit) count++;
        }
        printf( "count = " );
        printd( count );
        printf( "\n" );
    }
}
