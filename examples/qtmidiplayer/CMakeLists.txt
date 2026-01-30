find_package(Threads REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Gui Widgets REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(ALSA REQUIRED)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  find_library(COREMIDI_LIBRARY CoreMIDI REQUIRED)
  find_library(COREAUDIO_LIBRARY CoreAudio REQUIRED)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
  find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
endif()


add_executable(qtmidiplayer
  main.cpp
  mainwindow.cpp)

target_compile_options(qtmidiplayer PRIVATE -Wall -pedantic -Wextra)
target_compile_features(qtmidiplayer PRIVATE cxx_std_17)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_include_directories(qtmidiplayer PRIVATE ${ALSA_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

target_link_libraries(qtmidiplayer
  CxxMidi::CxxMidi
  Threads::Threads
  Qt6::Widgets)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(qtmidiplayer ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  target_link_libraries(qtmidiplayer 
    ${COREMIDI_LIBRARY}
    ${COREAUDIO_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${AUDIOTOOLBOX_LIBRARY})
endif()