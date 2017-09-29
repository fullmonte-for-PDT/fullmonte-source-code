# Create a Boost unit test with the given executable name from the named C++ source file
# Files to be tested can be placed in a library later specified with TARGET_LINK_LIBRARIES, or can be given as additional arguments

FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)

FUNCTION(ADD_BOOST_UNIT_TEST name src)
	ADD_EXECUTABLE(${name} ${src})
	TARGET_LINK_LIBRARIES(${name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
	
	SET_SOURCE_FILES_PROPERTIES(${src} PROPERTIES
		COMPILE_DEFINITIONS "BOOST_TEST_DYN_LINK;BOOST_TEST_MODULE=${name}")
ENDFUNCTION()
