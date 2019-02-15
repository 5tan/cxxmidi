find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)

include_directories(
  ${ALSA_INCLUDE_DIRS}
  ../../include)

add_executable(callbacks main.cpp)
target_link_libraries(callbacks
  ${ALSA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})
