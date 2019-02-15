cmake_minimum_required(VERSION 3.1.0)

project(synchronous)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

find_package(ALSA REQUIRED)

include_directories(
  ${ALSA_INCLUDE_DIRS}
  ../../include)

add_executable(synchronous main.cpp)
target_link_libraries(synchronous
  ${ALSA_LIBRARIES})
