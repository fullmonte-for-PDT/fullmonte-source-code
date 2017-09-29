ADD_SUBDIRECTORY(Placement)
ADD_SUBDIRECTORY(Modifiers)
ADD_SUBDIRECTORY(Units)

ADD_LIBRARY(FullMonteGeometry SHARED
	Geometry.cpp Points.cpp ErrorChecker.cpp TetraMesh.cpp Partition.cpp RayWalk.cpp Basis.cpp MaterialSet.cpp Material.cpp
	Filters/PlaneCrossingFilter.cpp Queries/TetrasNearPoint.cpp Queries/TetraEnclosingPointByLinearSearch.cpp Queries/PartitionRelabel.cpp
	Queries/PartitionFacesOnRegionBounds.cpp
	Sources/Abstract.cpp Sources/Print.cpp 
	Units/Unit.cpp Units/Quantity.cpp Units/BaseUnit.cpp
	TetraMeshBuilder.cpp PTTetraMeshBuilder.cpp Layer.cpp Layered.cpp)
TARGET_LINK_LIBRARIES(FullMonteGeometry ${Boost_TIMER_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})


IF (WRAP_TCL)
    SET_SOURCE_FILES_PROPERTIES(FullMonteGeometry.i PROPERTIES CPLUSPLUS ON)
    SWIG_ADD_MODULE(FullMonteGeometryTCL tcl FullMonteGeometry.i)
    SWIG_LINK_LIBRARIES(FullMonteGeometryTCL ${TCL_LIBRARY} FullMonteGeometry)
    
    INSTALL(TARGETS FullMonteGeometryTCL
        LIBRARY
        PERMISSIONS OWNER_READ OWNER_EXECUTE WORLD_READ WORLD_EXECUTE GROUP_READ GROUP_EXECUTE
    	DESTINATION lib)
ENDIF()


INSTALL(TARGETS FullMonteGeometry
    LIBRARY
    PERMISSIONS OWNER_READ OWNER_EXECUTE WORLD_READ WORLD_EXECUTE GROUP_READ GROUP_EXECUTE
	DESTINATION lib)

ADD_SUBDIRECTORY(UnitTest)


IF(WRAP_VTK)
	INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
	LINK_DIRECTORIES(${VTK_LIBRARY_DIRS} ${ANTLR3_LIB_DIR})
ENDIF()

ADD_SUBDIRECTORY(Queries)
