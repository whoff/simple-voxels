# Simple Voxels #
When the removal of a foreground voxel does not change the topology of a 3D shape, the voxel is called simple.
For example:
  * Sphere of radius 100 voxels: its surface voxels are all simple because their removal makes only a tiny dent on the surface and is never earth-shattering as to change the topology.
  * 1-voxel wide line: its two end voxels are simple, but the internal voxels are not because their removal divides the line into two shorter lines.
  * 1-voxel thick square plane: its border voxels are simple, but the internal voxels are not because their removal makes a hole in the plane.

# Definition #
The simplicity of a voxel can be defined locally within its 3x3x3 neighborhood
[[1-3](http://code.google.com/p/simple-voxels/wiki/Top#References)].
For the (26, 6) connectivity, a voxel is simple if the following two conditions are fulfilled:
  * The number of 26-adjacent foreground components in its 26-neighborhood is 1.
  * The number of 6-adjacent background components in its 18-neighborhood that appear in the 6-neighborhood is 1.
Note that, in these conditions, the center voxel to be tested is excluded from and does not belong to any foreground / background components.

## Implementation ##
The following functions defined in [Neighbors.cpp](http://code.google.com/p/simple-voxels/source/browse/trunk/Neighbors.cpp#82) calculates the number of foreground / background components given a 3x3x3 configuration as a 32-bit integer _bits_:
```
namespace NComponent {
    int CountForeCompsN26( int bits );
    int CountBackCompsN6( int bits );
}
```
These functions return zero when the center voxel is background (zero).

## Verification ##
The above functions were called with all the possible 2^27 = 134,217,728 configurations.
Then, the frequencies of each pair of the number of foreground and background components were counted:

![http://simple-voxels.googlecode.com/svn/wiki/images/cout-components-count.png](http://simple-voxels.googlecode.com/svn/wiki/images/cout-components-count.png)

Because both of the functions return zero when the center voxel is background, the first (0,0) count is equal to the number of configurations where the center voxel is background (zero).
In fact, if the center voxel is foreground, at least one of the functions returns non-zero value. The (0, 0) count therefore is equal to 2^26 = 67,108,864.

The implementation is verified by the fact that this output reproduces the same component counts exactly as in
[3D binary topological numbers](http://www-sop.inria.fr/members/Gregoire.Malandain/topology/tables.html#BINARY3).

## Simple Configurations ##
In the above table, the (1, 1) pair corresponds to the simple voxels
because removal / addition of the center point will not change the number of the foreground / background components.
Hence, there are 25,985,144 configurations out of 67,108,864, where the center (foreground) voxel is simple.

Please refer to [3D binary topological numbers](http://www-sop.inria.fr/members/Gregoire.Malandain/topology/tables.html#BINARY3) for the interpretation of other count pairs.