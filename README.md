OpenGL Annotations Toolkit
====

The GLAT project aims to offer a lightweight and easy to use toolkit to render high-definition and -quality annotations in 3D scenes. 


### Documentation


### Dependencies

The following dev-libraries and programs need to be provided for correct CMake configuration:
* C++11 compatible compiler (e.g. gcc 4.8, better 4.9, VS 2013)
* OpenGL 4.3 compatible graphics card and driver
* OpenGL Objects Wrapper Library (GLOW): http://libglow.org/
* FreeType: http://www.freetype.org

If distance field maps have to be generated (e.g. NV_Path_Rendering n/a) additional dependencies are needed:
* ShivaVG: https://github.com/ileben/ShivaVG
* svg lib
* pdf lib

For best performance and quality the OpenGL Extension "NV_Path_Rendering" (released with NVIDIA v275 drivers) should be present.

### License
This software contains source code provided by NVIDIA Corporation: https://developer.nvidia.com/nv-path-rendering 