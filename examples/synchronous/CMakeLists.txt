find_package(ALSA REQUIRED)

include_directories(
  ${ALSA_INCLUDE_DIRS}
  ../../include)

add_executable(synchronous main.cpp)
target_link_libraries(synchronous
  ${ALSA_LIBRARIES})
