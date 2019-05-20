find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(callbacks main.cpp)

target_compile_options(asynchronous PRIVATE -Wall -pedantic -Wextra)
target_compile_features(asynchronous PRIVATE cxx_std_17)

target_include_directories(callbacks
  PRIVATE
    ${ALSA_INCLUDE_DIRS})

target_link_libraries(callbacks
  ${ALSA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(callbacks
  CxxMidi::CxxMidi
  Threads::Threads
  ${ALSA_LIBRARIES})