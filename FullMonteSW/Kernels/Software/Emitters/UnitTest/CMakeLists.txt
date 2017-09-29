IF(WRAP_VTK)
        LINK_DIRECTORIES(${ANTLR3_LIB_DIR})

        ADD_EXECUTABLE(Test_TetraEmitterFactory Test_TetraEmitterFactory.cpp ../TetraMeshEmitterFactory.cpp ../../PacketDirection.cpp ../../SSEMath.cpp)
        TARGET_LINK_LIBRARIES(Test_TetraEmitterFactory ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} FullMonteGeometry FullMonteTIMOS SFMT FullMontePlacement vtkFullMonte)
	ADD_TEST(NAME Test_TetraEmitterFactory
		COMMAND ${CMAKE_BINARY_DIR}/bin/Test_TetraEmitterFactory
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
ENDIF()

IF(WRAP_VTK)
    FIND_PACKAGE(VTK REQUIRED)
	INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DVTK_OUTPUT)

    SET(EXTRA_SOURCES VTKPointCloud.cpp VTKLineCluster.cpp)
    SET(EXTRA_LIBS vtkIOLegacy vtkCommonDataModel)

#ADD_EXECUTABLE(Test_Emitters Test_Emitters.cpp ${EXTRA_SOURCES} ../../PacketDirection.cpp ../../SSEMath.cpp ../TetraMeshEmitterFactory.cpp)
#TARGET_LINK_LIBRARIES(Test_Emitters ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${EXTRA_LIBS} SFMT FullMonteGeometry FullMonteTIMOS)
#ADD_TEST(NAME Test_Emitters
#	COMMAND  ${CMAKE_BINARY_DIR}/bin/Test_Emitters
#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

ELSE()
	MESSAGE("Test_Emitters cannot be built because VTK libraries are not provided")
ENDIF()

