ADD_LIBRARY(FullMonteMCMLFile SHARED MCMLCase.cpp MCMLInputReader.cpp MCMLOutputReader.cpp ${CMAKE_SOURCE_DIR}/External/src-mcml/mcmlio.c ${CMAKE_SOURCE_DIR}/External/src-mcml/mcmlnr.c)
TARGET_LINK_LIBRARIES(FullMonteMCMLFile FullMonteGeometry FullMonteTextFile)


IF (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    SET_SOURCE_FILES_PROPERTIES(
        ${CMAKE_SOURCE_DIR}/External/src-mcml/mcmlio.c
        PROPERTIES
            COMPILE_FLAGS "-Wno-tautological-compare -Wno-return-stack-address")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    SET_SOURCE_FILES_PROPERTIES(
        ${CMAKE_SOURCE_DIR}/External/src-mcml/mcmlio.c
        PROPERTIES
            COMPILE_FLAGS "-Wno-tautological-compare -Wno-return-local-addr")
ENDIF()

IF(WRAP_TCL)
    SET_SOURCE_FILES_PROPERTIES(FullMonteMCMLFile.i PROPERTIES CPLUSPLUS ON)
    SWIG_ADD_MODULE(FullMonteMCMLFileTCL tcl FullMonteMCMLFile.i)
    SWIG_LINK_LIBRARIES(FullMonteMCMLFileTCL FullMonteMCMLFile FullMonteGeometry ${TCL_LIBRARY})
ENDIF()

ADD_BOOST_UNIT_TEST(Test_MCML Test_MCML.cpp)
TARGET_LINK_LIBRARIES(Test_MCML FullMonteMCMLFile)
