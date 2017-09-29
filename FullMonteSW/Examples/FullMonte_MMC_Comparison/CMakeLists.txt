FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/colin27)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Result/mouse)


ADD_EXECUTABLE(FullMonte_MMC_Comparison FullMonte_MMC_Comparison.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/fileWriter.cpp ../Helpers/fileWriter.hpp ../Helpers/visualize.hpp ../Helpers/visualize.cpp ../Helpers/chi2test.hpp ../Helpers/chi2test.cpp)

SET_TARGET_PROPERTIES (FullMonte_MMC_Comparison
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Examples/FullMonte_MMC_Comparison) 

TARGET_LINK_LIBRARIES(FullMonte_MMC_Comparison ${VTK_LIBRARIES})

FILE(COPY FullMonte_MMC_Comparison.in DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_MMC_Comparison)


ADD_EXECUTABLE(Test_FullMonte_MMC_Comparison Test_FullMonte_MMC_Comparison.cpp ../Helpers/fileReader.cpp ../Helpers/fileReader.hpp ../Helpers/chi2test.cpp ../Helpers/chi2test.hpp)

TARGET_LINK_LIBRARIES(Test_FullMonte_MMC_Comparison ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${VTK_LIBRARIES})
SET_SOURCE_FILES_PROPERTIES(Test_FullMonte_MMC_Comparison.cpp PROPERTIES
	COMPILE_DEFINITIONS "BOOST_TEST_DYN_LINK;BOOST_TEST_MODULE=Test_FullMonte_MMC_Comparison")
ADD_TEST(NAME Test_FullMonte_MMC_Comparison
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_BINARY_DIR}/bin/Test_FullMonte_MMC_Comparison)


FILE(COPY ExpectedData DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_MMC_Comparison)
