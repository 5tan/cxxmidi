cmake_minimum_required(VERSION 3.10.2)

project(sequencing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

include_directories(
  ${ALSA_INCLUDE_DIRS}
  ../../include)

add_executable(sequencing main.cpp)
target_link_libraries(sequencing
  ${ALSA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})
