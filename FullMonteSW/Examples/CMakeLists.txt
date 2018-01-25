###### Setup basic BLI script (data path, VTK version)
SET(FULLMONTE_DATA_DIR ${CMAKE_SOURCE_DIR}/data)
CONFIGURE_FILE(basic_bli.tcl.in basic_bli.tcl)
CONFIGURE_FILE(mouse_energy.tcl.in mouse_energy.tcl)
CONFIGURE_FILE(mouse_fluence.tcl.in mouse_fluence.tcl)
CONFIGURE_FILE(colin27_energy.tcl.in colin27_energy.tcl)
CONFIGURE_FILE(colin27_fluence.tcl.in colin27_fluence.tcl)
CONFIGURE_FILE(colin27_comsol.tcl.in colin27_comsol.tcl)
CONFIGURE_FILE(HeadAndNeckModel.tcl.in HeadAndNeckModel.tcl)
CONFIGURE_FILE(colin27_line_source.tcl.in colin27_line_source.tcl)
CONFIGURE_FILE(colin27_energy_ball.tcl.in colin27_energy_ball.tcl)
CONFIGURE_FILE(colin27_energy_composite.tcl.in colin27_energy_composite.tcl)

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Data)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Data/colin27)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Data/mouse)


ADD_SUBDIRECTORY(FullMonte_MMC_Comparison)
ADD_SUBDIRECTORY(FullMonte_self)
ADD_SUBDIRECTORY(FullMonte_approx)
ADD_SUBDIRECTORY(FullMonte_composite)
ADD_SUBDIRECTORY(FullMonte_visualize)
ADD_SUBDIRECTORY(FullMonte_line_source)

ADD_BOOST_UNIT_TEST(Test_tclShell test_tclShell.cpp)
TARGET_LINK_LIBRARIES(Test_tclShell vtkFullMonte)
ADD_TEST(NAME Test_tclShell
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_tclShell)


FILE(COPY tclShell.tcl DESTINATION ${CMAKE_BINARY_DIR}/Examples)
FILE(COPY Helpers DESTINATION ${CMAKE_BINARY_DIR}/Examples)
#FILE(COPY Data DESTINATION ${CMAKE_BINARY_DIR}/Examples)
