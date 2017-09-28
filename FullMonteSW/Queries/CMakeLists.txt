ADD_LIBRARY(FullMonteQueries SHARED FluenceLineQuery.cpp DoseHistogramGenerator.cpp DoseVolumeHistogramGenerator.cpp  DoseSurfaceHistogramGenerator.cpp DoseVolumeHistogramGenerator.cpp EnergyToFluence.cpp DoseHistogram.cpp DoseHistogramCollection.cpp DirectedSurfaceOfRegion.cpp)
TARGET_LINK_LIBRARIES(FullMonteQueries FullMonteGeometry FullMonteData)

IF(WRAP_TCL)
	SET_SOURCE_FILES_PROPERTIES(FullMonteQueries.i PROPERTIES CPLUSPLUS ON)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

	SWIG_ADD_MODULE(FullMonteQueriesTCL tcl FullMonteQueries.i)
	SWIG_LINK_LIBRARIES(FullMonteQueriesTCL FullMonteQueries ${TCL_LIBRARY})
	INSTALL(TARGETS FullMonteQueriesTCL LIBRARY DESTINATION lib)
ENDIF() 

ADD_SUBDIRECTORY(UnitTest)
#ADD_BOOST_UNIT_TEST(Test_Histograms Test_Histograms.cpp)
#TARGET_LINK_LIBRARIES(Test_Histograms FullMonteQueries FullMonteData)

#ADD_TEST(DVH ${CMAKE_BINARY_DIR}/bin/Test_Histograms foo bar)
#ADD_TEST(DSH ${CMAKE_BINARY_DIR}/bin/Test_Histograms foo bar baz)

INSTALL(TARGETS FullMonteQueries LIBRARY DESTINATION lib)
