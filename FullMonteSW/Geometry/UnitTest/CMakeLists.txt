ADD_BOOST_UNIT_TEST(Test_Layered Test_Layered.cpp)
TARGET_LINK_LIBRARIES(Test_Layered FullMonteGeometry)
ADD_TEST(NAME Test_Layered_Basic
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_Layered)


ADD_BOOST_UNIT_TEST(Test_Queries Test_Queries.cpp)
TARGET_LINK_LIBRARIES(Test_Queries FullMonteTIMOS)

ADD_TEST(NAME Test_Queries
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_Queries)

# Link VTK if it's used
IF(WRAP_VTK)
        TARGET_LINK_LIBRARIES(Test_Queries vtkFullMonte)
ENDIF()

