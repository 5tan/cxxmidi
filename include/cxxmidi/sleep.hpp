#ifndef CXXMIDI_SLEEP_HPP
#define CXXMIDI_SLEEP_HPP

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __unix
#include <unistd.h>
#endif  // __unix

#include <chrono>

namespace cxxmidi {
namespace sleep {

template <typename T>
inline void SleepUs(T us) {
#ifdef _WIN32
  __int64 us64 = static_cast<__int64>(us_);

  HANDLE timer;
  LARGE_INTEGER ft;

  // Convert to 100 nanosecond interval
  ft.QuadPart = -(10 * us64);
  // negative value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
#endif
#ifdef __unix
#endif  // __unix
}

}  // namespace Sleep
}  // namespace CxxMidi

#endif  // CXXMIDI_SLEEP_HPP
