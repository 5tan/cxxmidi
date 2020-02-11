find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(tests
  endianness.cpp
  file.cpp
  instrument.cpp
  main.cpp
  message.cpp
  note.cpp
  other.cpp
  player_base.cpp)

target_compile_options(tests PRIVATE -Wall -pedantic -Wextra)
target_compile_features(tests PRIVATE cxx_std_17)

target_link_libraries(tests
  CxxMidi::CxxMidi
  GTest::GTest
  Threads::Threads)

gtest_discover_tests(tests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})