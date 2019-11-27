find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

add_executable(qtmidieditor
  filemodel.cpp
  fileview.cpp
  main.cpp
  mainwindow.cpp
  mainwindow.ui
  trackmodel.cpp
  trackview.cpp)

target_compile_options(qtmidieditor PRIVATE -Wall -pedantic -Wextra)
target_compile_features(qtmidieditor PRIVATE cxx_std_17)

target_include_directories(qtmidieditor
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(qtmidieditor
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES}
  Qt5::Widgets)