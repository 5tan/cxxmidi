find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(sequencing main.cpp)

target_compile_options(sequencing PRIVATE -Wall -pedantic -Wextra)
set_target_properties(sequencing PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF)

target_include_directories(sequencing
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(sequencing
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})