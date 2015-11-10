**Contents**


# Voxel Neighbors #
In the implementation throughout this project,
a 3x3x3 neighborhood configuration of a voxel is represented by a 32-bit integer,
whose lower 27 bits represent whether the corresponding position is foreground (black) or background (white) by the bit value '1' or '0', respectively.

The left figure below illustrates the assignment of a neighboring point to the corresponding bit position.

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-bits.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-bits.png)
![http://simple-voxels.googlecode.com/svn/wiki/images/6directions.png](http://simple-voxels.googlecode.com/svn/wiki/images/6directions.png)

The figure on the right shows the direction regarding the center voxel (13):

| **Directions** | **Axis** |
|:---------------|:---------|
| Up, Down       | -z, +z   |
| North, South   | -y, +y   |
| West, East     | -x, +x   |

As can be seen, the integer bits from LSB to MSB
represent the neighbor positions from UNW direction to DSE direction
traversing x, y, and z axes in this order.

# Neighborhood Configurations #
A set of macro functions is defined to make an integer from the 3x3x3 neighborhood configuration. Here is an example:
```
    const int bits = BV( BP(0,0,0), BP(0,2,0), BP(0,0,0) );
```
where BV() and BP() macro names stand for "Bit Volume" and "Bit Plane", respectively.
In BV(), the three BP() macros define the three planes from Up to Down.
Similarly, in BP(), the three numbers define the three lines from North to South,
where each number is a 3-bit integer that represents, from LSB to MSB,
the consecutive three points on each line from West to East.

In the above example, "bits" represents the configuration
where the center voxel is the only voxel that is in the foreground (black, 1) while all the other voxels are in the background (white, 0).

The following code is another example that represents the 3x3x3 configuration depicted in the figure below, where the foreground points are painted, for the illustration purpose, by the same color if they are on the same plane as in the top figure. Namely Up = red, Middle = green, and Down = blue.

```
    const int bits = BV( BP(0,1,2), BP(3,4,5), BP(6,7,0) );
```

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-hihumi.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-hihumi.png)

## Dump Configurations ##
DumpBits() function can be used to print the configurations on the console as follows:
```
    DumpBits( BV( BP(0,1,2), BP(3,4,5), BP(6,7,0) ) );
```

![http://simple-voxels.googlecode.com/svn/wiki/images/cout-neighbor-hihumi.png](http://simple-voxels.googlecode.com/svn/wiki/images/cout-neighbor-hihumi.png)

The foreground voxels are printed as 'o' and the background voxels as '.'.
As in the above, configurations are printed plane-by-plane from Up to Down, from left to right on the console.

# Matching Templates #
To define deletable conditions, 3x3x3 matching templates are often used.
In this project,
[Mask](http://code.google.com/p/simple-voxels/source/browse/trunk/Mask.h)
structure is used to define such matching template masks.
```
struct Mask {
    Mask( int _mask, int _value, int _nzero1 = 0, int _nzero2 = 0 );
    int mask;
    int value;
    int nzero1, nzero2;
    bool Match( int bits ) const {
        return (mask & bits) == value
            && (nzero1 == 0 || (bits & nzero1) != 0)
            && (nzero2 == 0 || (bits & nzero2) != 0);
    }
};
```
In this structure,
  * _mask_ defines the mask of interested bits that will be compared with _value_.
  * _nzero1_ and _nzero2_ are the masks that, if not zero, will be used to check whether at least one of the masked bits is 1 or not.

For example, the following code snippet defines [Palagyi's M1 and M2 masks](http://code.google.com/p/simple-voxels/source/browse/trunk/Palagyi.cpp#13) and [Isthmus M1](http://code.google.com/p/simple-voxels/source/browse/trunk/Isthmus.cpp#42) mask.
```
    const Mask Palagyi_M1(
        BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ),    // mask
        BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ),    // value
        BV( BP(0,0,0), BP(7,5,7), BP(7,5,7) )     // nzero1
    );
    const Mask Palagyi_M2(
        BV( BP(0,7,7), BP(2,2,0), BP(0,2,0) ),    // mask
        BV( BP(0,0,0), BP(2,2,0), BP(0,2,0) )     // value
    );
    const Mask Isthmus_M1(
        BV( BP(0,0,0), BP(7,5,7), BP(0,0,0) ),    // mask
        0,                                        // value
        BV( BP(7,7,7), BP(0,0,0), BP(0,0,0) ),    // nzero1
        BV( BP(0,0,0), BP(0,0,0), BP(7,7,7) )     // nzero2
    );
```

## Dump Templates ##
By using DumpMask() function, the matching template masks can be printed on the console window.
```
    DumpMask( NPalagyi::TemplatesU[0] );
    DumpMask( NPalagyi::TemplatesU[1] );
    DumpMask( NIsthmus::TemplatesU[0] );
```
![http://simple-voxels.googlecode.com/svn/wiki/images/cout-template-masks.png](http://simple-voxels.googlecode.com/svn/wiki/images/cout-template-masks.png)

where each letter in the output denotes:
| **Letter** | **Mask member** | **Description** |
|:-----------|:----------------|:----------------|
| o          | mask & value    | foreground points |
| .          | mask & ~value   | background points |
| ' '        |                 | don't care      |
| x          | nzero1          | at least one x position should be foreground |
| +          | nzero2          | at least one + position should be foreground |