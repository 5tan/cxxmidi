find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(player_async main.cpp)

target_compile_options(player_async PRIVATE -Wall -pedantic -Wextra)
set_target_properties(player_async PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF)

target_include_directories(player_async
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(player_async
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})