# Install script for directory: /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/VTK

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "RuntimeLibraries")
  foreach(file
      "$ENV{DESTDIR}/usr/local/lib/libvtkFullMonteTCL-6.0.so.6.0.0"
      "$ENV{DESTDIR}/usr/local/lib/libvtkFullMonteTCL-6.0.so.6.0"
      "$ENV{DESTDIR}/usr/local/lib/libvtkFullMonteTCL-6.0.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libvtkFullMonteTCL-6.0.so.6.0.0;/usr/local/lib/libvtkFullMonteTCL-6.0.so.6.0;/usr/local/lib/libvtkFullMonteTCL-6.0.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES
    "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libvtkFullMonteTCL-6.0.so.6.0.0"
    "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libvtkFullMonteTCL-6.0.so.6.0"
    "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libvtkFullMonteTCL-6.0.so"
    )
  foreach(file
      "$ENV{DESTDIR}/usr/local/lib/libvtkFullMonteTCL-6.0.so.6.0.0"
      "$ENV{DESTDIR}/usr/local/lib/libvtkFullMonteTCL-6.0.so.6.0"
      "$ENV{DESTDIR}/usr/local/lib/libvtkFullMonteTCL-6.0.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libvtkFullMonte.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so"
         OLD_RPATH "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvtkFullMonte.so")
    endif()
  endif()
endif()

