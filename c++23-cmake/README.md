SDL3, C++23 and CMake
=====================

Requirements
------------

* C++ compiler that supports C++23, such as a recent version of GCC (`g++`) or Clang (`clang++`).
* CMake
* Make (or `gmake` on BSD).
* SDL3

One way of building with C++23, CMake, Make and SDL3
----------------------------------------------------

    cmake -B build -D CMAKE_BUILD_TYPE=Release -S .
    make -C build

Running
-------

    build/main

Cleaning up the binary file and build directory
-----------------------------------------------

    rm -rf build
