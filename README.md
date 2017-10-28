# Z buffer visibility algorithm

Add the Z buffer visibility algorithm to your triangle scan
conversion algorithm in Assignment 2. Define a scene made up of colored triangles with 3D vertex coordinates.
Assume orthogonal projection, so that the x and y coordinates correspond to the pixel coordinates in the image
raster, and the z coordinate corresponds to the depth. Now when triangles overlap, the front most one will be
visible, rather than the one that is scan converted last. Make sure that each triangle fits completely inside the
screen window, so that no clipping is necessary. Render the triangles into a software color buffer and z buffer,
and copy the color buffer to the screen when all the triangles are finished, or else write the colored pixels to the
graphics hardware color buffer one by one when the software depth buffer test indicates that their color should
change. (The pixel by pixel color writing may be useful for debugging because it shows the progress of the
rendering, but may be very slow, so make sure that your test image takes less than a minute to generate if this is
your only or default option.)
