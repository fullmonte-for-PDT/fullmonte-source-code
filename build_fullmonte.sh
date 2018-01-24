#!/bin/bash
#module load cmake
SRC=`pwd`

#### Release - Clang - VTK 7.1.1 - Boost 1.58.0

mkdir -p FullMonteSW/Build/ReleaseClang

cd FullMonteSW/Build/ReleaseClang

cmake -DCMAKE_BUILD_TYPE=Release\
    -DARCH=AVX2 \
    -DCMAKE_MODULE_PATH=$SRC/cmake \
    -DWRAP_TCL=ON\
    -DWRAP_VTK=ON\
    -DVTK_DIR=/tools/vtk/install/7.1.1/lib/cmake/vtk-7.1\
    -DBOOST_ROOT=/home/fullmontebot/ANTLR-BOOST/boost58/install\
    ../.. && make -j8

#exit

#### Debug - Clang - VTK 7.1.1 - Boost 1.58.0

cd $SRC
mkdir -p FullMonteSW/Build/DebugClang

cd FullMonteSW/Build/DebugClang

cmake -DCMAKE_BUILD_TYPE=Debug\
    -DARCH=AVX2 \
    -DCMAKE_MODULE_PATH=$SRC/cmake \
    -DWRAP_TCL=ON\
    -DWRAP_VTK=ON\
    -DVTK_DIR=/tools/vtk/install/7.1.1/lib/cmake/vtk-7.1\
    -DBOOST_ROOT=/home/fullmontebot/ANTLR-BOOST/boost/install\
    ../.. && make -j8



#### RelWithDebInfo - Clang - VTK 7.1.1 - Boost 1.58.0

cd $SRC
mkdir -p FullMonteSW/Build/RelWithDebInfoClang

cd FullMonteSW/Build/RelWithDebInfoClang

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo\
    -DARCH=AVX2 \
    -DCMAKE_MODULE_PATH=$SRC/cmake \
    -DWRAP_TCL=ON\
    -DWRAP_VTK=ON\
    -DVTK_DIR=/tools/vtk/install/7.1.1/lib/cmake/vtk-7.1\
    -DBOOST_ROOT=/home/fullmontebot/ANTLR-BOOST/boost/install\
    ../.. && make -j8



#### Release - GCC 4.9 - VTK 7.1.1 - Boost 1.60.0

cd $SRC

mkdir -p FullMonteSW/Build/ReleaseGCC
cd FullMonteSW/Build/ReleaseGCC

cmake -DCMAKE_BUILD_TYPE=Release\
    -DARCH=AVX2 \
    -DCMAKE_MODULE_PATH=$SRC/cmake \
    -DWRAP_TCL=ON\
    -DWRAP_VTK=ON\
    -DVTK_DIR=/tools/vtk/install/7.1.1/lib/cmake/vtk-7.1\
    -DCMAKE_CXX_FLAGS=-Wa,-q \
    -DCMAKE_C_FLAGS=-Wa,-q \
    -DBOOST_ROOT=/home/fullmontebot/ANTLR-BOOST/boost/install\
    -DCMAKE_CXX_COMPILER=/usr/bin/g++-5 \
    -DCMAKE_C_COMPILER=/usr/bin/gcc-5 \
    ../.. && make -j8
