set(THIS_TARGET "tests")

# set(GTest_DIR "C:/Program Files (x86)/googletest-distribution/")
# set(GTEST_INCLUDE_DIR "C:/Program Files (x86)/googletest-distribution/include")
# set(GTEST_LIBRARY "C:/Program Files (x86)/googletest-distribution/lib/libgtest.a")
# set(GTEST_MAIN_LIBRARY "C:/Program Files (x86)/googletest-distribution/libgtest_main.a")

find_package(Threads REQUIRED)
# find_package(GTest REQUIRED)

find_package(GTest QUIET)

if(NOT GTest_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  )

  FetchContent_MakeAvailable(googletest)
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

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
#   target_link_libraries(${THIS_TARGET} winmm)
# endif()

include(GoogleTest)
gtest_discover_tests(${THIS_TARGET} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} DISCOVERY_MODE PRE_TEST)