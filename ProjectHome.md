This project is intended to help implement 3D parallel curve thinning algorithms in the literature by providing working C++ codes that can be used to determine whether a voxel is deletable or not. In doing so, new template masks or new implementation were devised for some of the algorithms.

The current scope of this project is 3D parallel thinning algorithms that are characterized by:
  * (26, 6) foreground and background connectivity
  * Curve thinning
  * Parallel thinning with 6 subiterations
  * Deletability of a voxel can be determined locally with its 3x3x3 neighborhood

Platform: Visual Studio 2012

![http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-masks.png](http://simple-voxels.googlecode.com/svn/wiki/images/isthmus-masks.png)