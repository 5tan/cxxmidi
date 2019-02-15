find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

include_directories(
  ${ALSA_INCLUDE_DIRS}
  ../../include)

set(QTMIDIEDITOR_SRCS
  filemodel.cpp
  fileview.cpp
  main.cpp
  mainwindow.cpp
  mainwindow.ui
  trackmodel.cpp
  trackview.cpp)

add_executable(qtmidieditor ${QTMIDIEDITOR_SRCS})
target_link_libraries(qtmidieditor
  ${ALSA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  Qt5::Widgets)
