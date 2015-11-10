# Summary #
This project is intended to help implement 3D parallel curve thinning algorithms by providing working C++ code that can be used to find deletable voxels according to various deletable conditions proposed in the literature.
In doing so, new implementations are also devised for some of the deletable conditions.

The current scope of this project is the 3D parallel thinning algorithms that are characterized by:
  * (26, 6) foreground and background connectivity
  * Curve thinning
  * Parallel thinning with 6 sub-iterations
  * Deletability of a voxel can be determined locally within its 3x3x3 neighborhood

This Wiki content is also a footprint of my trial seeking for a way to reduce spurious branches in thinned objects, and from some point, to remove skeletal branches, too, to have ultimately thinned objects.

My current choice is the combination of:
  * Nemeth, Kardos, and Palagyi's object surface smoothing, followed by
  * Raynal and Couprie's isthmus preservation, and then
  * Lohou and Bertrand's P2x-simple condition

However, it is out of the scope to provide an implementation of whole thinning algorithms using the deletable conditions picked up in this project.
So, there is no fancy screenshots of how objects are thinned by those deletable conditions as in papers regarding thinning algorithms. Sorry for the boringness. :-)

# References #
  * [[1](http://www.sciencedirect.com/science/article/pii/S0167865598000312)]. Palagyi, Kuba, A 3D 6-subiteration thinning algorithm for extracting medial lines, 1998.
  * [[2](http://link.springer.com/chapter/10.1007/978-3-642-19867-0_15)]. Raynal, Couprie, Isthmus-Based 6-Directional Parallel Thinning Algorithms, 2011.
  * [[3](http://www.sciencedirect.com/science/article/pii/S0166218X05001319)]. Lohou, Bertrand, A 3D 6-subiteration curve thinning algorithm based on P-simple points, 2005.
  * [[4](http://www.sciencedirect.com/science/article/pii/S1524070311000063)]. Nemeth, Kardos, Palagyi, Thinning combined with iteration-by-iteration smoothing for 3D binary images, 2011.

Thanks!