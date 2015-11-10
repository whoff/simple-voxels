

# Introduction #
The spurious branches, which are often observed with topological thinning algorithms, are ending with "spurious" end points.
The removal of "spurious" end points thus leads to the reduction of the spurious branches.
In fact, with the deletable conditions proposed by
[Raynal and Couprie](RaynalCouprie.md) or those by
[Lohou and Bertrand](LohouBertrand.md),
the spurious branches are considerably reduced as these methods
remove more simple points including such spurious end points at each subiteration than [Palagyi and Kuba](PalagyiKuba.md)'s method does.

Another way of reducing the spurious branches is
to suppress the generation of the spurious end points.
The 3x3x3 template matching is a very efficient technique,
however, as it is based on local information only,
objects are not always thinned uniformly through iterations
leaving noisy voxels on the object surface.

In [[4](Top#References.md)],
Nemeth, Kardos, and Palagyi proposed additional 2 subiterations
to remove the noisy voxels and smooth the object surface,
which are used as a preprocessing before the major thinning iterations.

The removal of noisy voxels can also improve the obtained skeletons.
Especially in the case with large or thick objects that needs more thinning iterations,
the skeletons may deviate more from their medial lines because
the object surface is fluctuated at every iteration due to the noisy voxels.

# Matching Templates #
The following is the base templates for the U-, UN-, and UNW-directions,
which are translated into
[Mask](http://code.google.com/p/simple-voxels/source/browse/trunk/Mask.h) structure.
To construct a 2-subiteration smoothing,
the appropriate rotations and reflections are generated as matching masks as shown in the next section.

```
namespace NNemeth {

    const Mask TemplatesU[3] = {
        Mask( BV( BP(0,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(0,2,0), BP(0,2,0), BP(0,0,0) ), BV( BP(7,5,7), BP(0,0,0), BP(0,0,0) ) ),   // U0
        Mask( BV( BP(1,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(1,2,0), BP(1,2,0), BP(0,0,0) ), BV( BP(6,5,7), BP(0,0,0), BP(0,0,0) ) ),   // U1
        Mask( BV( BP(2,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(2,2,0), BP(2,2,0), BP(0,0,0) ), BV( BP(5,5,7), BP(0,0,0), BP(0,0,0) ) ),   // U2
    };

    const Mask TemplatesUN[1] = {
        Mask( BV( BP(2,0,7), BP(0,7,7), BP(7,7,7) ), BV( BP(2,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(5,7,0), BP(7,0,0), BP(0,0,0) ) ),   // UN
    };

    const Mask TemplatesUNW[1] = {
        Mask( BV( BP(5,4,7), BP(4,6,7), BP(7,7,7) ), BV( BP(1,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(2,3,0), BP(3,1,0), BP(0,0,0) ) ),   // UNW
    };
}
```

Here is the console dump of the above templates:

![http://simple-voxels.googlecode.com/svn/wiki/images/nemeth-templates.png](http://simple-voxels.googlecode.com/svn/wiki/images/nemeth-templates.png)

# Implementation #
In [[4](Top#References.md)], the authors proposed the use of a 8MB table
to detect the deletable points assigning a bit for each one of 2^26 = 64M configurations.
However, in this project, the _old_ way of the matching templates are employed for the illustration purpose.

By enumerating all the symmetric configurations of each template and removing redundant ones,
the [list](http://code.google.com/p/simple-voxels/source/browse/trunk/Nemeth_table.h)
of matching templates can be obtained for the UNW-direction as follows:
```
const Mask nemeth_tables[2][37] = {
    {// dir = 0
        // M1
        Mask( BV( BP(0,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(0,2,0), BP(0,2,0), BP(0,0,0) ), BV( BP(7,5,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,7), BP(2,7,7), BP(0,7,7) ), BV( BP(0,0,0), BP(2,2,0), BP(0,0,0) ), BV( BP(7,0,0), BP(5,0,0), BP(7,0,0) ) ),
        Mask( BV( BP(6,6,6), BP(6,7,6), BP(6,6,6) ), BV( BP(0,0,0), BP(0,3,0), BP(0,0,0) ), BV( BP(1,1,1), BP(1,0,1), BP(1,1,1) ) ),
        // M2
        Mask( BV( BP(1,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(1,2,0), BP(1,2,0), BP(0,0,0) ), BV( BP(6,5,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,7), BP(2,7,7), BP(1,7,7) ), BV( BP(0,0,0), BP(2,2,0), BP(1,1,0) ), BV( BP(7,0,0), BP(5,0,0), BP(6,0,0) ) ),
        Mask( BV( BP(6,6,6), BP(6,7,6), BP(7,6,6) ), BV( BP(0,0,0), BP(0,3,0), BP(3,0,0) ), BV( BP(1,1,1), BP(1,0,1), BP(0,1,1) ) ),
        Mask( BV( BP(4,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(4,2,0), BP(4,2,0), BP(0,0,0) ), BV( BP(3,5,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,7), BP(2,7,7), BP(4,7,7) ), BV( BP(0,0,0), BP(2,2,0), BP(4,4,0) ), BV( BP(7,0,0), BP(5,0,0), BP(3,0,0) ) ),
        Mask( BV( BP(7,6,6), BP(6,7,6), BP(6,6,6) ), BV( BP(3,0,0), BP(0,3,0), BP(0,0,0) ), BV( BP(0,1,1), BP(1,0,1), BP(1,1,1) ) ),
        Mask( BV( BP(0,2,4), BP(7,7,7), BP(7,7,7) ), BV( BP(0,2,4), BP(0,2,4), BP(0,0,0) ), BV( BP(7,5,3), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(4,7,7), BP(2,7,7), BP(0,7,7) ), BV( BP(4,4,0), BP(2,2,0), BP(0,0,0) ), BV( BP(3,0,0), BP(5,0,0), BP(7,0,0) ) ),
        Mask( BV( BP(6,6,7), BP(6,7,6), BP(6,6,6) ), BV( BP(0,0,3), BP(0,3,0), BP(0,0,0) ), BV( BP(1,1,0), BP(1,0,1), BP(1,1,1) ) ),
        Mask( BV( BP(0,2,1), BP(7,7,7), BP(7,7,7) ), BV( BP(0,2,1), BP(0,2,1), BP(0,0,0) ), BV( BP(7,5,6), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(1,7,7), BP(2,7,7), BP(0,7,7) ), BV( BP(1,1,0), BP(2,2,0), BP(0,0,0) ), BV( BP(6,0,0), BP(5,0,0), BP(7,0,0) ) ),
        Mask( BV( BP(6,6,6), BP(6,7,6), BP(6,6,7) ), BV( BP(0,0,0), BP(0,3,0), BP(0,0,3) ), BV( BP(1,1,1), BP(1,0,1), BP(1,1,0) ) ),
        // M3
        Mask( BV( BP(2,2,0), BP(7,7,7), BP(7,7,7) ), BV( BP(2,2,0), BP(2,2,0), BP(0,0,0) ), BV( BP(5,5,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,7), BP(2,7,7), BP(2,7,7) ), BV( BP(0,0,0), BP(2,2,0), BP(2,2,0) ), BV( BP(7,0,0), BP(5,0,0), BP(5,0,0) ) ),
        Mask( BV( BP(6,6,6), BP(7,7,6), BP(6,6,6) ), BV( BP(0,0,0), BP(3,3,0), BP(0,0,0) ), BV( BP(1,1,1), BP(0,0,1), BP(1,1,1) ) ),
        Mask( BV( BP(0,6,0), BP(7,7,7), BP(7,7,7) ), BV( BP(0,6,0), BP(0,6,0), BP(0,0,0) ), BV( BP(7,1,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,7), BP(6,7,7), BP(0,7,7) ), BV( BP(0,0,0), BP(6,6,0), BP(0,0,0) ), BV( BP(7,0,0), BP(1,0,0), BP(7,0,0) ) ),
        Mask( BV( BP(6,7,6), BP(6,7,6), BP(6,6,6) ), BV( BP(0,3,0), BP(0,3,0), BP(0,0,0) ), BV( BP(1,0,1), BP(1,0,1), BP(1,1,1) ) ),
        Mask( BV( BP(0,2,2), BP(7,7,7), BP(7,7,7) ), BV( BP(0,2,2), BP(0,2,2), BP(0,0,0) ), BV( BP(7,5,5), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(2,7,7), BP(2,7,7), BP(0,7,7) ), BV( BP(2,2,0), BP(2,2,0), BP(0,0,0) ), BV( BP(5,0,0), BP(5,0,0), BP(7,0,0) ) ),
        Mask( BV( BP(6,6,6), BP(6,7,7), BP(6,6,6) ), BV( BP(0,0,0), BP(0,3,3), BP(0,0,0) ), BV( BP(1,1,1), BP(1,0,0), BP(1,1,1) ) ),
        Mask( BV( BP(0,3,0), BP(7,7,7), BP(7,7,7) ), BV( BP(0,3,0), BP(0,3,0), BP(0,0,0) ), BV( BP(7,4,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(0,7,7), BP(3,7,7), BP(0,7,7) ), BV( BP(0,0,0), BP(3,3,0), BP(0,0,0) ), BV( BP(7,0,0), BP(4,0,0), BP(7,0,0) ) ),
        Mask( BV( BP(6,6,6), BP(6,7,6), BP(6,7,6) ), BV( BP(0,0,0), BP(0,3,0), BP(0,3,0) ), BV( BP(1,1,1), BP(1,0,1), BP(1,0,1) ) ),
        // M4
        Mask( BV( BP(2,0,7), BP(0,7,7), BP(7,7,7) ), BV( BP(2,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(5,7,0), BP(7,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(1,5,1), BP(3,3,3), BP(7,7,7) ), BV( BP(0,4,0), BP(0,2,0), BP(0,0,0) ), BV( BP(6,2,6), BP(4,4,4), BP(0,0,0) ) ),
        Mask( BV( BP(7,0,2), BP(7,7,0), BP(7,7,7) ), BV( BP(0,0,2), BP(0,2,0), BP(0,0,0) ), BV( BP(0,7,5), BP(0,0,7), BP(0,0,0) ) ),
        Mask( BV( BP(4,5,4), BP(6,6,6), BP(7,7,7) ), BV( BP(0,1,0), BP(0,2,0), BP(0,0,0) ), BV( BP(3,2,3), BP(1,1,1), BP(0,0,0) ) ),
        Mask( BV( BP(4,6,7), BP(5,6,7), BP(4,6,7) ), BV( BP(0,0,0), BP(1,2,0), BP(0,0,0) ), BV( BP(3,1,0), BP(2,1,0), BP(3,1,0) ) ),
        Mask( BV( BP(1,3,7), BP(5,3,7), BP(1,3,7) ), BV( BP(0,0,0), BP(4,2,0), BP(0,0,0) ), BV( BP(6,4,0), BP(2,4,0), BP(6,4,0) ) ),
        // M5
        Mask( BV( BP(5,4,7), BP(4,6,7), BP(7,7,7) ), BV( BP(1,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(2,3,0), BP(3,1,0), BP(0,0,0) ) ),
        Mask( BV( BP(5,1,7), BP(1,3,7), BP(7,7,7) ), BV( BP(4,0,0), BP(0,2,0), BP(0,0,0) ), BV( BP(2,6,0), BP(6,4,0), BP(0,0,0) ) ),
        Mask( BV( BP(7,1,5), BP(7,3,1), BP(7,7,7) ), BV( BP(0,0,4), BP(0,2,0), BP(0,0,0) ), BV( BP(0,6,2), BP(0,4,6), BP(0,0,0) ) ),
        Mask( BV( BP(7,4,5), BP(7,6,4), BP(7,7,7) ), BV( BP(0,0,1), BP(0,2,0), BP(0,0,0) ), BV( BP(0,3,2), BP(0,1,3), BP(0,0,0) ) ),
    },
//...
};
```

In [[4](Top#References.md)],
The matching templates are divided into two sets [R1](https://code.google.com/p/simple-voxels/source/detail?r=1), [R2](https://code.google.com/p/simple-voxels/source/detail?r=2),
where [R1](https://code.google.com/p/simple-voxels/source/detail?r=1) contains masks that are for the UNW-direction,
and [R2](https://code.google.com/p/simple-voxels/source/detail?r=2) for the DSE-direction, respectively.
In the above table, the table index 0 is for [R1](https://code.google.com/p/simple-voxels/source/detail?r=1) and 1 is for [R2](https://code.google.com/p/simple-voxels/source/detail?r=2).

By using the above list, the deletability of a voxel can be determined by
```
namespace NNemeth {
    bool IsDeletable( int bits, int idx ) {
        for each (const Mask& mask in nemeth_tables[(int)idx]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
```
where idx is 0 (UNW) or 1 (DSE).

This function is defined in [Nemeth.cpp](http://code.google.com/p/simple-voxels/source/browse/trunk/Nemeth.cpp#74).

# Statistics #
The next table shows how many simple configurations will be deleted by the above templates for the two indices:

![http://simple-voxels.googlecode.com/svn/wiki/images/nemeth-stat.png](http://simple-voxels.googlecode.com/svn/wiki/images/nemeth-stat.png)

where bit0 - bit1 denotes the following indices:
| **Bit** | **Index** |
|:--------|:----------|
| bit0    | 0         |
| bit1    | 1         |

The number of deletable configurations is not many, however,
it _does_ improve the obtained skeletons.

# For Ultimate Thinning #
This is also mentioned in [[4](Top#References.md)],
for the ultimate thinning purpose,
one can loosen the matching templates to allow removing end points
by not checking the at-least-one-foreground-point positions ('x's in the console dump).