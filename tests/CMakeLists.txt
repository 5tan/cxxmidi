set(THIS_TARGET "tests")

find_package(Threads REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
  # https://google.github.io/googletest/quickstart-cmake.html
  cmake_policy(SET CMP0135 NEW) 
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)
else()
  find_package(GTest REQUIRED)
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

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
  target_link_libraries(${THIS_TARGET} winmm)
endif()

gtest_discover_tests(${THIS_TARGET} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} DISCOVERY_MODE PRE_TEST)