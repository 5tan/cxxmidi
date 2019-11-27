find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(player_async main.cpp)

target_compile_options(player_async PRIVATE -Wall -pedantic -Wextra)
target_compile_features(player_async PRIVATE cxx_std_17)

target_include_directories(player_async
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(player_async
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})