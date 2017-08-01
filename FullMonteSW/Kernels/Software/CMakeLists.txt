ADD_SUBDIRECTORY(Emitters)

ADD_LIBRARY(FullMonteSWKernel SHARED SSEMath.cpp ThreadedMCKernelBase.cpp PacketDirection.cpp HenyeyGreenstein.cpp TetraMCKernel.cpp
	Material.cpp
	Logger/VolumeAbsorptionScorer.cpp Logger/SurfaceExitScorer.cpp Logger/AbstractScorer.cpp
	Logger/PathScorer.cpp Logger/EventScorer.cpp Logger/ConservationScorer.cpp
	Tetra.cpp TetrasFromTetraMesh.cpp TetrasFromLayered.cpp)
TARGET_LINK_LIBRARIES(FullMonteSWKernel SFMT FullMonteGeometry FullMonteKernelBase FullMonteData ${Boost_LIBRARIES})

INSTALL(TARGETS FullMonteSWKernel
    DESTINATION lib)
    


### MCML-like kernel with layered geometry and cylindrical scoring

ADD_BOOST_UNIT_TEST(Test_MCMLKernel Test_MCMLKernel.cpp)
TARGET_LINK_LIBRARIES(Test_MCMLKernel FullMonteSWKernel FullMonteData FullMonteMCMLFile)

ADD_TEST(Test_MCMLKernel_Sample
    COMMAND ${CMAKE_BINARY_DIR}/bin/Test_MCMLKernel
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

## 

ADD_BOOST_UNIT_TEST(Test_InternalKernel Test_InternalKernel.cpp)
TARGET_LINK_LIBRARIES(Test_InternalKernel FullMonteSWKernel FullMonteData FullMonteGeometry vtkFullMonte FullMonteTIMOS)

IF(WRAP_VTK)
	TARGET_LINK_LIBRARIES(Test_MCMLKernel ${VTK_LIBRARIES} vtkFullMonte)
ENDIF()

	IF (WRAP_VTK)
		INCLUDE(${VTK_USE_FILE})
		ADD_BOOST_UNIT_TEST(Test_Mouse Test_Mouse.cpp)
		TARGET_LINK_LIBRARIES(Test_Mouse vtkFullMonteTCL ${VTK_LIBRARIES} FullMonteGeometry FullMonteKernelBase FullMonteSWKernel FullMonteTIMOS FullMonteQueries)

        ## Paraview visualization file
        FILE(COPY VisualizeMouse.pvsm DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	ENDIF()
	
	IF(WRAP_TCL)
		SET_SOURCE_FILES_PROPERTIES(FullMonteSWKernel.i PROPERTIES CPLUSPLUS ON)
    	SWIG_ADD_MODULE(FullMonteSWKernelTCL tcl FullMonteSWKernel.i)
    	SWIG_LINK_LIBRARIES(FullMonteSWKernelTCL ${TCL_LIBRARY} FullMonteGeometry FullMonteSWKernel FullMonteData)

        INSTALL(TARGETS FullMonteSWKernelTCL
            DESTINATION lib)
	ENDIF()
#ENDIF()

ADD_EXECUTABLE(Test_BlockRandomDistribution Test_BlockRandomDistribution.cpp HenyeyGreenstein.cpp)
TARGET_LINK_LIBRARIES(Test_BlockRandomDistribution ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

ADD_EXECUTABLE(Test_RNG Test_RNG.cpp)
TARGET_LINK_LIBRARIES(Test_RNG ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} SFMT)
