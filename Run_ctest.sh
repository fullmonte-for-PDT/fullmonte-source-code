#!/bin/bash
#function

call_Tests () {
 
# ctest -R Test_VTK # does not run
ctest -R Test_AffineTransform
     check_exit_code_buildbot()
ctest -R Test_Units
    check_exit_code_buildbot()
ctest -R Test_Layered_Basic
    check_exit_code_buildbot()
ctest -R Test_Queries
    check_exit_code_buildbot()
# ctest -R Test_TIMOSReaderWriter  #This test fails
# ctest -R Test_MCML #This test fails
ctest -R Test_COMSOLMeshReaderWriter 
    check_exit_code_buildbot()
# ctest -R Test_TetraEmitterFactory # This test fails
# ctest -R Test_MCMLKernel_Sample # This test fails
# ctest -R Test_InternalKernel # This test fails
ctest -R Test_Mouse
    check_exit_code_buildbot()
# ctest -R Test_BlockRandomDistribution # This test fails
ctest -R Test_RNG
    check_exit_code_buildbot()
# ctest -R DVH # This test fails
# ctest -R DSH # This test fails
# The following tests should succeed if they run and generate output for comparison, working on them
# ctest -R Test_tclShell
 ctest -R Test_FullMonte_MMC_Comparison
     check_exit_code_buildbot()
 #ctest -R Test_FullMonte_Self
 # ctest -R Test_FullMonte_Approx
 ctest -R Test_FullMonte_Composite
     check_exit_code_buildbot()
}

check_exit_code_buildbot() {
if [ $? -eq 0 ]
then
  echo "Test passed Successfully"
  exit 0
else
  echo "Test Failed" 
  exit 1
fi

}

SRC=`pwd`

# Release - Clang - VTK 7.1.1 - Boost 1.58.0

cd FullMonteSW/Build/ReleaseClang
call_Tests

if [ 1 -eq 0 ]; then
# Debug - Clang - VTK 7.1.1 - Boost 1.58.0
cd $SRC
cd FullMonteSW/Build/DebugClang
call_Tests


# RelWithDebInfo - Clang - VTK 7.1.1 - Boost 1.58.0
cd $SRC
cd FullMonteSW/Build/RelWithDebInfoClang
call_Tests


# Release - GCC 4.9 - VTK 6.3 - Boost 1.60.0
cd $SRC
 cd FullMonteSW/Build/ReleaseGCC
call_Tests


fi




