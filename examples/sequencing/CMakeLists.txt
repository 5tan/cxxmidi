find_package(Threads REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(ALSA REQUIRED)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  find_library(COREMIDI_LIBRARY CoreMIDI REQUIRED)
  find_library(COREAUDIO_LIBRARY CoreAudio REQUIRED)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
  find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
endif()

add_executable(sequencing main.cpp)

target_compile_options(sequencing PRIVATE -Wall -pedantic -Wextra)
target_compile_features(sequencing PRIVATE cxx_std_17)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_include_directories(sequencing PRIVATE ${ALSA_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

target_link_libraries(sequencing
  CxxMidi::CxxMidi
  Threads::Threads)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(sequencing ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  target_link_libraries(sequencing 
    ${COREMIDI_LIBRARY}
    ${COREAUDIO_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${AUDIOTOOLBOX_LIBRARY})
endif()