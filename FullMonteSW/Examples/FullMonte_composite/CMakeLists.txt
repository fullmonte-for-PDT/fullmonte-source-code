FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/colin27)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/mouse)


ADD_EXECUTABLE(FullMonte_Composite FullMonte_composite.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/fileWriter.hpp ../Helpers/fileWriter.cpp ../Helpers/visualize.hpp ../Helpers/visualize.cpp ../Helpers/chi2test.hpp ../Helpers/chi2test.cpp)

SET_TARGET_PROPERTIES (FullMonte_Composite 
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Examples/FullMonte_composite) 

TARGET_LINK_LIBRARIES(FullMonte_Composite ${VTK_LIBRARIES})


FILE(COPY FullMonte_composite.in DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_composite)
FILE(COPY ExpectedData DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_composite)



ADD_EXECUTABLE(Test_FullMonte_Composite Test_FullMonte_composite.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/chi2test.cpp ../Helpers/chi2test.hpp)

TARGET_LINK_LIBRARIES(Test_FullMonte_Composite ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${VTK_LIBRARIES})
SET_SOURCE_FILES_PROPERTIES(Test_FullMonte_composite.cpp PROPERTIES
	COMPILE_DEFINITIONS "BOOST_TEST_DYN_LINK;BOOST_TEST_MODULE=Test_FullMonte_Composite")

ADD_TEST(NAME Test_FullMonte_Composite
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_FullMonte_Composite)

