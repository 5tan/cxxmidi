#ifndef CXXMIDI_GUTS_THREAD_HPP
#define CXXMIDI_GUTS_THREAD_HPP

#include <cxxmidi/guts/compiler.hpp>

#if __cplusplus > 199711L
#    include <thread>
typedef std::thread NativeThread;
#else
#   ifdef _WIN32
#     include <windows.h>
typedef HANDLE NativeThread;
#   endif // _WIN32
#   ifdef __unix
#      include <pthread.h>
typedef pthread_t NativeThread;
#   endif // __unix
#endif // __cplusplus > 199711L

namespace CxxMidi {
namespace Guts {

class Thread
{
public:

    inline Thread();
    inline Thread( void* (*fun_)(void *), void* ctx_);
    inline ~Thread();

    Thread(const Thread&); // non-copyable
    Thread &operator=(const Thread &); // non-copyable (assignment)
#if __cplusplus > 199711L
    Thread(Thread&&) = default;
    Thread& operator=(Thread&&) = default;
#endif // __cplusplus > 199711L

    inline void join();

private:

    NativeThread _nativeThread;
};

} // namespace Guts
} // namespace CxxMidi


namespace CxxMidi {
namespace Guts {

Thread::Thread( void* (*fun_)(void *), void* ctx_)
{
#if __cplusplus > 199711L
    _nativeThread = std::thread(fun_,ctx_);
#else
#   ifdef _WIN32
    _nativeThread = CreateThread(
                NULL,                         // default security attributes
                0,                            // use default stack size
                (LPTHREAD_START_ROUTINE)fun_, // thread function name
                ctx_,                         // argument to thread function
                0,                            // use default creation flags
                NULL);                        // returns the thread identifier
#   endif // _WIN32
#   ifdef __unix
    pthread_create(&_nativeThread,0,fun_,ctx_);
#   endif // __unix
#endif // __cplusplus > 199711L
}

Thread::Thread()
{

}

Thread::~Thread()
{

}

void Thread::join()
{
#if __cplusplus > 199711L
#    include <thread>
    if(_nativeThread.joinable())
        _nativeThread.join();
#else
#   ifdef _WIN32
    WaitForSingleObject(_nativeThread, INFINITE);
    CloseHandle(_nativeThread);
#   endif // _WIN32
#   ifdef __unix
    pthread_join(_nativeThread, NULL);
#   endif // __unix
#endif // __cplusplus > 199711L
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_THREAD_HPP
