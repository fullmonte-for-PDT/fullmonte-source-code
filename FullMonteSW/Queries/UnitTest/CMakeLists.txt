ADD_BOOST_UNIT_TEST(Test_Histograms Test_Histograms.cpp)
TARGET_LINK_LIBRARIES(Test_Histograms FullMonteQueries FullMonteData)

ADD_TEST(NAME DVH 
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_Histograms foo bar
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
ADD_TEST(NAME DSH 
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_Histograms foo bar baz
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

