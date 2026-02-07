set(THIS_TARGET "tests")

find_package(Threads REQUIRED)

find_package(GTest QUIET)

if(NOT GTest_FOUND)
  include(FetchContent)

  fetchcontent_declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz)

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  fetchcontent_makeavailable(googletest)
endif()

add_executable(${THIS_TARGET}
  endianness.cpp
  file.cpp
  instrument.cpp
  main.cpp
  message.cpp
  note.cpp
  other.cpp
  player_base.cpp)

if(MSVC)
  target_compile_options(${THIS_TARGET} PRIVATE /W4)
else()
  target_compile_options(${THIS_TARGET} PRIVATE -Wall -pedantic -Wextra)
endif()
target_compile_features(${THIS_TARGET} PRIVATE cxx_std_17)

target_link_libraries(${THIS_TARGET}
  CxxMidi::CxxMidi
  GTest::gtest_main
  Threads::Threads)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows"
  OR CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
  target_link_libraries(${THIS_TARGET} winmm)
endif()

include(GoogleTest)
gtest_discover_tests(
  ${THIS_TARGET}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DISCOVERY_MODE PRE_TEST)