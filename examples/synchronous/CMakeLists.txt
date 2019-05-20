find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(synchronous main.cpp)

target_compile_options(synchronous PRIVATE -Wall -pedantic -Wextra)
target_compile_features(synchronous PRIVATE cxx_std_17)

target_include_directories(synchronous 
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(synchronous
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})