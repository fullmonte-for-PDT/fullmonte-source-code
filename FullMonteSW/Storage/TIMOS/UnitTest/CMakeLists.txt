ADD_BOOST_UNIT_TEST(Test_TIMOSReaderWriter Test_TIMOSReaderWriter.cpp)
TARGET_LINK_LIBRARIES(Test_TIMOSReaderWriter FullMonteGeometry FullMonteTIMOS)
ADD_TEST(NAME Test_TIMOSReaderWriter
	COMMAND ${CMAKE_COMMAND}
		-DCMD1=${CMAKE_BINARY_DIR}/bin/Test_TIMOSReaderWriter
		-DCMD2=compare_files ${CMAKE_CURRENT_BINARY_DIR}/TIMOSMeshWriter.mesh ${CMAKE_CURRENT_SOURCE_DIR}/TIMOSMeshWriter.expected.mesh
	-P ${CMAKE_CURRENT_SOURCE_DIR}/run_Test_TIMOSReaderWriter.cmake
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


