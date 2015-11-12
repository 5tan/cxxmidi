#ifndef CXXMIDI_SLEEP_HPP
#define CXXMIDI_SLEEP_HPP

namespace CxxMidi {
namespace Sleep {

#ifdef _WIN32
#include <windows.h>

inline void us(__int64 us_)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10*us_); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif // _WIN32

#ifdef __unix
#include <unistd.h>

inline void us(unsigned int us_)
{
    usleep(us_);
}
#endif // __unix

} // namespace Sleep
} // namespace CxxMidi

#endif // CXXMIDI_SLEEP_HPP
