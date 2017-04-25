#ifndef CXXMIDI_GUTS_MUTEX_HPP
#define CXXMIDI_GUTS_MUTEX_HPP

#include <cxxmidi/guts/compiler.hpp>

#if __cplusplus > 199711L
#    include <mutex>
typedef std::mutex NativeMutex;
#else
#   ifdef _WIN32
#       include <windows.h>
typedef CRITICAL_SECTION NativeMutex;
#   endif // _WIN32
#   ifdef __unix
#       include <pthread.h>
typedef pthread_mutex_t NativeMutex;
#   endif // __unix
#endif // __cplusplus > 199711L

namespace CxxMidi {
namespace Guts {

class Mutex
{
public:
    inline  Mutex();
    inline ~Mutex();

    Mutex(const Mutex&); // non-copyable
    Mutex &operator=(const Mutex &); // non-copyable (assignment)
#if __cplusplus > 199711L
    Mutex(Mutex&&) = default;
    Mutex& operator=(Mutex&&) = default;
#endif // __cplusplus > 199711L

    inline void lock();
    inline void unlock();

private:
    NativeMutex _nativeMutex;
};

} // namespace Guts
} // namespace CxxMidi

namespace CxxMidi {
namespace Guts {

Mutex::Mutex()
{    
#if __cplusplus > 199711L
    // no init
#else
#   ifdef _WIN32
    InitializeCriticalSectionAndSpinCount(&_nativeMutex,0x00000400);
#   endif // _WIN32
#   ifdef __unix
    pthread_mutex_init(&_nativeMutex,0);
#   endif // __unix
#endif // __cplusplus > 199711L
}

Mutex::~Mutex()
{
#if __cplusplus > 199711L
    // nothing to do
#else
#   ifdef _WIN32
    DeleteCriticalSection(&_nativeMutex);
#   endif // _WIN32
#   ifdef __unix
    pthread_mutex_destroy(&_nativeMutex);
#   endif // __unix
#endif // __cplusplus > 199711L
}

void Mutex::lock()
{
#if __cplusplus > 199711L
    _nativeMutex.lock();
#else
#   ifdef _WIN32
    EnterCriticalSection(&_nativeMutex);
#   endif // _WIN32
#   ifdef __unix
    pthread_mutex_lock(&_nativeMutex);
#   endif // __unix
#endif // __cplusplus > 199711L
}

void Mutex::unlock()
{
#if __cplusplus > 199711L
    _nativeMutex.unlock();
#else
#   ifdef _WIN32
    LeaveCriticalSection(&_nativeMutex);
#   endif // _WIN32
#   ifdef __unix
    pthread_mutex_unlock(&_nativeMutex);
#   endif // __unix
#endif // __cplusplus > 199711L
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_MUTEX_HPP
