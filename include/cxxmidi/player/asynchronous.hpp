/* *****************************************************************************
Copyright (c) 2018 Stan Chlebicki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************** */

#ifndef CXXMIDI_PLAYER_ASYNCHRONOUS_HPP
#define CXXMIDI_PLAYER_ASYNCHRONOUS_HPP

#include <cxxmidi/player/abstract.hpp>

#include <thread>
#include <mutex>

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

  inline void GoTo(const std::chrono::microseconds& pos);
  inline std::chrono::microseconds CurrentTimePos();

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
  bool pause_request_;

  inline static void* PlayerLoop(void* caller);

  std::mutex mutex_;
  std::thread* thread_;
};

}  // namespace player
}  // namespace cxxmidi

#include <assert.h>

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>

#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace player {

Asynchronous::Asynchronous(output::Abstract* output)
    : Abstract(output), pause_request_(false), thread_(0) {}

Asynchronous::~Asynchronous() {
  if (thread_) {
      if(thread_->joinable()) thread_->join();
      delete thread_;
  } //! @FIXME
}

void Asynchronous::Play() {
  bool reject = false;

  mutex_.lock();
  reject = is_playing_ || (!output_) || (!file_);
  if (!reject) {
    pause_request_ = false;
    is_playing_ = true;
  }
  mutex_.unlock();

  if (reject) return;

  if (thread_) {
      if(thread_->joinable()) thread_->join();
      delete thread_;
  } //! @FIXME
  thread_ = new std::thread(Asynchronous::PlayerLoop, this);
}

void Asynchronous::Pause() {
  mutex_.lock();
  pause_request_ = true;
  mutex_.unlock();

  if(thread_) {
  if(thread_->joinable())
      thread_->join(); //! @FIXME
  delete thread_;
  }
  thread_ = 0;
}

void* Asynchronous::PlayerLoop(void* caller) {
  Asynchronous* that = reinterpret_cast<Asynchronous*>(caller);

  bool finished = false;
  bool pause_request = false;
  unsigned int track_num = 0;
  unsigned int event_num = 0;
  uint32_t dt = 0;
  auto us = std::chrono::microseconds::zero();
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
    that->mutex_.lock();
    if (!(pause_request = that->pause_request_)) {
      if (!(finished = that->Abstract::Finished())) {
        track_num = that->Abstract::TrackPending();
        event_num = that->player_state_[track_num].track_pointer_;
        dt = that->player_state_[track_num].track_dt_;
        us = converters::Dt2us(dt, that->tempo_, that->file_->TimeDivision());
        speed = that->speed_;

        clbk_fun_ptr_heartbeat = that->clbk_fun_ptr_heartbeat_;
        clbk_fun_ctx_heartbeat = that->clbk_fun_ctx_heartbeat_;
        clbk_fun_ptr_finished = that->_clbkFunPtrFinished;
        clbk_fun_ctx_finished = that->clbk_fun_ctx_finished_;

        clbk_obj_ptr_heartbeat = that->clbk_obj_ptr_heartbeat_;
        clbk_obj_ptr_finished = that->clbk_obj_ptr_finished_;

#if __cplusplus > 199711L
        clbk_fun_heartbeat = that->clbk_fun_heartbeat_;
        clbk_fun_finished = that->clbk_fun_finished_;
#endif  // __cplusplus > 199711L
      }
    }
    that->mutex_.unlock();

    if (pause_request || finished) {
      that->mutex_.lock();
      that->pause_request_ = false;
      that->is_playing_ = false;
      that->mutex_.unlock();

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
    while ((that->heartbeat_helper_ + us.count()) >= 10000) {
      unsigned int partial = 10000 - that->heartbeat_helper_;
      that->heartbeat_helper_ = 0;
      us -= std::chrono::microseconds(partial);

      // update player data before sleep
      that->mutex_.lock();
      pause_request = that->pause_request_;
      speed = that->speed_;

      clbk_fun_ptr_heartbeat = that->clbk_fun_ptr_heartbeat_;
      clbk_fun_ctx_heartbeat = that->clbk_fun_ctx_heartbeat_;
      clbk_fun_ptr_finished = that->_clbkFunPtrFinished;
      clbk_fun_ctx_finished = that->clbk_fun_ctx_finished_;

      clbk_obj_ptr_heartbeat = that->clbk_obj_ptr_heartbeat_;
      clbk_obj_ptr_finished = that->clbk_obj_ptr_finished_;
#if __cplusplus > 199711L
      clbk_fun_heartbeat = that->clbk_fun_heartbeat_;
      clbk_fun_finished = that->clbk_fun_finished_;
#endif  // __cplusplus > 199711L
      that->mutex_.unlock();

      if (pause_request) {
        that->mutex_.lock();
        that->pause_request_ = false;
        that->is_playing_ = false;
        that->mutex_.unlock();
        return 0;
      }

      unsigned int wait = partial / speed;
      std::this_thread::sleep_for(std::chrono::microseconds(wait));
      that->mutex_.lock();
      that->played_us_ += std::chrono::microseconds(partial);
      that->mutex_.unlock();

      if (clbk_fun_ptr_heartbeat) (*clbk_fun_ptr_heartbeat)(clbk_fun_ctx_heartbeat);

      if (clbk_obj_ptr_heartbeat) (*clbk_obj_ptr_heartbeat)();

#if __cplusplus > 199711L
      if (clbk_fun_heartbeat) clbk_fun_heartbeat();
#endif  // __cplusplus > 199711L
    }

    unsigned int wait = us.count() / speed;
    std::this_thread::sleep_for(std::chrono::microseconds(wait));
    that->heartbeat_helper_ += us.count();

    that->mutex_.lock();
    that->played_us_ += std::chrono::microseconds(us);
    that->ExecEvent((*that->file_)[track_num][event_num]);
    that->UpdatePlayerState(track_num, dt);
    that->mutex_.unlock();
  }

  return 0;
}

void Asynchronous::GoTo(const std::chrono::microseconds& pos) {
  //! @TODO If goTo is called from player's callback, different impl is needed

  mutex_.lock();
  bool was_playing = is_playing_;
  mutex_.unlock();

  if (was_playing) this->Pause();

  mutex_.lock();
  Abstract::GoTo(pos);
  mutex_.unlock();

  if (was_playing) this->Play();
}

std::chrono::microseconds Asynchronous::CurrentTimePos() {
  std::chrono::microseconds r;
  mutex_.lock();
  r = played_us_;
  mutex_.unlock();
  return r;
}

void Asynchronous::SetFile(const File* file) {
  mutex_.lock();
  Abstract::SetFile(file);
  mutex_.unlock();
}

void Asynchronous::SetOutput(output::Abstract* output) {
  mutex_.lock();
  output_ = output;
  mutex_.unlock();
}

output::Abstract* Asynchronous::output() {
  output::Abstract* r;
  mutex_.lock();
  r = output_;
  mutex_.unlock();
  return r;
}

bool Asynchronous::Finished() {
  bool r;
  mutex_.lock();
  r = player::Abstract::Finished();
  mutex_.unlock();
  return r;
}

bool Asynchronous::IsPlaying() {
  bool r;
  mutex_.lock();
  r = is_playing_;
  mutex_.unlock();
  return r;
}

bool Asynchronous::IsPaused() {
  bool r;
  mutex_.lock();
  r = !is_playing_;
  mutex_.unlock();
  return r;
}

void Asynchronous::SetSpeed(float speed) {
  mutex_.lock();
  speed_ = speed;
  mutex_.unlock();
}

float Asynchronous::Speed() {
  float r;
  mutex_.lock();
  r = speed_;
  mutex_.unlock();

  return r;
}

void Asynchronous::SetCallbackHeartbeat(void (*callback_)(void*),
                                        void* context) {
  mutex_.lock();
  clbk_fun_ptr_heartbeat_ = callback_;
  clbk_fun_ctx_heartbeat_ = context;
  mutex_.unlock();
}

void Asynchronous::SetCallbackFinished(void (*callback)(void*),
                                       void* context) {
  mutex_.lock();
  _clbkFunPtrFinished = callback;
  clbk_fun_ctx_finished_ = context;
  mutex_.unlock();
}

void Asynchronous::SetCallbackHeartbeat(Callback* callback) {
  mutex_.lock();
  clbk_obj_ptr_heartbeat_ = callback;
  mutex_.unlock();
}

void Asynchronous::SetCallbackFinished(Callback* callback) {
  mutex_.lock();
  clbk_obj_ptr_finished_ = callback;
  mutex_.unlock();
}

#if __cplusplus > 199711L
void Asynchronous::SetCallbackHeartbeat(
    const std::function<void()>& callback) {
  mutex_.lock();
  clbk_fun_heartbeat_ = callback;
  mutex_.unlock();
}

void Asynchronous::SetCallbackFinished(const std::function<void()>& callback) {
  mutex_.lock();
  clbk_fun_finished_ = callback;
  mutex_.unlock();
}
#endif  // __cplusplus > 199711L

}  // namespace player
}  // namespace cxxmidi

#endif  // CXXMIDI_PLAYER_ASYNCHRONOUS_HPP
