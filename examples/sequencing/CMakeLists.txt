find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(sequencing main.cpp)

target_compile_options(sequencing PRIVATE -Wall -pedantic -Wextra)
target_compile_features(sequencing PRIVATE cxx_std_17)

target_include_directories(sequencing
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(sequencing
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})