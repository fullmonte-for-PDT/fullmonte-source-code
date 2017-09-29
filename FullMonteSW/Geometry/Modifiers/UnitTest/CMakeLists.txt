ADD_EXECUTABLE(Test_AffineTransform Test_AffineTransform.cpp)
TARGET_LINK_LIBRARIES(Test_AffineTransform ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ADD_TEST(NAME Test_AffineTransform
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_AffineTransform)
	
