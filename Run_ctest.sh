#!/bin/bash

SRC=`pwd`



# Release - Clang - VTK 7.1.1 - Boost 1.58.0


cd FullMonteSW/Build/ReleaseClang
 

ctest -R Test_AffineTransform
ctest -R Test_Units
ctest -R Test_Layered_Basic
ctest -R Test_Queries
ctest -R Test_TIMOSReaderWriter
ctest -R Test_MCML
ctest -R Test_COMSOLMeshReaderWriter
ctest -R Test_TetraEmitterFactory
ctest -R Test_MCMLKernel_Sample
ctest -R Test_InternalKernel
ctest -R Test_Mouse
ctest -R Test_BlockRandomDistribution
ctest -R Test_RNG
ctest -R DVH
ctest -R DSH
ctest -R Test_tclShell
ctest -R Test_FullMonte_MMC_Comparison
ctest -R Test_FullMonte_Self
ctest -R Test_FullMonte_Approx
ctest -R Test_FullMonte_Composite


# Debug - Clang - VTK 7.1.1 - Boost 1.58.0
cd $SRC

cd FullMonteSW/Build/DebugClang

ctest


# RelWithDebInfo - Clang - VTK 7.1.1 - Boost 1.58.0
cd $SRC

cd FullMonteSW/Build/RelWithDebInfoClang

ctest



# Release - GCC 4.9 - VTK 6.3 - Boost 1.60.0
cd $SRC

 cd FullMonteSW/Build/ReleaseGCC

ctest






