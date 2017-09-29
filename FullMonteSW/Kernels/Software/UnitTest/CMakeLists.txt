### MCML-like kernel with layered geometry and cylindrical scoring

ADD_BOOST_UNIT_TEST(Test_MCMLKernel Test_MCMLKernel.cpp)
TARGET_LINK_LIBRARIES(Test_MCMLKernel FullMonteSWKernel FullMonteData FullMonteMCMLFile)

ADD_TEST(NAME Test_MCMLKernel_Sample
    COMMAND ${CMAKE_BINARY_DIR}/bin/Test_MCMLKernel
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## 

ADD_BOOST_UNIT_TEST(Test_InternalKernel Test_InternalKernel.cpp)
TARGET_LINK_LIBRARIES(Test_InternalKernel FullMonteSWKernel FullMonteData FullMonteGeometry vtkFullMonte FullMonteTIMOS)
ADD_TEST(NAME Test_InternalKernel
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_InternalKernel
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

IF(WRAP_VTK)
        TARGET_LINK_LIBRARIES(Test_MCMLKernel ${VTK_LIBRARIES} vtkFullMonte)
ENDIF()

        IF (WRAP_VTK)
                INCLUDE(${VTK_USE_FILE})
                ADD_BOOST_UNIT_TEST(Test_Mouse Test_Mouse.cpp)
                TARGET_LINK_LIBRARIES(Test_Mouse vtkFullMonteTCL ${VTK_LIBRARIES} FullMonteGeometry FullMonteKernelBase FullMonteSWKernel FullMonteTIMOS FullMonteQueries)
		ADD_TEST(NAME Test_Mouse
			COMMAND ${CMAKE_BINARY_DIR}/bin/Test_Mouse
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        ## Paraview visualization file
        FILE(COPY VisualizeMouse.pvsm DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        ENDIF()

#        IF(WRAP_TCL)
#                SET_SOURCE_FILES_PROPERTIES(FullMonteSWKernel.i PROPERTIES CPLUSPLUS ON)
#        SWIG_ADD_MODULE(FullMonteSWKernelTCL tcl FullMonteSWKernel.i)
#        SWIG_LINK_LIBRARIES(FullMonteSWKernelTCL ${TCL_LIBRARY} FullMonteGeometry FullMonteSWKernel FullMonteData)

#        INSTALL(TARGETS FullMonteSWKernelTCL
#            DESTINATION lib)
#        ENDIF()
#ENDIF()

ADD_EXECUTABLE(Test_BlockRandomDistribution Test_BlockRandomDistribution.cpp ../HenyeyGreenstein.cpp)
TARGET_LINK_LIBRARIES(Test_BlockRandomDistribution ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ADD_TEST(NAME Test_BlockRandomDistribution
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_BlockRandomDistribution
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

ADD_EXECUTABLE(Test_RNG Test_RNG.cpp)
TARGET_LINK_LIBRARIES(Test_RNG ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} SFMT)
ADD_TEST(NAME Test_RNG
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_RNG
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
