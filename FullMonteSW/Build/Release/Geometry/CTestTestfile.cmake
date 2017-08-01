# CMake generated Testfile for 
# Source directory: /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Geometry
# Build directory: /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Geometry
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_Layered_Basic "COMMAND" "Test_Layered")
subdirs(Placement)
subdirs(Modifiers)
subdirs(Units)
subdirs(Queries)
