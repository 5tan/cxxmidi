cmake_minimum_required(VERSION 3.1.0)

project(qtmidiplayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

include_directories(
  ${ALSA_INCLUDE_DIRS}
  ../../include)

set(QTMIDIPLAYER_SRCS
  main.cpp
  mainwindow.cpp)

add_executable(qtmidiplayer ${QTMIDIPLAYER_SRCS})
target_link_libraries(qtmidiplayer
  ${ALSA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  Qt5::Widgets)
