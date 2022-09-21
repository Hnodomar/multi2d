# multi2d

A cross-platform multiplayer 2D isometric voxel game.

![alt text](https://github.com/Hnodomar/multi2d/blob/master/extern/img/img1.png?raw=true)

### Building

The build pattern is typical for CMake projects, i.e. from the project root-dir:
```
mkdir build
cd build
cmake ..
```

By default this will build for unix-based systems. To build for Windows, add the flag `-DCMAKE_TOOLCHAIN_FILE=../tc-mingw.cmake`. This requires certain mingw-compilers.
