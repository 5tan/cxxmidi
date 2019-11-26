find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(player_sync main.cpp)

target_compile_options(player_sync PRIVATE -Wall -pedantic -Wextra)
target_compile_features(player_sync PRIVATE cxx_std_17)

target_include_directories(player_sync
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(player_sync
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})