find_package(Threads REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Gui Widgets REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(ALSA REQUIRED)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

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

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_include_directories(qtmidieditor PRIVATE ${ALSA_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

target_link_libraries(qtmidieditor
  CxxMidi::CxxMidi
  Threads::Threads
  Qt6::Widgets)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(qtmidieditor ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()