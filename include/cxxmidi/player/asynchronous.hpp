#ifndef CXXMIDI_PLAYER_ASYNCHRONOUS_HPP
#define CXXMIDI_PLAYER_ASYNCHRONOUS_HPP

#include <cxxmidi/guts/mutex.hpp>
#include <cxxmidi/guts/thread.hpp>
#include <cxxmidi/player/abstract.hpp>

namespace cxxmidi {
class File;
namespace player {

class Asynchronous : public player::Abstract {
 public:
  inline Asynchronous(output::Abstract* output);
  inline virtual ~Asynchronous();

  Asynchronous(const Asynchronous&);             // non-copyable
  Asynchronous& operator=(const Asynchronous&);  // non-copyable (assignment)
#if __cplusplus > 199711L
  Asynchronous(Asynchronous&&) = default;
  Asynchronous& operator=(Asynchronous&&) = default;
#endif  // __cplusplus > 199711L

  inline virtual void Play();
  inline virtual void Pause();

  inline void GoTo(const time::Point& pos);
  inline time::Point CurrentTimePos();

  inline void SetFile(const File* file);
  inline void SetOutput(output::Abstract* output);
  inline output::Abstract* output();

  inline bool Finished();

  inline bool IsPlaying();
  inline bool IsPaused();

  inline void SetSpeed(float speed);
  inline float Speed();

  inline void SetCallbackHeartbeat(void (*callback_)(void*), void* context);
  inline void SetCallbackFinished(void (*callback)(void*), void* context);

  inline void SetCallbackHeartbeat(Callback* callback);
  inline void SetCallbackFinished(Callback* callback);
#if __cplusplus > 199711L
  inline void SetCallbackHeartbeat(const std::function<void()>& callback);
  inline void SetCallbackFinished(const std::function<void()>& callback);
#endif  // __cplusplus > 199711L

 private:
  bool _pauseRequest;

  inline static void* PlayerLoop(void* caller);

  cxxmidi::guts::Mutex _mutex;
  cxxmidi::guts::Thread* _thread;
};

}  // namespace Player
}  // namespace CxxMidi

#include <assert.h>

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/sleep.hpp>
#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace player {

Asynchronous::Asynchronous(output::Abstract* output)
    : Abstract(output), _pauseRequest(false), _thread(0) {}

Asynchronous::~Asynchronous() {
  if (_thread) delete _thread;
}

void Asynchronous::Play() {
  bool reject = false;

  _mutex.lock();
  reject = _isPlaying || (!_output) || (!_file);
  if (!reject) {
    _pauseRequest = false;
    _isPlaying = true;
  }
  _mutex.unlock();

  if (reject) return;

  if (_thread) delete _thread;
  _thread = new cxxmidi::guts::Thread(Asynchronous::PlayerLoop, this);
}

void Asynchronous::Pause() {
  _mutex.lock();
  _pauseRequest = true;
  _mutex.unlock();

  _thread->join();
  delete _thread;
  _thread = 0;
}

void* Asynchronous::PlayerLoop(void* caller) {
  Asynchronous* that = reinterpret_cast<Asynchronous*>(caller);

  bool finished = false;
  bool pause_request = false;
  unsigned int track_num = 0;
  unsigned int event_num = 0;
  uint32_t dt = 0;
  unsigned int us = 0;
  float speed = 0;
  // C style callbacks
  void (*clbk_fun_ptr_heartbeat)(void*) = 0;
  void* clbk_fun_ctx_heartbeat;
  void (*clbk_fun_ptr_finished)(void*) = 0;
  void* clbk_fun_ctx_finished = 0;

  // C++ style callbacs
  Callback* clbk_obj_ptr_heartbeat = 0;
  Callback* clbk_obj_ptr_finished = 0;

#if __cplusplus > 199711L
  // C++11 style callbacks
  std::function<void()> clbk_fun_heartbeat;
  std::function<void()> clbk_fun_finished;
#endif  // __cplusplus > 199711L

  while (true) {
    // copy player data
    that->_mutex.lock();
    if (!(pause_request = that->_pauseRequest)) {
      if (!(finished = that->Abstract::Finished())) {
        track_num = that->Abstract::TrackPending();
        event_num = that->_playerState[track_num].trackPointer;
        dt = that->_playerState[track_num].trackDt;
        us = converters::Dt2us(dt, that->_tempo, that->_file->TimeDivision());
        speed = that->_speed;

        clbk_fun_ptr_heartbeat = that->_clbkFunPtrHeartbeat;
        clbk_fun_ctx_heartbeat = that->_clbkFunCtxHeartbeat;
        clbk_fun_ptr_finished = that->_clbkFunPtrFinished;
        clbk_fun_ctx_finished = that->_clbkFunCtxFinished;

        clbk_obj_ptr_heartbeat = that->_clbkObjPtrHeartbeat;
        clbk_obj_ptr_finished = that->_clbkObjPtrFinished;

#if __cplusplus > 199711L
        clbk_fun_heartbeat = that->_clbkFunHeartbeat;
        clbk_fun_finished = that->_clbkFunFinished;
#endif  // __cplusplus > 199711L
      }
    }
    that->_mutex.unlock();

    if (pause_request || finished) {
      that->_mutex.lock();
      that->_pauseRequest = false;
      that->_isPlaying = false;
      that->_mutex.unlock();

      if (finished) {
        if (clbk_fun_ptr_finished) (*clbk_fun_ptr_finished)(clbk_fun_ctx_finished);

        if (clbk_obj_ptr_finished) (*clbk_obj_ptr_finished)();

#if __cplusplus > 199711L
        if (clbk_fun_finished) clbk_fun_finished();
#endif  // __cplusplus > 199711L
      }

      return 0;
    }

    // Note that the _heartbeatHelper is not protected by the _mutex.
    // It should be used only in Acynchronous::Asynchronous::playerLoop()
    // and in Abstract::goTo() (these two are exclusive).
    while ((that->_heartbeatHelper + us) >= 10000) {
      unsigned int partial = 10000 - that->_heartbeatHelper;
      that->_heartbeatHelper = 0;
      us -= partial;

      // update player data before sleep
      that->_mutex.lock();
      pause_request = that->_pauseRequest;
      speed = that->_speed;

      clbk_fun_ptr_heartbeat = that->_clbkFunPtrHeartbeat;
      clbk_fun_ctx_heartbeat = that->_clbkFunCtxHeartbeat;
      clbk_fun_ptr_finished = that->_clbkFunPtrFinished;
      clbk_fun_ctx_finished = that->_clbkFunCtxFinished;

      clbk_obj_ptr_heartbeat = that->_clbkObjPtrHeartbeat;
      clbk_obj_ptr_finished = that->_clbkObjPtrFinished;
#if __cplusplus > 199711L
      clbk_fun_heartbeat = that->_clbkFunHeartbeat;
      clbk_fun_finished = that->_clbkFunFinished;
#endif  // __cplusplus > 199711L
      that->_mutex.unlock();

      if (pause_request) {
        that->_mutex.lock();
        that->_pauseRequest = false;
        that->_isPlaying = false;
        that->_mutex.unlock();
        return 0;
      }

      sleep::SleepUs(partial / speed);
      that->_mutex.lock();
      that->_currentTimePos.AddUs(partial);
      that->_mutex.unlock();

      if (clbk_fun_ptr_heartbeat) (*clbk_fun_ptr_heartbeat)(clbk_fun_ctx_heartbeat);

      if (clbk_obj_ptr_heartbeat) (*clbk_obj_ptr_heartbeat)();

#if __cplusplus > 199711L
      if (clbk_fun_heartbeat) clbk_fun_heartbeat();
#endif  // __cplusplus > 199711L
    }

    sleep::SleepUs(us / speed);
    that->_heartbeatHelper += us;

    that->_mutex.lock();
    that->_currentTimePos.AddUs(us);
    that->ExecEvent((*that->_file)[track_num][event_num]);
    that->UpdatePlayerState(track_num, dt);
    that->_mutex.unlock();
  }

  return 0;
}

void Asynchronous::GoTo(const time::Point& pos) {
  //! @TODO If goTo is called from player's callback, different impl is needed

  _mutex.lock();
  bool was_playing = _isPlaying;
  _mutex.unlock();

  if (was_playing) this->Pause();

  _mutex.lock();
  Abstract::GoTo(pos);
  _mutex.unlock();

  if (was_playing) this->Play();
}

time::Point Asynchronous::CurrentTimePos() {
  time::Point r;
  _mutex.lock();
  r = _currentTimePos;
  _mutex.unlock();
  return r;
}

void Asynchronous::SetFile(const File* file) {
  _mutex.lock();
  Abstract::SetFile(file);
  _mutex.unlock();
}

void Asynchronous::SetOutput(output::Abstract* output) {
  _mutex.lock();
  _output = output;
  _mutex.unlock();
}

output::Abstract* Asynchronous::output() {
  output::Abstract* r;
  _mutex.lock();
  r = _output;
  _mutex.unlock();
  return r;
}

bool Asynchronous::Finished() {
  bool r;
  _mutex.lock();
  r = player::Abstract::Finished();
  _mutex.unlock();
  return r;
}

bool Asynchronous::IsPlaying() {
  bool r;
  _mutex.lock();
  r = _isPlaying;
  _mutex.unlock();
  return r;
}

bool Asynchronous::IsPaused() {
  bool r;
  _mutex.lock();
  r = !_isPlaying;
  _mutex.unlock();
  return r;
}

void Asynchronous::SetSpeed(float speed) {
  _mutex.lock();
  _speed = speed;
  _mutex.unlock();
}

float Asynchronous::Speed() {
  float r;
  _mutex.lock();
  r = _speed;
  _mutex.unlock();

  return r;
}

void Asynchronous::SetCallbackHeartbeat(void (*callback_)(void*),
                                        void* context) {
  _mutex.lock();
  _clbkFunPtrHeartbeat = callback_;
  _clbkFunCtxHeartbeat = context;
  _mutex.unlock();
}

void Asynchronous::SetCallbackFinished(void (*callback)(void*),
                                       void* context) {
  _mutex.lock();
  _clbkFunPtrFinished = callback;
  _clbkFunCtxFinished = context;
  _mutex.unlock();
}

void Asynchronous::SetCallbackHeartbeat(Callback* callback) {
  _mutex.lock();
  _clbkObjPtrHeartbeat = callback;
  _mutex.unlock();
}

void Asynchronous::SetCallbackFinished(Callback* callback) {
  _mutex.lock();
  _clbkObjPtrFinished = callback;
  _mutex.unlock();
}

#if __cplusplus > 199711L
void Asynchronous::SetCallbackHeartbeat(
    const std::function<void()>& callback) {
  _mutex.lock();
  _clbkFunHeartbeat = callback;
  _mutex.unlock();
}

void Asynchronous::SetCallbackFinished(const std::function<void()>& callback) {
  _mutex.lock();
  _clbkFunFinished = callback;
  _mutex.unlock();
}
#endif  // __cplusplus > 199711L

}  // namespace Player
}  // namespace CxxMidi

#endif  // CXXMIDI_PLAYER_ASYNCHRONOUS_HPP
