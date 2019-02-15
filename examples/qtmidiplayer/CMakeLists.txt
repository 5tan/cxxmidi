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
  mainwindow.cpp
  playerfinishedcallback.cpp
  playerheartbeatcallback.cpp)

add_executable(qtmidiplayer ${QTMIDIPLAYER_SRCS})
target_link_libraries(qtmidiplayer
  ${ALSA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  Qt5::Widgets)
