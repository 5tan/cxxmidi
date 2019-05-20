find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(asynchronous main.cpp)

target_compile_options(asynchronous PRIVATE -Wall -pedantic -Wextra)
target_compile_features(asynchronous PRIVATE cxx_std_17)

target_include_directories(asynchronous 
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(asynchronous
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})