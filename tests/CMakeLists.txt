find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(tests
  endianness.cpp
  file.cpp
  instrument.cpp
  main.cpp
  message.cpp
  other.cpp)

target_compile_options(tests PRIVATE -Wall -pedantic -Wextra)
set_target_properties(tests PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF)

target_link_libraries(tests
  CxxMidi::CxxMidi
  GTest::GTest
  Threads::Threads)

gtest_discover_tests(tests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})