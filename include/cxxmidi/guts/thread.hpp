#ifndef CXXMIDI_GUTS_THREAD_HPP
#define CXXMIDI_GUTS_THREAD_HPP

#ifdef _WIN32
#include <windows.h>
typedef HANDLE NativeThread;
#endif // _WIN32

#ifdef __unix
#include <pthread.h>
typedef pthread_t NativeThread;
#endif // __unix

namespace CxxMidi {
namespace Guts {

class Thread
{
public:

    inline Thread();
    inline Thread( void* (*fun_)(void *), void* ctx_);
    inline ~Thread();

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

#ifdef _WIN32
    _nativeThread = CreateThread(
                NULL,                         // default security attributes
                0,                            // use default stack size
                (LPTHREAD_START_ROUTINE)fun_, // thread function name
                ctx_,                      // argument to thread function
                0,                            // use default creation flags
                NULL);                        // returns the thread identifier
#endif // _WIN32
#ifdef __unix
    pthread_create(&_nativeThread,0,fun_,ctx_);
#endif // __unix

}

Thread::Thread()
{

}

Thread::~Thread()
{
    this->join();
}

void Thread::join()
{
#ifdef _WIN32
    WaitForSingleObject(_nativeThread, INFINITE);
    CloseHandle(_nativeThread)
#endif // _WIN32
#ifdef __unix
    pthread_join(_nativeThread, NULL);
#endif // __unix
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_THREAD_HPP
