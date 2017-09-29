ADD_EXECUTABLE(Test_Units ../Unit.cpp Test_Units.cpp ../Unit.cpp ../BaseUnit.cpp ../PrefixedUnit.cpp ../UnitPrefix.cpp ../Quantity.cpp)
TARGET_LINK_LIBRARIES(Test_Units ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ADD_TEST(NAME Test_Units
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND ${CMAKE_BINARY_DIR}/bin/Test_Units)
