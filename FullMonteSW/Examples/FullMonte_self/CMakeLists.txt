FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/colin27)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/mouse)


ADD_EXECUTABLE(FullMonte_Self FullMonte_self.cpp ../Helpers/fileReader.hpp ../Helpers/fileReader.cpp ../Helpers/fileWriter.hpp ../Helpers/fileWriter.cpp ../Helpers/visualize.hpp ../Helpers/visualize.cpp ../Helpers/chi2test.hpp ../Helpers/chi2test.cpp)

SET_TARGET_PROPERTIES (FullMonte_Self 
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Examples/FullMonte_self) 

TARGET_LINK_LIBRARIES(FullMonte_Self ${VTK_LIBRARIES})


FILE(COPY FullMonte_self.in DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_self)
FILE(COPY ExpectedData DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_self)



ADD_EXECUTABLE(Test_FullMonte_Self Test_FullMonte_self.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/chi2test.cpp ../Helpers/chi2test.hpp)

TARGET_LINK_LIBRARIES(Test_FullMonte_Self ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${VTK_LIBRARIES})
SET_SOURCE_FILES_PROPERTIES(Test_FullMonte_self.cpp PROPERTIES
	COMPILE_DEFINITIONS "BOOST_TEST_DYN_LINK;BOOST_TEST_MODULE=Test_FullMonte_Self")
ADD_TEST(NAME Test_FullMonte_Self
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_FullMonte_Self)

