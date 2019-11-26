cmake_minimum_required(VERSION 3.10.2)

project(cxxmidi VERSION 0.2.0 LANGUAGES CXX)

include(CTest)

add_library(CxxMidi INTERFACE)
add_library(CxxMidi::CxxMidi ALIAS CxxMidi)

target_include_directories(CxxMidi
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

include(CMakePackageConfigHelpers)

set(INCLUDE_INSTALL_DIR include)
set(LIB_INSTALL_DIR lib)
set(SYSCONFIG_INSTALL_DIR etc)

configure_package_config_file(cmake/CxxMidiConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CxxMidiConfig.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cxxmidi/cmake
  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/CxxMidiConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CxxMidiConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CxxMidiConfigVersion.cmake
  DESTINATION
    ${LIB_INSTALL_DIR}/cxxmidi/cmake)

install(DIRECTORY include/cxxmidi
  DESTINATION ${INCLUDE_INSTALL_DIR})

install(TARGETS CxxMidi
  EXPORT CxxMidi
  DESTINATION ${LIB_INSTALL_DIR}/cxxmidi/cmake)

install(EXPORT CxxMidi
  FILE CxxMidiTargets.cmake
  NAMESPACE CxxMidi::
  DESTINATION ${LIB_INSTALL_DIR}/cxxmidi/cmake)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(examples)
