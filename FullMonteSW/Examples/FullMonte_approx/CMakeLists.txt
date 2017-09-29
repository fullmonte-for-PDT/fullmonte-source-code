FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/colin27)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/mouse)


ADD_EXECUTABLE(FullMonte_Approx FullMonte_approx.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/fileWriter.hpp ../Helpers/fileWriter.cpp ../Helpers/visualize.hpp ../Helpers/visualize.cpp ../Helpers/chi2test.hpp ../Helpers/chi2test.cpp)

SET_TARGET_PROPERTIES (FullMonte_Approx 
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Examples/FullMonte_approx) 

TARGET_LINK_LIBRARIES(FullMonte_Approx ${VTK_LIBRARIES})


FILE(COPY FullMonte_approx.in DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_approx)
FILE(COPY ExpectedData DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_approx)



ADD_EXECUTABLE(Test_FullMonte_Approx Test_FullMonte_approx.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/chi2test.cpp ../Helpers/chi2test.hpp)

TARGET_LINK_LIBRARIES(Test_FullMonte_Approx ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${VTK_LIBRARIES})
SET_SOURCE_FILES_PROPERTIES(Test_FullMonte_approx.cpp PROPERTIES
	COMPILE_DEFINITIONS "BOOST_TEST_DYN_LINK;BOOST_TEST_MODULE=Test_FullMonte_Approx")
ADD_TEST(NAME Test_FullMonte_Approx
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_FullMonte_Approx)

ADD_EXECUTABLE(Position find_pos.cpp spherePos.cpp spherePos.hpp)

SET_TARGET_PROPERTIES (Position
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Examples/FullMonte_approx)
FILE(COPY find_pos.in DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_approx)

