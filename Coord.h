#pragma once

enum {
    DX = 1,
    DY = 3,
    DZ = 9,
};

// 3x3x3 range [-1, +1]^3
inline bool isValid( int x, int y, int z ) { return (-1 <= x && x <= 1 && -1 <= y && y <= 1 && -1 <= z && z <= 1); }
// 3x3x3 index [0, 27)
inline int getIndex( int x, int y, int z ) { return DX * (x + 1) + DY * (y + 1) + DZ * (z + 1); }
inline int getShift( int dx, int dy, int dz ) { return DX * dx + DY * dy + DZ * dz; }

struct Coord {
    int x, y, z;
    Coord() : x( 0 ), y( 0 ), z( 0 ) {}
    Coord( int _x, int _y, int _z ) : x( _x ), y( _y ), z( _z ) {}
    void Set( int _x, int _y, int _z ) { x = _x; y = _y; z = _z; }
    bool IsValid() const { return isValid( x, y, z ); }
    int GetIndex() const { return getIndex( x, y, z ); }
    friend static Coord operator+( const Coord& a, const Coord& b ) { return Coord( a.x + b.x, a.y + b.y, a.z + b.z ); }
};

// bit plane & volume macros
#define BP( Y0, Y1, Y2 ) ((Y0) | ((Y1) << DY) | ((Y2) << (2*DY)))
#define BV( Z0, Z1, Z2 ) ((Z0) | ((Z1) << DZ) | ((Z2) << (2*DZ)))
