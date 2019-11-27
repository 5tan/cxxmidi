find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

add_executable(callbacks main.cpp)

target_compile_options(callbacks PRIVATE -Wall -pedantic -Wextra)
set_target_properties(callbacks PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF)

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