find_package(Threads REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(ALSA REQUIRED)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

add_executable(player_sync main.cpp)

target_compile_options(player_sync PRIVATE -Wall -pedantic -Wextra)
target_compile_features(player_sync PRIVATE cxx_std_17)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_include_directories(player_sync PRIVATE ${ALSA_INCLUDE_DIRS})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()

target_link_libraries(player_sync
  CxxMidi::CxxMidi
  Threads::Threads)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(player_sync ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # todo
endif()