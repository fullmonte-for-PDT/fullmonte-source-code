#ADD_BOOST_UNIT_TEST(TestQueries TestQueries.cpp)
#TARGET_LINK_LIBRARIES(TestQueries FullMonteTIMOS)

#ADD_TEST(TestQueries
#	COMMAND ${CMAKE_BINARY_DIR}/bin/TestQueries)

# Link VTK if it's used
#IF(WRAP_VTK)
#	TARGET_LINK_LIBRARIES(TestQueries vtkFullMonte)
#ENDIF()
