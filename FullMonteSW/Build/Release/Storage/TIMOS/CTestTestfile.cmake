# CMake generated Testfile for 
# Source directory: /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Storage/TIMOS
# Build directory: /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Storage/TIMOS
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(TIMOSReaderWriter "COMMAND" "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/bin/TIMOSReaderWriter" "COMMAND" "diff" "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Storage/TIMOS/TIMOSMeshWriter.mesh" "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Storage/TIMOS/TIMOSMeshWriter.expected.mesh")
