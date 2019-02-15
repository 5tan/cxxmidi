cmake_minimum_required(VERSION 2.6)
project(cxxmidi_tests)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ../include)

add_executable(cxxmidi_tests
    file.cpp
    instrument.cpp
    main.cpp
    other.cpp)

target_link_libraries(cxxmidi_tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_test(cxxmidi_tests cxxmidi_tests)
