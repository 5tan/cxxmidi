cmake_minimum_required(VERSION 3.1.0)

project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

enable_testing()

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_directories(
  ${GTEST_INCLUDE_DIRS}
  ../include)

add_executable(tests
  file.cpp
  instrument.cpp
  main.cpp
  other.cpp)

target_link_libraries(tests
  ${GTEST_BOTH_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})

add_test(tests tests)
