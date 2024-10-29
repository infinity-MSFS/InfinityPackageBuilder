cmake_minimum_required(VERSION 3.28)
project(InfinityPackageBuilder)

set(CMAKE_CXX_STANDARD 20)

add_executable(InfinityPackageBuilder main.cpp
        Dependencies/HDiff/include/HDiff/diff_types.h
        Dependencies/HDiff/include/HDiff/hdiffpatch.h
        Dependencies/HDiff/include/HDiff/patch_types.h)
