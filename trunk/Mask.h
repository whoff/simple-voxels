#pragma once

// Mask to match bit patterns (such as Palagyi M1 - M6)
//   - Masked bits should be equal to `value'
//   - If `nzero1' or `nzero2' is not zero, checks if any bit where nzero1/2 is one is not zero
struct Mask {
    Mask( int _mask, int _value, int _nzero1 = 0, int _nzero2 = 0 ) : mask( _mask ), value( _value ), nzero1( _nzero1 ), nzero2( _nzero2 ) {}
    int mask;
    int value;
    int nzero1, nzero2;
    bool Match( int bits ) const {
        return (mask & bits) == value
            && (nzero1 == 0 || (bits & nzero1) != 0)
            && (nzero2 == 0 || (bits & nzero2) != 0);
    }
    friend static bool operator==( const Mask& a, const Mask& b) {
        return (a.mask == b.mask && a.value == b.value
            && ((a.nzero1 == b.nzero1 && a.nzero2 == b.nzero2) || (a.nzero1 == b.nzero2 && a.nzero2 == b.nzero1)));
    }
};
