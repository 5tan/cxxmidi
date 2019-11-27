find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(player_sync main.cpp)

target_compile_options(player_sync PRIVATE -Wall -pedantic -Wextra)
set_target_properties(player_sync PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF)

target_include_directories(player_sync
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(player_sync
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})