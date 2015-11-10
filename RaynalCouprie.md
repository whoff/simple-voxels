**Contents**


# Introduction #
When the [deletable conditions](PalagyiKuba#Matching_Templates.md) proposed by Palagyi and Kuba is used for thinning implementation,
one finds that numerous spurious branches are generated
along with object skeletons.
This is especially the case when the object resolution is higher such as 512x512x1024, which is however typical in the recent medical imaging applications.
The spurious branches are generated partly due to the skeleton preserving nature of the deletable conditions that are designed to keep end points of curvilinear structures.

In order to address this issue, Raynal and Couprie proposed an isthmus based 6-subiteration thinning algorithm.
In [[2](Top#References.md)], the authors:
  1. loosened Palagyi and Kuba's deletable conditions to further remove end points (see the figure below)
  1. instead prohibited removal of _isthmus_ points

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-end-points.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-end-points.png)

## Isthmus Points ##
The isthmus points are defined as the points that fulfill:
  * The number of 26-connected foreground components >= 2
The isthmus points are therefore the points that connect multiple foreground components. For example, the points on a curve line are all isthmus points except the two end points.

It is expected that, while the spurious branches will be reduced by the removal of end points, the skeletal branches will remain
by prohibiting the removal of the isthmus points.

# Project Proposal #
As for the implementation of the isthmus point detection,
the authors used "look-up table in order to use pre-computation,
for each possible configuration of 26-neighborhood" (Section 4 [2](2.md)).

In order to allow more memory efficient implementation,
I tried to find a set of matching templates for the isthmus points.
In the [latter](RaynalCouprie#Matching_Templates_for_Isthmus_Points.md) of this page,
I will propose a set of six matching templates for the isthmus detection.

# Matching Templates for Deletable Points #
In [[2](Top#References.md)], the authors proposed a variant of Palagyi and Kuba's matching templates to additionally remove end points.

The following is their base templates for the U-direction
translated into
[Mask](http://code.google.com/p/simple-voxels/source/browse/trunk/Mask.h) structure.
```
namespace NRaynal {

    const Mask Templates[7] = {
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ) ),   // M1'
        Mask( BV( BP(0,7,7), BP(2,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,2,0), BP(0,2,0) ) ),   // M2'
        Mask( BV( BP(0,3,3), BP(2,6,0), BP(0,2,0) ), BV( BP(0,0,0), BP(2,6,0), BP(0,2,0) ) ),   // M3'
        Mask( BV( BP(7,7,7), BP(4,2,0), BP(0,2,0) ), BV( BP(4,0,0), BP(4,2,0), BP(0,2,0) ) ),   // M4'
        Mask( BV( BP(7,7,7), BP(0,2,7), BP(2,2,7) ), BV( BP(0,0,0), BP(0,2,0), BP(2,0,0) ) ),   // M5'
        Mask( BV( BP(7,7,7), BP(0,3,3), BP(2,7,3) ), BV( BP(0,0,0), BP(0,2,0), BP(2,4,0) ) ),   // M6'
        Mask( BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,1) ) ),   // M7'
    };
}
```

Compared to the [Palagyi and Kuba's templates](PalagyiKuba#Matching_Templates.md),
  * M1 is loosened to M1' to include the 6-adjacent end points.
  * M5 is loosened to M5' to include the 18-adjacent end points.
  * M7' is added for the 26-adjacent end points.
  * Other templates are same.

Here are the console dump of the above templates on the left,
and that of Palagyi and Kuba's on the right:

|![http://simple-voxels.googlecode.com/svn/wiki/images/raynal-templates.png](http://simple-voxels.googlecode.com/svn/wiki/images/raynal-templates.png)|![http://simple-voxels.googlecode.com/svn/wiki/images/palagyi-templates.png](http://simple-voxels.googlecode.com/svn/wiki/images/palagyi-templates.png)|
|:----------------------------------------------------------------------------------------------------------------------------------------------------|:------------------------------------------------------------------------------------------------------------------------------------------------------|

## Implementation ##
By enumerating all the symmetric configurations of each template within the NSWE plane (there are eight of them) and removing redundant ones,
the [list](http://code.google.com/p/simple-voxels/source/browse/trunk/Raynal_table.h)
of matching templates can be obtained for the U-direction as follows:
```
const Mask raynal_tables[][25] = {
    {// dir = 0
        // M1
        Mask( BV( BP(7,7,7), BP(0,2,0), BP(0,2,0) ), BV( BP(0,0,0), BP(0,2,0), BP(0,2,0) ) ),
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
        Mask( BV( BP(7,7,7), BP(0,2,7), BP(2,2,7) ), BV( BP(0,0,0), BP(0,2,0), BP(2,0,0) ) ),
        Mask( BV( BP(7,7,7), BP(1,3,1), BP(1,7,1) ), BV( BP(0,0,0), BP(0,2,0), BP(0,4,0) ) ),
        Mask( BV( BP(7,7,7), BP(7,2,0), BP(7,2,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,2) ) ),
        Mask( BV( BP(7,7,7), BP(4,6,4), BP(4,7,4) ), BV( BP(0,0,0), BP(0,2,0), BP(0,1,0) ) ),
        // M6
        Mask( BV( BP(7,7,7), BP(0,3,3), BP(2,7,3) ), BV( BP(0,0,0), BP(0,2,0), BP(2,4,0) ) ),
        Mask( BV( BP(7,7,7), BP(0,6,6), BP(2,7,6) ), BV( BP(0,0,0), BP(0,2,0), BP(2,1,0) ) ),
        Mask( BV( BP(7,7,7), BP(3,3,0), BP(3,7,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,4,2) ) ),
        Mask( BV( BP(7,7,7), BP(6,6,0), BP(6,7,2) ), BV( BP(0,0,0), BP(0,2,0), BP(0,1,2) ) ),
        // M7
        Mask( BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,1) ) ),
        Mask( BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ), BV( BP(0,0,0), BP(0,2,0), BP(0,0,4) ) ),
        Mask( BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ), BV( BP(0,0,0), BP(0,2,0), BP(1,0,0) ) ),
        Mask( BV( BP(7,7,7), BP(7,7,7), BP(7,7,7) ), BV( BP(0,0,0), BP(0,2,0), BP(4,0,0) ) ),
    },
//...
};
```

By using this list, the deletability of a voxel can be determined by
```
namespace NRaynal {
    bool IsDeletable( int bits, EFaceDir dir ) {
        for each (const Mask& mask in raynal_tables[(int)dir]) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
```
This function is defined in [Isthmus.cpp](http://code.google.com/p/simple-voxels/source/browse/trunk/Isthmus.cpp#29).

## Statistics ##
The next table shows how many simple configurations can be deleted by the above templates for each of the six directions:

![http://simple-voxels.googlecode.com/svn/wiki/images/raynal-stat.png](http://simple-voxels.googlecode.com/svn/wiki/images/raynal-stat.png)

where bit0 - bit5 denotes the following directions:
| **Bit** | **Direction** |
|:--------|:--------------|
| bit0    | U             |
| bit1    | D             |
| bit2    | N             |
| bit3    | S             |
| bit4    | W             |
| bit5    | E             |

Compared with [PalagyiKuba#Statistics](PalagyiKuba#Statistics.md),
  * For each direction, 2,124,292 - 2,124,283 = 9 configurations are further removed.
  * As a whole, 9,916,952 - 9,916,926 = 26 configurations are further removed.
These numbers clearly coincide with the number of end point configurations.

In the console dump below, the configurations that are deletable by Raynal and Couprie's but not by Palagyi and Kuba's are printed for the U-direction.
It is confirmed that they are all end points.

![http://simple-voxels.googlecode.com/svn/wiki/images/raynal-palagyi.png](http://simple-voxels.googlecode.com/svn/wiki/images/raynal-palagyi.png)

# Matching Templates for Isthmus Points #
Now, let's find the isthmus points next.

## Definition ##
Again, the isthmus points are defined in [[2](Top#References.md)] as the points that fullfil:
  * The number of 26-adjacent foreground components >= 2
According to [components count table](SimpleVoxels#Verification.md),
the number of voxels that satisfy this condition is as follows:

| **Components Count** | **Number** |
|:---------------------|:-----------|
| (2,1)                | 3,626,396  |
| (3,1)                | 651,784    |
| (4,1)                | 106,054    |
| (5,1)                | 13,512     |
| (6,1)                | 1,180      |
| (7,1)                | 56         |
| (8,1)                | 1          |
| (2,2)                | 904,832    |
| (3,2)                | 49,216     |
| (4,2)                | 2,688      |
| (5,2)                | 96         |
| (2,3)                | 63,744     |
| (3,3)                | 768        |
| (2,4)                | 1,024      |
| **total**            | **5,421,351** |

## Proposed Templates ##
In order to detect isthmus points, one can use a look-up truth table
for possible 2^26 = 67,108,864 = 64M configurations assuming that the center voxel is a foreground point.
If a true/false value for one configuration is represented by one bit,
it amounts to a memory space of 8MB.
This table size may be practical enough to be loaded onto a certain level of cached memory and randomly accessed on recent high performance computers.
Nevertheless, I was still curious about the matching templates for the isthmus points because it may lead to more efficient implementations.

In contrast to that the simple points have only one single foreground component, the isthmus points have multiple foreground components.
It is easily expected that finding matching templates separately for configurations for which the number of foreground components is 2, 3, ..., and 8 is not practical because the placement of foreground points could be highly scattered in the 3x3x3 neighborhood.

Instead, if there are multiple foreground components,
there should be a background _plane_ that completely separates the 3x3x3 neighborhood into two regions.
In addition, because the background points are connected by merely 6-adjacency while the foreground points are connected by 26-adjacency,
the background plane must be _firm_.

Having considered this way,
I could figure out the matching templates for the isthmus points.
The figure below illustrates the six matching templates,
where at least one of the blue points should be foreground
and similarly at least one of the orange points should be foreground.
Also, although the center voxel is actually a foreground isthmus point,
it is treated background (white) because it is excluded when counting the foreground components by definition.

![http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-masks.png](http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-masks.png)

The following is the base templates translated into [Mask](http://code.google.com/p/simple-voxels/source/browse/trunk/Mask.h) structure.
```
namespace NIsthmus {

    const Mask Templates[6] = {
        Mask( BV( BP(0,0,0), BP(7,5,7), BP(0,0,0) ), 0, BV( BP(7,7,7), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,0), BP(0,0,0), BP(7,7,7) ) ),
        Mask( BV( BP(0,7,0), BP(7,5,0), BP(0,0,0) ), 0, BV( BP(7,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,7), BP(0,0,7), BP(7,7,7) ) ),
        Mask( BV( BP(2,6,0), BP(6,4,0), BP(0,0,0) ), 0, BV( BP(4,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(1,1,7), BP(1,1,7), BP(7,7,7) ) ),
        Mask( BV( BP(2,6,0), BP(3,5,7), BP(0,0,0) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(7,7,7) ), BV( BP(1,1,7), BP(0,0,0), BP(0,0,0) ) ),
        Mask( BV( BP(2,6,0), BP(3,5,6), BP(0,3,2) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(7,4,4) ), BV( BP(1,1,7), BP(0,0,1), BP(0,0,1) ) ),
        Mask( BV( BP(2,6,0), BP(2,5,7), BP(2,3,0) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(4,4,7) ), BV( BP(1,1,7), BP(1,0,0), BP(1,0,0) ) ),
    };
}
```

Here is the console dump of the above templates:

![http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-templates.png](http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-templates.png)

## Implementation ##
By enumerating all the symmetric configurations of each template (there are 48 of them) and removing redundant ones,
the [list](http://code.google.com/p/simple-voxels/source/browse/trunk/Isthmus_table.h) of matching templates can be obtained as follows:

```
const Mask isthmus_table[63] = {
    // M1
    Mask( BV( BP(0,0,0), BP(7,5,7), BP(0,0,0) ), 0, BV( BP(7,7,7), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,0), BP(0,0,0), BP(7,7,7) ) ),
    Mask( BV( BP(0,7,0), BP(0,5,0), BP(0,7,0) ), 0, BV( BP(7,0,0), BP(7,0,0), BP(7,0,0) ), BV( BP(0,0,7), BP(0,0,7), BP(0,0,7) ) ),
    Mask( BV( BP(2,2,2), BP(2,0,2), BP(2,2,2) ), 0, BV( BP(1,1,1), BP(1,1,1), BP(1,1,1) ), BV( BP(4,4,4), BP(4,4,4), BP(4,4,4) ) ),
    // M2
    Mask( BV( BP(0,7,0), BP(7,5,0), BP(0,0,0) ), 0, BV( BP(7,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(0,0,7), BP(0,0,7), BP(7,7,7) ) ),
    Mask( BV( BP(2,2,2), BP(6,4,6), BP(0,0,0) ), 0, BV( BP(4,4,4), BP(0,0,0), BP(0,0,0) ), BV( BP(1,1,1), BP(1,1,1), BP(7,7,7) ) ),
    Mask( BV( BP(0,7,0), BP(0,5,7), BP(0,0,0) ), 0, BV( BP(0,0,7), BP(0,0,0), BP(0,0,0) ), BV( BP(7,0,0), BP(7,0,0), BP(7,7,7) ) ),
    Mask( BV( BP(2,2,2), BP(3,1,3), BP(0,0,0) ), 0, BV( BP(1,1,1), BP(0,0,0), BP(0,0,0) ), BV( BP(4,4,4), BP(4,4,4), BP(7,7,7) ) ),
    Mask( BV( BP(0,0,0), BP(0,5,7), BP(0,7,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,0,7) ), BV( BP(7,7,7), BP(7,0,0), BP(7,0,0) ) ),
    Mask( BV( BP(0,0,0), BP(6,4,6), BP(2,2,2) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(4,4,4) ), BV( BP(7,7,7), BP(1,1,1), BP(1,1,1) ) ),
    Mask( BV( BP(0,0,0), BP(7,5,0), BP(0,7,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(7,0,0) ), BV( BP(7,7,7), BP(0,0,7), BP(0,0,7) ) ),
    Mask( BV( BP(0,0,0), BP(3,1,3), BP(2,2,2) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(1,1,1) ), BV( BP(7,7,7), BP(4,4,4), BP(4,4,4) ) ),
    Mask( BV( BP(2,6,0), BP(2,4,0), BP(2,6,0) ), 0, BV( BP(4,0,0), BP(4,0,0), BP(4,0,0) ), BV( BP(1,1,7), BP(1,1,7), BP(1,1,7) ) ),
    Mask( BV( BP(2,3,0), BP(2,1,0), BP(2,3,0) ), 0, BV( BP(1,0,0), BP(1,0,0), BP(1,0,0) ), BV( BP(4,4,7), BP(4,4,7), BP(4,4,7) ) ),
    Mask( BV( BP(0,6,2), BP(0,4,2), BP(0,6,2) ), 0, BV( BP(0,0,4), BP(0,0,4), BP(0,0,4) ), BV( BP(7,1,1), BP(7,1,1), BP(7,1,1) ) ),
    Mask( BV( BP(0,3,2), BP(0,1,2), BP(0,3,2) ), 0, BV( BP(0,0,1), BP(0,0,1), BP(0,0,1) ), BV( BP(7,4,4), BP(7,4,4), BP(7,4,4) ) ),
    // M3
    Mask( BV( BP(2,6,0), BP(6,4,0), BP(0,0,0) ), 0, BV( BP(4,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(1,1,7), BP(1,1,7), BP(7,7,7) ) ),
    Mask( BV( BP(2,3,0), BP(3,1,0), BP(0,0,0) ), 0, BV( BP(1,0,0), BP(0,0,0), BP(0,0,0) ), BV( BP(4,4,7), BP(4,4,7), BP(7,7,7) ) ),
    Mask( BV( BP(0,6,2), BP(0,4,6), BP(0,0,0) ), 0, BV( BP(0,0,4), BP(0,0,0), BP(0,0,0) ), BV( BP(7,1,1), BP(7,1,1), BP(7,7,7) ) ),
    Mask( BV( BP(0,3,2), BP(0,1,3), BP(0,0,0) ), 0, BV( BP(0,0,1), BP(0,0,0), BP(0,0,0) ), BV( BP(7,4,4), BP(7,4,4), BP(7,7,7) ) ),
    Mask( BV( BP(0,0,0), BP(0,4,6), BP(0,6,2) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,0,4) ), BV( BP(7,7,7), BP(7,1,1), BP(7,1,1) ) ),
    Mask( BV( BP(0,0,0), BP(0,1,3), BP(0,3,2) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(0,0,1) ), BV( BP(7,7,7), BP(7,4,4), BP(7,4,4) ) ),
    Mask( BV( BP(0,0,0), BP(6,4,0), BP(2,6,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(4,0,0) ), BV( BP(7,7,7), BP(1,1,7), BP(1,1,7) ) ),
    Mask( BV( BP(0,0,0), BP(3,1,0), BP(2,3,0) ), 0, BV( BP(0,0,0), BP(0,0,0), BP(1,0,0) ), BV( BP(7,7,7), BP(4,4,7), BP(4,4,7) ) ),
//...
};
```

By using this list, the isthmus points can be found by
```
namespace NIsthmus {
    bool IsIsthmus( int bits ) {
        for each (const Mask& mask in isthmus_table) {
            if (mask.Match( bits )) {
                return true;
            }
        }
        return false;
    }
}
```
This function is defined in [Isthmus.cpp](http://code.google.com/p/simple-voxels/source/browse/trunk/Isthmus.cpp#56).

## Statistics ##
The next table shows how many isthmus configurations are found, where bit0 denotes if a configuration matches one of the templates or not.

Note that the number of points that are detected as isthmus is exactly the same with the number [above](RaynalCouprie#Definition.md).

![http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-stat.png](http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-stat.png)