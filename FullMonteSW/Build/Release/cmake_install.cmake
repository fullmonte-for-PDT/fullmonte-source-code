# Install script for directory: /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/tcl/FullMonte.tcl")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/tcl" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME "FullMonte.tcl" FILES "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/tcl/FullMonte.tcl.install")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/tcl/pkgIndex.tcl")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/tcl" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME "pkgIndex.tcl" FILES "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/tcl/pkgIndex.tcl.install")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "tclmonte.sh" FILES "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/tclmonte.sh.install")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/VTK/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/SFMT/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Geometry/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Storage/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Kernels/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/OutputTypes/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Queries/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/Examples/cmake_install.cmake")
  include("/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/External/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
