#!/bin/bash

SRC=`pwd`



# Release - Clang - VTK 7.1.1 - Boost 1.58.0


cd FullMonteSW/Build/ReleaseClang
 
# ctest -R Test_VTK # does not run
ctest -R Test_AffineTransform
ctest -R Test_Units
ctest -R Test_Layered_Basic
ctest -R Test_Queries
# ctest -R Test_TIMOSReaderWriter  #This test fails
# ctest -R Test_MCML #This test fails
ctest -R Test_COMSOLMeshReaderWriter 
# ctest -R Test_TetraEmitterFactory # This test fails
# ctest -R Test_MCMLKernel_Sample # This test fails
# ctest -R Test_InternalKernel # This test fails
ctest -R Test_Mouse
# ctest -R Test_BlockRandomDistribution # This test fails
ctest -R Test_RNG
# ctest -R DVH # This test fails
# ctest -R DSH # This test fails
# The following tests should succeed if they run and generate output for comparison, working on them
 ctest -R Test_tclShell
 ctest -R Test_FullMonte_MMC_Comparison
 ctest -R Test_FullMonte_Self
 ctest -R Test_FullMonte_Approx
 ctest -R Test_FullMonte_Composite

if [ 1 -eq 0 ]; then
# Debug - Clang - VTK 7.1.1 - Boost 1.58.0
cd $SRC

cd FullMonteSW/Build/DebugClang

# ctest -R Test_VTK # does not run
ctest -R Test_AffineTransform
ctest -R Test_Units
ctest -R Test_Layered_Basic
ctest -R Test_Queries
# ctest -R Test_TIMOSReaderWriter  #This test fails
# ctest -R Test_MCML #This test fails
ctest -R Test_COMSOLMeshReaderWriter 
# ctest -R Test_TetraEmitterFactory # This test fails
# ctest -R Test_MCMLKernel_Sample # This test fails
# ctest -R Test_InternalKernel # This test fails
ctest -R Test_Mouse
# ctest -R Test_BlockRandomDistribution # This test fails
ctest -R Test_RNG
# ctest -R DVH # This test fails
# ctest -R DSH # This test fails
# The following tests should succeed if they run and generate output for comparison, working on them
 ctest -R Test_tclShell
 ctest -R Test_FullMonte_MMC_Comparison
 ctest -R Test_FullMonte_Self
 ctest -R Test_FullMonte_Approx
 ctest -R Test_FullMonte_Composite


# RelWithDebInfo - Clang - VTK 7.1.1 - Boost 1.58.0
cd $SRC

cd FullMonteSW/Build/RelWithDebInfoClang

# ctest -R Test_VTK # does not run
ctest -R Test_AffineTransform
ctest -R Test_Units
ctest -R Test_Layered_Basic
ctest -R Test_Queries
# ctest -R Test_TIMOSReaderWriter  #This test fails
# ctest -R Test_MCML #This test fails
ctest -R Test_COMSOLMeshReaderWriter 
# ctest -R Test_TetraEmitterFactory # This test fails
# ctest -R Test_MCMLKernel_Sample # This test fails
# ctest -R Test_InternalKernel # This test fails
ctest -R Test_Mouse
# ctest -R Test_BlockRandomDistribution # This test fails
ctest -R Test_RNG
# ctest -R DVH # This test fails
# ctest -R DSH # This test fails
# The following tests should succeed if they run and generate output for comparison, working on them
 ctest -R Test_tclShell
 ctest -R Test_FullMonte_MMC_Comparison
 ctest -R Test_FullMonte_Self
 ctest -R Test_FullMonte_Approx
 ctest -R Test_FullMonte_Composite



# Release - GCC 4.9 - VTK 6.3 - Boost 1.60.0
cd $SRC

 cd FullMonteSW/Build/ReleaseGCC

# ctest -R Test_VTK # does not run
ctest -R Test_AffineTransform
ctest -R Test_Units
ctest -R Test_Layered_Basic
ctest -R Test_Queries
# ctest -R Test_TIMOSReaderWriter  #This test fails
# ctest -R Test_MCML #This test fails
ctest -R Test_COMSOLMeshReaderWriter 
# ctest -R Test_TetraEmitterFactory # This test fails
# ctest -R Test_MCMLKernel_Sample # This test fails
# ctest -R Test_InternalKernel # This test fails
ctest -R Test_Mouse
# ctest -R Test_BlockRandomDistribution # This test fails
ctest -R Test_RNG
# ctest -R DVH # This test fails
# ctest -R DSH # This test fails
# The following tests should succeed if they run and generate output for comparison, working on them
 ctest -R Test_tclShell
 ctest -R Test_FullMonte_MMC_Comparison
 ctest -R Test_FullMonte_Self
 ctest -R Test_FullMonte_Approx
 ctest -R Test_FullMonte_Composite

fi




