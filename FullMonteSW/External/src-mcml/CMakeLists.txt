## CMakeLists.txt file created by Jeffrey Cassidy / University of Toronto 2017

PROJECT(MCML)

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

ADD_COMPILE_OPTIONS(-Wno-implicit-function-declaration)

# Silence warnings arising from MCML code
IF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    ADD_COMPILE_OPTIONS(-Wno-return-local-addr)
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    ADD_COMPILE_OPTIONS(-Wno-return-stack-address)
    ADD_COMPILE_OPTIONS(-Wno-tautological-compare)
    ADD_COMPILE_OPTIONS(-Wno-tautological-constant-out-of-range-compare)
    ADD_COMPILE_OPTIONS(-Wno-main-return-type)
ENDIF()

FILE(COPY
    sample.mci
    DESTINATION ${CMAKE_SOURCE_DIR}/data/MCML/sample)

INSTALL(FILES
        sample.mci
        sample.mco
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/data/MCML/sample)


ADD_EXECUTABLE(mcml mcmlgo.c mcmlio.c mcmlmain.c mcmlnr.c)
TARGET_LINK_LIBRARIES(mcml m)

