#ifndef CXXMIDI_GUTS_MUTEX_HPP
#define CXXMIDI_GUTS_MUTEX_HPP

namespace CxxMidi {
namespace Guts {

#ifdef _WIN32
#include <windows.h>
typedef CRITICAL_SECTION NativeMutexType;
#endif // _WIN32

#ifdef __unix
#include <pthread.h>
typedef pthread_mutex_t NativeMutexType;
#endif // __unix

class Mutex
{
public:
    inline  Mutex();
    inline ~Mutex();
    inline void lock();
    inline void unlock();

private:
    NativeMutexType _nativeMutex;
};

} // namespace Guts
} // namespace CxxMidi

namespace CxxMidi {
namespace Guts {

Mutex::Mutex()
{
#ifdef _WIN32
    InitializeCriticalSectionAndSpinCount(&_nativeMutex,0x00000400);
#endif // _WIN32
#ifdef __unix
    pthread_mutex_init(&_nativeMutex,0);
#endif // __unix
}

Mutex::~Mutex()
{
#ifdef _WIN32
    DeleteCriticalSection(&_nativeMutex);
#endif // _WIN32
#ifdef __unix
    pthread_mutex_destroy(&_nativeMutex);
#endif // __unix
}

void Mutex::lock()
{
#ifdef _WIN32
    EnterCriticalSection(&_nativeMutex);
#endif // _WIN32
#ifdef __unix
    pthread_mutex_lock(&_nativeMutex);
#endif // __unix
}

void Mutex::unlock()
{
#ifdef _WIN32
    LeaveCriticalSection(&_nativeMutex);
#endif // _WIN32
#ifdef __unix
    pthread_mutex_unlock(&_nativeMutex);
#endif // __unix
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_MUTEX_HPP
