#ifndef CXXMIDI_GUTS_THREAD_HPP
#define CXXMIDI_GUTS_THREAD_HPP

#ifdef _WIN32
#include <windows.h>
typedef HANDLE NativThreadType;
#endif // _WIN32

#ifdef __unix
#include <pthread.h>
typedef pthread_t NativThreadType;
#endif // __unix

namespace CxxMidi {
namespace Guts {

class Thread
{
public:

    inline Thread();
    inline Thread( void* (*fun_)(void *), void* caller_ );
    inline ~Thread();

    inline void join();

private:

    NativThreadType _nativeThread;
};

} // namespace Guts
} // namespace CxxMidi


namespace CxxMidi {
namespace Guts {

Thread::Thread( void* (*fun_)(void *), void* caller_ )
{

#ifdef _WIN32
    _nativeThread = CreateThread(
                NULL,                         // default security attributes
                0,                            // use default stack size
                (LPTHREAD_START_ROUTINE)fun_, // thread function name
                caller_,                      // argument to thread function
                0,                            // use default creation flags
                NULL);                        // returns the thread identifier
#endif // _WIN32
#ifdef __unix
    pthread_create(&_nativeThread,0,fun_,caller_);
#endif // __unix

}

Thread::Thread()
{

}

Thread::~Thread()
{
}

void Thread::join()
{
#ifdef _WIN32
    WaitForSingleObject(_nativeThread, INFINITE);
#endif // _WIN32
#ifdef __unix
    pthread_join(_nativeThread, NULL);
#endif // __unix
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_THREAD_HPP
