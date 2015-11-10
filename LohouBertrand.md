**Contents**


# Introduction #
By using [Raynal and Couprie](RaynalCouprie.md)'s method,
a considerable amount of spurious branches can be suppressed.
In addition, as is suggested in [[2](Top#References.md)],
if the [isthmus points](RaynalCouprie#Isthmus_Points.md) are not preserved but deleted,
the skeletal branches will also be removed resulting in _ultimate thinning_.
An ultimately thinned object has no branches but instead is composed of circular curves or cocoon like shells.

However, as far as I tested, there still remained some branches even when no isthmus points were preserved.
Some of these remaining branches ended with a specific configuration depicted in the figure below.

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-not-removed.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-not-removed.png)

It turned out that this configuration is the one that Lohou and Bertrand illustrated in [[3](Top#References.md)]
as an example configuration that is not deleted by [Palagyi and Kuba](PalagyiKuba.md)'s method whereas can be removed by their method introduced and implemented here in this page.

They derived in [[3](Top#References.md)] a deletable condition that is capable of deleting more points based on a powerful notion called _Px-simple_,
which allows one to devise a new parallel thinning algorithm without verifying
that the set of matching templates does not break the objct topology when applied in parallel.

In the rest of this page, after their deletable condition is briefly explained,
an implementation will be proposed using both direct bit operations and supplemental template matching.

# Deletable Condition based on P2x-simple Points #
Throughout the following description, the U-direction is assumed for the sake of explanation.
For other directions, the conditions will immediately follow
by rotating the 3x3x3 neighborhood configuration to the desired direction.

## P2-simple ##
A foreground point x is called P2-simple if:
  1. Its U-neighbor is background.
  1. If any of its N/S/W/E-neighbor is background, then the neighbor's U-neighbor is also background.

In the next figure, the black point denotes the foreground point x.
The first condition requires the point above x be background.
The second condition says that if a yellow position is background,
then its U-neighbor (the gray position above it) should also be background.

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-p2simple.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-p2simple.png)

## P2x-simple ##
Given a center point x, its 26-neighborhood points are called P2x-simple
if it is not denied that they are P2-simple within the 3x3x3 neighborhood of x.
In other words, a 26-neighbor point of x is not P2x-simple if it disqualifies the P2-simple condition even in the 3x3x3 neighborhood of x.

Based on the P2x-simple condition, foreground points in the 3x3x3 neighborhood of x are divided into the following two sets.
  * Px : the set of foreground points that are P2x-simple
  * Rx : the set of foreground points that are not P2x-simple

Note that any foreground points in the top plane are always P2x-simple because
the points above them and above their N/S/W/E-neighbors are all outside the 3x3x3 neighborhood of x,
the P2-simple condition for these points cannot be denied.

## Deletable Condition ##
Now, a foreground point x is deletable if the following conditions are fulfilled.
  * (P2) x is P2-simple
  * (C1) The number of 26-connected components in Rx is 1.
  * (C2) The number of 6-connected background components within the 18-neighborhood of x is 1.
  * (C3) For every point y in Px, there exists z in Rx such that z is 26-adjacent to both x and y.
  * (C4) For every point y in Px that is 6-adjacent to x, there exist background points z and t such that {x, y, z, t} forms a unit square.

As is shown in the next section, with this deletable condition,
6,129,532 points are deletable for one direction
whereas 2,124,292 points are deletable
with the [Palagyi and Kuba](PalagyiKuba.md)'s condition.
When all the six directions are considered,
23,722,000 points are deletable whereas 9,196,926 points are
deletable with [Palagyi and Kuba](PalagyiKuba.md)'s condition.

# Proposed Method #
In [[3](Top#References.md)], the authors proposed the use of two binary decision diagrams (BDD) to find deletable points.
I was again interested in devising a set of matching templates,
but failed to find one that implements the deletable condition as a whole.
So, I started looking at each condition P2, C1/2/3/4 one by one.
To begin with, the number of configurations that satisfy each condition was counted.

![http://simple-voxels.googlecode.com/svn/wiki/images/pxcondition-stat.png](http://simple-voxels.googlecode.com/svn/wiki/images/pxcondition-stat.png)

where bit0 - bit5 denotes the following conditions:
| **Bit** | **Condition** |
|:--------|:--------------|
| bit0    | Deletable     |
| bit1    | P2            |
| bit2    | C1            |
| bit3    | C2            |
| bit4    | C3            |
| bit5    | C4            |

## C4 : included by P2 ##
From the co-occurrence table above,
it can be seen that the C4 condition is always fulfilled if P2 is fulfilled.
The C4 condition can therefore be omitted.

## P2 : x is P2-simple ##
From the definition of P2-simple points,
the P2 condition can be easily implemented by using matching templates or bit operations.
Once the P2 condition is checked by any means,
the configurations that fail the P2 condition can be treated as _don't-care_
in the subsequent evaluation of C1, C2, and C3 conditions.

The existence of such don't-care configurations may make it easier to find matching templates for the remaining conditions.
However, even with this technique, the matching templates for C1 and C3 were still hard to find probably because they were talking about Px and Rx situations whereas the matching templates were defined with respect to the 3x3x3 neighborhood of x.

That was when I decided to calculate Px and Rx directly by using bit operations, and to find matching templates with respect to Px or Rx instead of the x's neighborhood.
In addition, the P2 condition is computed as part of the calculation of P2x-simple points because x is P2-simple only when it is P2x-simple by the definition.

### P2x-simple Points ###
Given a configuration as an 32-bit integer _bits_, the following code computes the Px points.
Please see [Notations](Notations.md) for the bit assignment to the 3x3x3 positions in _bits_.
```
enum {
    DX = 1,
    DY = 3,
    DZ = 9,
};

const int MaskW = BV( BP(3,3,3), BP(3,3,3), BP(3,3,3) );    // West
const int MaskE = BV( BP(6,6,6), BP(6,6,6), BP(6,6,6) );    // East
const int MaskN = BV( BP(7,7,0), BP(7,7,0), BP(7,7,0) );    // North
const int MaskS = BV( BP(0,7,7), BP(0,7,7), BP(0,7,7) );    // South

int CalcP2x( int bits ) {
    const int top = bits << DZ;
    const int mid = top & ~bits;
    const int rx = top | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX) | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY);
    return bits & ~rx;
}
```

In the above, the Rx points are computed first.
By negating the P2x-simple condition, a foreground point belongs to Rx when:
  1. Its U-neighbor is foreground.
  1. Or, any of its N/S/W/E-neighbors is background and the neighbor's U-neighbor is foreground.
Accordingly, the above code works as follows:
  * _top_ holds the value of the U-neighbor of each position by shifting _bits_ by DZ = 9 bits, i.e. by one plane.
  * _mid_ has '1' bits where the corresponding points are background (~bits) and (&) their U-neighbor is foreground (top).
  * _rx_ then has '1' bits if the U-neighbor of the corresponding bits is foreground (top) or (|) any of their N/S/W/E-neighbors are background and its U-neighbor is foreground (shifted mid's).
  * Finally, Px is obtained by negating rx (bits & ~rx).

The P2 condition can be checked by looking at the center bit of Px.

## C3 : Px points are 26-adjacent to Rx ##
The C3 condition says that, for any P2x-simple point other than x,
there should be found a Rx point that is 26-adjacent to it.
This condition may be implemented more simply by using bit operations than matching templates as follows:
  1. Compute Rn = one 26-neighbor inflate of Rx
  1. If Rn has a '0' bit at any position where Px has '1' bit , the C3 condition fails.

The code below implements the above procedure.
Note that the center point x is excluded from the evaluation.
```
enum {
    N0_bits  = BV( BP(0,0,0), BP(0,2,0), BP(0,0,0) ),
};
//...
    int rn = rx;    // N26( rx )
    rn = rn | ((rn & MaskE) >> DX) | ((rn & MaskW) << DX);
    rn = rn | ((rn & MaskS) >> DY) | ((rn & MaskN) << DY);
    rn = rn | (rn >> DZ) | (rn << DZ);
    if (~rn & px & ~N0_bits) return false;
//...
```

## C2: exactly one background component ##
With the P2 condition fulfilled, namely if the center point x is P2-simple,
the C2 condition empirically turned out to be reduced to the following two matching templates.
Note that because x is P2-simple, its U-neighbor is known to be background for both templates.
Also, if its N/S/W/E-neighbor is backgroud, the neighbor's U-neighbor is background, too.
Both templates rely on this fact.

![http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-t6nx-masks.png](http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-t6nx-masks.png)

### Case 1: bottom center is foreground (Left) ###
When the bottom center point is foreground, the C2 condition will be always fulfilled as long as x is P2-simple.
In fact, if any N/S/W/E-neighbor point of x is background,
then its U-neighbor is also background since x is P2-simple,
and thus it is 6-connected to the top center background point via its U-neighbor.
This ensures that there is exactly one background component.

### Case 2: bottom center is background (Right) ###
On the other hand, when the bottom center point is background,
it should be connected with the top center background point to have the C2 condition fulfilled.
Since the background component needs to be 6-connected,
there should be a column of background points in any of N/S/W/E-neighborhood of x.
Considering again that x is P2-simple, the right template is enough to ensure
that there is exactly one background component in this case.

## C1 : exactly one 26-connected Rx component ##
As is pointed out in [P2x-simple](LohouBertrand#P2x-simple.md) section,
any foreground point in the top plane is P2x-simple.
The Rx points can accordingly exist in the middle or in the bottom plane only.

### Case 1: bottom center is foreground ###
If the bottom center is a foreground point, the C1 condition is always fulfilled.
Because the bottom center is not P2x-simple, it is a Rx point.
If there are other Rx points, since they are in the middle or in the bottom plane,
they are 26-adjacent to the bottom center Rx point.

### Case 2: bottom center is background ###
In this case, I could figure out "mismatching" templates as below where Rx points are divided into at least two components.

![http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-t26rx-masks.png](http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-t26rx-masks.png)

In the figures above, at least one blue point should be foreground, and similarly,
at least one orange point should be foreground, too.
Also, as the center point is excluded from Rx components,
it is depicted as a background point although it is actually a P2(x)-simple foreground point for the sake of explanation.

The C1 condition will be fulfilled if the given configuration has at least one Rx point and if it does not match any of the above templates,

The following is the mismatching templates translated into
[Mask](http://code.google.com/p/simple-voxels/source/browse/trunk/Mask.h) structure
and their console dump.
```
namespace NPxSimple {// P2x-simple
    const Mask TemplatesT26Rx[2] = {
        Mask( BV( BP(0,0,0), BP(2,0,2), BP(2,2,2) ), 0, BV( BP(0,0,0), BP(1,1,1), BP(1,1,1) ), BV( BP(0,0,0), BP(4,4,4), BP(4,4,4) ) ),
        Mask( BV( BP(0,0,0), BP(2,1,0), BP(2,3,0) ), 0, BV( BP(0,0,0), BP(1,0,0), BP(1,0,0) ), BV( BP(0,0,0), BP(4,4,7), BP(4,4,7) ) ),
    };
}
```

![http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-t26rx-templates.png](http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-t26rx-templates.png)

## Implementation ##
Now, the next code gives an implementation of the above described method that works for the specified direction out of the six directions.
```
const Mask pxsimple_T26Rx_tables[][6] = {
    {// dir = 0
        // M1
        Mask( BV( BP(0,0,0), BP(2,0,2), BP(2,2,2) ), 0, BV( BP(0,0,0), BP(1,1,1), BP(1,1,1) ), BV( BP(0,0,0), BP(4,4,4), BP(4,4,4) ) ),
        Mask( BV( BP(0,0,0), BP(0,5,0), BP(0,7,0) ), 0, BV( BP(0,0,0), BP(7,0,0), BP(7,0,0) ), BV( BP(0,0,0), BP(0,0,7), BP(0,0,7) ) ),
        // M2
        Mask( BV( BP(0,0,0), BP(2,1,0), BP(2,3,0) ), 0, BV( BP(0,0,0), BP(1,0,0), BP(1,0,0) ), BV( BP(0,0,0), BP(4,4,7), BP(4,4,7) ) ),
        Mask( BV( BP(0,0,0), BP(2,4,0), BP(2,6,0) ), 0, BV( BP(0,0,0), BP(4,0,0), BP(4,0,0) ), BV( BP(0,0,0), BP(1,1,7), BP(1,1,7) ) ),
        Mask( BV( BP(0,0,0), BP(0,4,2), BP(0,6,2) ), 0, BV( BP(0,0,0), BP(0,0,4), BP(0,0,4) ), BV( BP(0,0,0), BP(7,1,1), BP(7,1,1) ) ),
        Mask( BV( BP(0,0,0), BP(0,1,2), BP(0,3,2) ), 0, BV( BP(0,0,0), BP(0,0,1), BP(0,0,1) ), BV( BP(0,0,0), BP(7,4,4), BP(7,4,4) ) ),
    },
//...
};

bool IsDeletable( int bits, EFaceDir dir ) {
    int top, btm;
    int neib;
    int maskT6N4;
    if (dir == ED_U || dir == ED_D) {
        top = (dir == ED_U) ? (bits << DZ) : (bits >> DZ);
        btm = (dir == ED_U) ? (bits >> DZ) : (bits << DZ);
        const int mid = top & ~bits;
        neib = top | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX) | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY);
        maskT6N4 = BV( BP(0,0,0), BP(2,5,2), BP(0,0,0) );
    } else if (dir == ED_N || dir == ED_S) {
        top = (dir == ED_N) ? ((bits & MaskN) << DY) : ((bits & MaskS) >> DY);
        btm = (dir == ED_N) ? ((bits & MaskS) >> DY) : ((bits & MaskN) << DY);
        const int mid = top & ~bits;
        neib = top | (mid >> DZ) | (mid << DZ) | ((mid & MaskE) >> DX) | ((mid & MaskW) << DX);
        maskT6N4 = BV( BP(0,2,0), BP(0,5,0), BP(0,2,0) );
    } else if (dir == ED_W || dir == ED_E) {
        top = (dir == ED_W) ? ((bits & MaskW) << DX) : ((bits & MaskE) >> DX);
        btm = (dir == ED_W) ? ((bits & MaskE) >> DX) : ((bits & MaskW) << DX);
        const int mid = top & ~bits;
        neib = top | ((mid & MaskS) >> DY) | ((mid & MaskN) << DY) | (mid >> DZ) | (mid << DZ);
        maskT6N4 = BV( BP(0,2,0), BP(2,0,2), BP(0,2,0) );
    } else {
        return false;
    }
    const int rx = bits & neib;
    if (rx == 0) return false;              // C1
    const int px = bits & ~rx;
    if ((px & N0_bits) == 0) return false;  // x is P2?
    {// C3
        int rn = rx;    // N26(Rx)
        rn = rn | ((rn & MaskE) >> DX) | ((rn & MaskW) << DX);
        rn = rn | ((rn & MaskS) >> DY) | ((rn & MaskN) << DY);
        rn = rn | (rn >> DZ) | (rn << DZ);
        if (~rn & px & ~N0_bits) return false;
    }
    if ((btm & N0_bits) == 0) {
        {// C2
            const int n4 = ~(bits | btm);
            if ((n4 & maskT6N4) == 0) return false;
        }
        {// C1
            for each (const Mask& mask in pxsimple_T26Rx_tables[dir]) {
                if (mask.Match( rx )) return false;
            }
        }
    }
    return true;
}
```

# Statistics #
The next table shows how many simple configurations can be deleted by the P2x-simple condition for each of the six directions:

![http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-stat.png](http://simple-voxels.googlecode.com/svn/wiki/images/pxsimple-stat.png)

where bit0 - bit5 denotes the following directions:
| **Bit** | **Direction** |
|:--------|:--------------|
| bit0    | U             |
| bit1    | D             |
| bit2    | N             |
| bit3    | S             |
| bit4    | W             |
| bit5    | E             |

It can be observed that
  * As a whole, 23,722,000 out of 25,985,144 [simple](SimpleVoxels#Simple_Configurations.md) configurations are deletable.
  * For each direction, 6,129,532 configurations are deletable.
  * Between the opposite directions such as U and D, 641,232 configurations are deletable in common. (Likewise, between N and S or between W and E.)

It should be noted that, in the original paper [[3](Top#References.md)],
curve ending points are not removed in the thinning process
to preserve the object skeletons.
Even when a center point is deletable,
if there is only one foreground point in its 26-neighorhood, it will not be deleted.
Therefore, the number of deletable configurations above is larger by 18 than that appeared in the paper: 23,721,982.

## Comparison ##
Compared with [Palagyi and Kuba](PalagyiKuba.md)'s method and [Raynal and Couprie](RaynalCouprie.md)'s method,
the above deletable condition allows for the removal of more than twice number of simple voxels.
However, there are several configurations that are deletable by [Raynal and Couprie](RaynalCouprie.md)'s method, but not by this method.
The following is the console dump of those points found by the program for the U-direction:

![http://simple-voxels.googlecode.com/svn/wiki/images/raynal-pxsimple.png](http://simple-voxels.googlecode.com/svn/wiki/images/raynal-pxsimple.png)

As it can be seen, these points are diagonal end points that match the following template.
Because the foreground point in the bottom plane is P2x-simple,
there is no Rx point and thus the C1 condition fails with this configuration.

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-diag-end.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-diag-end.png)

# Proposal for Ultimate Thinning #
If one wants to use this method for the [ultimate thinning](LohouBertrand#Introduction.md) purpose, the diagonal end points need to be deletable.

Suppose that these diagonal end points are added to the deletable configurations for the U-direction.
Then, if the foreground point in the bottom plane is not deleted during the U-direction sub-iteration,
the extended deletable condition can still be used for parallel thinning.

When the above configuration is viewed from the foreground point in the bottom plane, named y here, it looks like the next one:

![http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-diag-end-opp.png](http://simple-voxels.googlecode.com/svn/wiki/images/neighbor-diag-end-opp.png)

The point y is now placed at the center,
and the original center point x is on the (opposite) top diagonal corner.

In this configuration, because the point x is P2y-simple and
because there is no Ry point in the 26-neighborhood of y, the C1 condition does not hold.

Therefore the diagonal end points can be deleted safely in the same parallel sub-iteration for the U-direction.