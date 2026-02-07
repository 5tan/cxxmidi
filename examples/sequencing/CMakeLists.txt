set(THIS_TARGET "sequencing")

find_package(Threads REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(ALSA REQUIRED)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  find_library(COREMIDI_LIBRARY CoreMIDI REQUIRED)
  find_library(COREAUDIO_LIBRARY CoreAudio REQUIRED)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
  find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
endif()

add_executable(${THIS_TARGET} main.cpp)

if(MSVC)
  target_compile_options(${THIS_TARGET} PRIVATE /W4)
else()
  target_compile_options(${THIS_TARGET} PRIVATE -Wall -pedantic -Wextra)
endif()
target_compile_features(${THIS_TARGET} PRIVATE cxx_std_17)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_include_directories(${THIS_TARGET} PRIVATE ${ALSA_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

target_link_libraries(${THIS_TARGET}
  CxxMidi::CxxMidi
  Threads::Threads)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(${THIS_TARGET} ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  target_link_libraries(${THIS_TARGET}
    ${COREMIDI_LIBRARY}
    ${COREAUDIO_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${AUDIOTOOLBOX_LIBRARY})
  # todo
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows"
  OR CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
  target_link_libraries(${THIS_TARGET} winmm)
endif()