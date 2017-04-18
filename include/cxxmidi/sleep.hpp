#ifndef CXXMIDI_SLEEP_HPP
#define CXXMIDI_SLEEP_HPP

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __unix
#include <unistd.h>
#endif // __unix

namespace CxxMidi {
namespace Sleep {

template<typename T>
inline void us(T us_)
{
#ifdef _WIN32
    __int64 us = static_cast<__int64>(us_);

    HANDLE timer;
    LARGE_INTEGER ft;

    // Convert to 100 nanosecond interval
    ft.QuadPart = -(10*us);
    // negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
#endif
#ifdef __unix
    usleep(static_cast<unsigned int>(us_));
#endif // __unix
}

} // namespace Sleep
} // namespace CxxMidi

#endif // CXXMIDI_SLEEP_HPP
