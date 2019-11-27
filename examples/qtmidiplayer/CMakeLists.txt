find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

add_executable(qtmidiplayer
  main.cpp
  mainwindow.cpp)

target_compile_options(qtmidiplayer PRIVATE -Wall -pedantic -Wextra)
set_target_properties(qtmidiplayer PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF)

target_include_directories(qtmidiplayer
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(qtmidiplayer
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES}
  Qt5::Widgets)
