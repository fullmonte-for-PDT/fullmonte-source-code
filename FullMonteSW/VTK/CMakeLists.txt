IF(WRAP_TCL)
	SET(VTK_INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_PREFIX}/bin)
	SET(VTK_INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
	SET(VTK_INSTALL_ARCHIVE_DIR ${CMAKE_INSTALL_PREFIX}/lib)
	
	INCLUDE(vtkModuleMacros)
	INCLUDE(vtkTclWrapping)
	
	SET(BUILD_SHARED_LIBS_PREV ${BUILD_SHARED_LIBS})
	SET(BUILD_SHARED_LIBS ON)
	
	SET(vtkFullMonte_TCL_NAME vtkFullMonte)	
ENDIF()

SET(MODULES vtkFullMonteTetraMeshWrapper;vtkFullMonteArrayAdaptor;vtkFullMontePacketPositionTraceSetToPolyData;vtkFullMonteMeshFromUnstructuredGrid;vtkFullMonteMeshFacesToPolyData)

# vtkDoseHistogramWrapper.cpp vtkSourceExporter.cpp

FOREACH(MOD ${MODULES})
	LIST(APPEND MODULECPP "${MOD}.cpp")
ENDFOREACH()

ADD_LIBRARY(vtkFullMonte SHARED ${MODULECPP} SwigWrapping.cpp)
TARGET_LINK_LIBRARIES(vtkFullMonte ${VTK_LIBRARIES} FullMonteData FullMonteGeometry)

IF(WRAP_TCL)
	FOREACH(MOD ${MODULES})
		LIST(APPEND HFILES ${MOD}.h)
		LIST(APPEND CXXSRC ${MOD}.cpp)
	ENDFOREACH()

	vtk_add_tcl_wrapping(vtkFullMonte "${HFILES}" "")
ENDIF()

ADD_SUBDIRECTORY(UnitTest)
#Temporarily disabled due to missing file
#ADD_BOOST_UNIT_TEST(TestVTK TestVTK.cpp)
#TARGET_LINK_LIBRARIES(TestVTK vtkFullMonte FullMonteTIMOS)

#ADD_TEST(TestVTK
#	COMMAND ${CMAKE_BINARY_DIR}/bin/TestVTK)

INSTALL (
    TARGETS vtkFullMonte
    DESTINATION lib)

SET(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_PREV})
