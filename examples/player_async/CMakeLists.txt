find_package(Threads REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(ALSA REQUIRED)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  find_library(COREMIDI_LIBRARY CoreMIDI REQUIRED)
  find_library(COREAUDIO_LIBRARY CoreAudio REQUIRED)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
  find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
endif()

add_executable(player_async main.cpp)

target_compile_options(player_async PRIVATE -Wall -pedantic -Wextra)
target_compile_features(player_async PRIVATE cxx_std_17)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_include_directories(player_async PRIVATE ${ALSA_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()


target_link_libraries(player_async
  CxxMidi::CxxMidi
  Threads::Threads)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(player_async ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  target_link_libraries(player_async 
    ${COREMIDI_LIBRARY}
    ${COREAUDIO_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${AUDIOTOOLBOX_LIBRARY})
endif()