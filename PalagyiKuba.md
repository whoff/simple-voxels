**Contents**


# Matching Templates #
In [[1](Top#References.md)],
Palagyi and Kuba proposed a 6-subiteration thinning algorithm that was based on a set of six 3x3x3 matching templates.
Please refer to the paper for the definition of the matching templates.

The following is their base templates for the U-direction
translated into
[Mask](http://code.google.com/p/simple-voxels/source/browse/trunk/Mask.h) structure.
```
namespace NPalagyi {

    const Mask Templates[6] = {
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(7,5,7), BP(7,5,7) ) ),   // M1
        Mask( BV( BP(0,7,7), BP(2,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,2,0), BP(0,2,0) ) ),   // M2
        Mask( BV( BP(0,3,3), BP(2,6,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,6,0), BP(0,2,0) ) ),   // M3
        Mask( BV( BP(7,7,7), BP(4,2,0), BP(0,2,0) ), BV( BP(4,0,0), BP(4,2,0), BP(0,2,0) ) ),   // M4
        Mask( BV( BP(7,7,7), BP(0,2,7), BP(2,2,7) ), BV( BP(0,0,0), BP(0,2,0), BP(2,0,0) ), BV( BP(0,0,0), BP(7,5,0), BP(5,5,0) ) ),   // M5
        Mask( BV( BP(7,7,7), BP(0,3,3), BP(2,7,3) ), BV( BP(0,0,0), BP(0,2,0), BP(2,4,0) ) ),   // M6
    };
}
```

As for the notations, see also [Matching Templates](Notations#Matching_Templates.md).

It should be noted that their templates are carefully designed not to remove end points of curvilinear structures for the skeletonization purpose.

Here is the console dump of the above templates:

![http://simple-voxels.googlecode.com/svn/wiki/images/palagyi-templates.png](http://simple-voxels.googlecode.com/svn/wiki/images/palagyi-templates.png)

# Implementation #
By enumerating all the symmetric configurations of each template within the NSWE plane (there are eight of them) and removing redundant ones,
the [list](http://code.google.com/p/simple-voxels/source/browse/trunk/Palagyi_table.h)
of matching templates can be obtained for the U-direction as follows:
```
const Mask palagyi_tables[][21] = {
    {// dir = 0
        // M1
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(7,5,7), BP(7,5,7) ) ),
        // M2
        Mask( BV( BP(0,7,7), BP(2,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,2,0), BP(0,2,0) ) ),
        Mask( BV( BP(3,3,3), BP(0,6,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,6,0), BP(0,2,0) ) ),
        Mask( BV( BP(7,7,0), BP(0,2,2), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,2), BP(0,2,0) ) ),
        Mask( BV( BP(6,6,6), BP(0,3,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,3,0), BP(0,2,0) ) ),
        // M3
        Mask( BV( BP(0,3,3), BP(2,6,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,6,0), BP(0,2,0) ) ),
        Mask( BV( BP(0,6,6), BP(2,3,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,3,0), BP(0,2,0) ) ),
        Mask( BV( BP(3,3,0), BP(0,6,2), BP(0,2,0) ), BV( BP(0,0,0), BP(0,6,2), BP(0,2,0) ) ),
        Mask( BV( BP(6,6,0), BP(0,3,2), BP(0,2,0) ), BV( BP(0,0,0), BP(0,3,2), BP(0,2,0) ) ),
        // M4
        Mask( BV( BP(7,7,7), BP(4,2,0), BP(0,2,0) ), BV( BP(4,0,0), BP(4,2,0), BP(0,2,0) ) ),
        Mask( BV( BP(7,7,7), BP(1,2,0), BP(0,2,0) ), BV( BP(1,0,0), BP(1,2,0), BP(0,2,0) ) ),
        Mask( BV( BP(7,7,7), BP(0,2,4), BP(0,2,0) ), BV( BP(0,0,4), BP(0,2,4), BP(0,2,0) ) ),
        Mask( BV( BP(7,7,7), BP(0,2,1), BP(0,2,0) ), BV( BP(0,0,1), BP(0,2,1), BP(0,2,0) ) ),
        // M5
        Mask( BV( BP(7,7,7), BP(0,2,7), BP(2,2,7) ), BV( BP(0,0,0), BP(0,2,0), BP(2,0,0) ), BV( BP(0,0,0), BP(7,5,0), BP(5,5,0) ) ),
        Mask( BV( BP(7,7,7), BP(1,3,1), BP(1,7,1) ), BV( BP(0,0,0), BP(0,2,0), BP(0,4,0) ), BV( BP(0,0,0), BP(6,4,6), BP(6,0,6) ) ),
        Mask( BV( BP(7,7,7), BP(7,2,0), BP(7,2,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,2) ), BV( BP(0,0,0), BP(0,5,7), BP(0,5,5) ) ),
        Mask( BV( BP(7,7,7), BP(4,6,4), BP(4,7,4) ), BV( BP(0,0,0), BP(0,2,0), BP(0,1,0) ), BV( BP(0,0,0), BP(3,1,3), BP(3,0,3) ) ),
        // M6
        Mask( BV( BP(7,7,7), BP(0,3,3), BP(2,7,3) ), BV( BP(0,0,0), BP(0,2,0), BP(2,4,0) ) ),
        Mask( BV( BP(7,7,7), BP(0,6,6), BP(2,7,6) ), BV( BP(0,0,0), BP(0,2,0), BP(2,1,0) ) ),
        Mask( BV( BP(7,7,7), BP(3,3,0), BP(3,7,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,4,2) ) ),
        Mask( BV( BP(7,7,7), BP(6,6,0), BP(6,7,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,1,2) ) ),
    },
//...
};
```

By using this list, the deletability of a voxel can be determined by
```
namespace NPalagyi {
    bool IsDeletable( int bits, EFaceDir dir ) {
        for each (const Mask& mask in palagyi_tables[(int)dir]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
```
This function is defined in [Palagyi.cpp](http://code.google.com/p/simple-voxels/source/browse/trunk/Palagyi.cpp).

# Statistics #
The next table shows how many simple configurations can be deleted by the above templates for each of the six directions:

![http://simple-voxels.googlecode.com/svn/wiki/images/palagyi-stat.png](http://simple-voxels.googlecode.com/svn/wiki/images/palagyi-stat.png)

where bit0 - bit5 denotes the following directions:
| **Bit** | **Direction** |
|:--------|:--------------|
| bit0    | U             |
| bit1    | D             |
| bit2    | N             |
| bit3    | S             |
| bit4    | W             |
| bit5    | E             |

It can be observed that:
  * As a whole, 9,916,926 out of 25,985,144 [simple](SimpleVoxels#Simple_Configurations.md) configurations are deleted.
  * For each direction, 2,124,283 configurations are deletable.
  * No common deletable configurations between U and D. (Likewise, between N and S or between W and E.)
  * Between other pairs of directions, 279,655 configurations are deletable in common.