PROJECT(FullMonte)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(FULLMONTE_VERSION_MAJOR 1)
SET(FULLMONTE_VERSION_MINOR 0)

ENABLE_TESTING()


## Boost setup
FIND_PACKAGE(Boost 1.58.0 REQUIRED COMPONENTS serialization system program_options timer)

INCLUDE(cmake/BoostUnitTest.cmake)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

## Include dirs for FullMonte
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/..)



SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

## Compiler setup
SET(CMAKE_CXX_STANDARD 11)

### Architecture
execute_process(COMMAND cat /proc/cpuinfo
COMMAND grep -c avx2
OUTPUT_VARIABLE OUTAVX2)

execute_process(COMMAND cat /proc/cpuinfo
COMMAND grep -c avx
OUTPUT_VARIABLE OUTAVX)


#MESSAGE(${OUT})
IF(OUTAVX2 GREATER 0)
        MESSAGE("AVX2 is found")
        SET(ARCH AVX2 CACHE STRING "Architecture flag for -m option")
ELSEIF(OUTAVX GREATER 0)
        MESSAGE("AVX is found")
        SET(ARCH AVX CACHE STRING "Architecture flag for -m option")
ELSE()
        MESSAGE(FATAL_ERROR "CPU does not provide AVX")

ENDIF()


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

SET(RUNTIME_LINKER_PATH_VARNAME LD_LIBRARY_PATH)

IF(APPLE)
    ## Require .so extension (Mac likes to make it .dylib, causing a lot of the TCL scripts to bomb)
    SET(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
    
    # Prevent unnecessary Clang warnings 
    ADD_COMPILE_OPTIONS(-Wno-missing-braces)
    
    ## Boost concept checks cause a horrendous number of (harmless) unused local typedef warnings
    OPTION(WARN_DISABLE_UNUSED_LOCAL_TYPEDEF ON)
    
    # Apple uses DYLD_LIBRARY_PATH for runtime link path, whereas most others use LD_LIBRARY_PATH
    SET(RUNTIME_LINKER_PATH_VARNAME DYLD_LIBRARY_PATH)

    MESSAGE("Using Apple rpath settings")
    
    
    # New Mac OS X has more restrictive security settings around rpath
    SET(CMAKE_SKIP_BUILD_RPATH FALSE)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF()


OPTION(WARN_DISABLE_UNUSED_LOCAL_TYPEDEF OFF)

IF(WARN_DISABLE_UNUSED_LOCAL_TYPEDEF)
    ADD_COMPILE_OPTIONS(-Wno-unused-local-typedef)
ENDIF()

IF("${ARCH}" STREQUAL "AVX2")
    IF(APPLE)
        # Apple provides some intrinsics that GCC does not (see source ssemath.h)
        # ADD_DEFINITIONS(-DAPPLE_AVX2)
    ENDIF()
    ADD_DEFINITIONS(-DUSE_AVX2 -DHAVE_AVX2)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mavx -msse4.1")
ELSEIF("${ARCH}" STREQUAL "AVX")
    ADD_DEFINITIONS(-DUSE_AVX -DHAVE_AVX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -msse4.1")
ENDIF()

### Warnings and esoteric settings
IF(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    # Required to avoid an error in colliding mangled names
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fabi-version=6")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
ENDIF()


MESSAGE("Compiler ID: '${CMAKE_CXX_COMPILER_ID}' with flags '${CMAKE_CXX_FLAGS}'")

GET_DIRECTORY_PROPERTY(DPROPS COMPILE_DEFINITIONS)
MESSAGE("Compile_definitions: ${DPROPS}")




###### TCL Wrapping
OPTION(WRAP_TCL ON)

IF(WRAP_TCL)
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    
    SET(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -Wall)

	FIND_PACKAGE(TCL REQUIRED)
	INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
	
ENDIF()


###### VTK bindings
OPTION(WRAP_VTK ON)

IF (WRAP_VTK)
	FIND_PACKAGE(VTK REQUIRED)
	INCLUDE(${VTK_USE_FILE})
	ADD_SUBDIRECTORY(VTK)
ENDIF()



SET(FULLMONTE_THREAD_COUNT 8 CACHE STRING "Number of threads for test cases")


###### Build the program

ADD_SUBDIRECTORY(SFMT)
ADD_SUBDIRECTORY(Geometry)
ADD_SUBDIRECTORY(Storage)
ADD_SUBDIRECTORY(Kernels)
ADD_SUBDIRECTORY(OutputTypes)
ADD_SUBDIRECTORY(Queries)
ADD_SUBDIRECTORY(Examples)


###### Configure the run scripts

IF(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    SET(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR_DEBUG})
ELSE()
    SET(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR_RELEASE})
ENDIF()

IF(WRAP_TCL)
    FILE(MAKE_DIRECTORY tcl)

    GET_FILENAME_COMPONENT(Qt5LIBPATH "${Qt5Core_DIR}/../.." ABSOLUTE)
    MESSAGE("Qt5LIBPATH: ${Qt5LIBPATH}")

    SET(FullMonteSW_LIBRARY_DIR ${CMAKE_BINARY_DIR}/lib)
    SET(FULLMONTE_CONFIG_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
    CONFIGURE_FILE(tclmonte.sh.in ${CMAKE_BINARY_DIR}/tclmonte.sh)
    CONFIGURE_FILE(FullMonte.tcl.in ${CMAKE_BINARY_DIR}/lib/tcl/FullMonte.tcl)
    CONFIGURE_FILE(pkgIndex.tcl.in ${CMAKE_BINARY_DIR}/lib/tcl/pkgIndex.tcl)


    SET(FULLMONTE_CONFIG_DATA_DIR ${CMAKE_INSTALL_PREFIX}/data)
    SET(FullMonteSW_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
    CONFIGURE_FILE(tclmonte.sh.in ${CMAKE_BINARY_DIR}/tclmonte.sh.install)
    CONFIGURE_FILE(FullMonte.tcl.in ${CMAKE_BINARY_DIR}/lib/tcl/FullMonte.tcl.install)

    FILE(
        COPY
            ${CMAKE_BINARY_DIR}/tclmonte.sh
            ${CMAKE_BINARY_DIR}/tclmonte.sh.install
        DESTINATION ${CMAKE_BINARY_DIR}/bin
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)

    INSTALL(
        FILES ${CMAKE_BINARY_DIR}/lib/tcl/FullMonte.tcl.install
        DESTINATION 
            ${CMAKE_INSTALL_PREFIX}/lib/tcl
        RENAME
            FullMonte.tcl
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    INSTALL(
        FILES       ${CMAKE_BINARY_DIR}/lib/tcl/pkgIndex.tcl
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/tcl
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    INSTALL(
        PROGRAMS
            ${CMAKE_BINARY_DIR}/tclmonte.sh.install
        DESTINATION bin
        RENAME
            tclmonte.sh)
ENDIF()
    


###### Configure headers to point to data (for test cases)

FILE(MAKE_DIRECTORY FullMonteSW)

SET(FULLMONTE_CONFIG_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
CONFIGURE_FILE(Config.h.in ${CMAKE_BINARY_DIR}/FullMonteSW/Config.h)

SET(FULLMONTE_CONFIG_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/data)
CONFIGURE_FILE(Config.h.in ${CMAKE_BINARY_DIR}/FullMonteSW/Config.install.h)



###### Create CMake config files

SET(FullMonteSW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/..)
SET(FullMonteSW_LIBRARY_DIR ${CMAKE_BINARY_DIR}/lib)
SET(FullMonteSW_CMAKE_DIR ${CMAKE_BINARY_DIR}/cmake)
CONFIGURE_FILE(FullMonteSWConfig.cmake.in ${CMAKE_BINARY_DIR}/cmake/FullMonteSWConfig.cmake)

SET(FullMonteSW_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
SET(FullMonteSW_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
SET(FullMonteSW_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)
CONFIGURE_FILE(FullMonteSWConfig.cmake.in ${CMAKE_BINARY_DIR}/cmake/FullMonteSWConfig.install.cmake)

FILE(COPY cmake/UseFullMonteSW.cmake DESTINATION ${CMAKE_BINARY_DIR}/cmake)

ADD_SUBDIRECTORY(External)
