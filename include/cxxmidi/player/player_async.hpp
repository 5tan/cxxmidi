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

#ifndef INCLUDE_CXXMIDI_PLAYER_PLAYER_ASYNC_HPP_
#define INCLUDE_CXXMIDI_PLAYER_PLAYER_ASYNC_HPP_

#include <assert.h>
#include <mutex>   // NOLINT() CPP11_INCLUDES
#include <thread>  // NOLINT() CPP11_INCLUDES

#include <cxxmidi/guts/player_impl.hpp>

namespace cxxmidi {
class File;
namespace player {

class PlayerAsync : public guts::PlayerImpl {
 public:
  inline explicit PlayerAsync(output::Abstract* output);
  inline ~PlayerAsync();

  PlayerAsync(const PlayerAsync&);             // non-copyable
  PlayerAsync& operator=(const PlayerAsync&);  // non-copyable (assignment)

  inline void Play();
  inline void Pause();

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

  inline void SetCallbackHeartbeat(const std::function<void()>& callback);
  inline void SetCallbackFinished(const std::function<void()>& callback);

 private:
  bool pause_request_;

  inline static void* PlayerLoop(void* caller);

  std::mutex mutex_;
  std::thread* thread_;
};

}  // namespace player
}  // namespace cxxmidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/guts/utils.hpp>

namespace cxxmidi {
namespace player {

PlayerAsync::PlayerAsync(output::Abstract* output)
    : PlayerImpl(output), pause_request_(false), thread_(0) {}

PlayerAsync::~PlayerAsync() {
  if (thread_) {
    if (thread_->joinable()) thread_->join();
    delete thread_;
  }  //! @FIXME
}

void PlayerAsync::Play() {
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
    if (thread_->joinable()) thread_->join();
    delete thread_;
  }  //! @FIXME
  thread_ = new std::thread(PlayerAsync::PlayerLoop, this);
}

void PlayerAsync::Pause() {
  mutex_.lock();
  pause_request_ = true;
  mutex_.unlock();

  if (thread_) {
    if (thread_->joinable()) thread_->join();  //! @FIXME
    delete thread_;
  }
  thread_ = 0;
}

void* PlayerAsync::PlayerLoop(void* caller) {
  PlayerAsync* that = reinterpret_cast<PlayerAsync*>(caller);

  bool finished = false;
  bool pause_request = false;
  unsigned int track_num = 0;
  unsigned int event_num = 0;
  uint32_t dt = 0;
  auto us = std::chrono::microseconds::zero();
  float speed = 0;

  std::function<void()> clbk_fun_heartbeat;
  std::function<void()> clbk_fun_finished;

  while (true) {
    // copy player data
    that->mutex_.lock();
    if (!(pause_request = that->pause_request_)) {
      if (!(finished = that->PlayerImpl::Finished())) {
        track_num = that->PlayerImpl::TrackPending();
        event_num = that->player_state_[track_num].track_pointer_;
        dt = that->player_state_[track_num].track_dt_;
        us = converters::Dt2us(dt, that->tempo_, that->file_->TimeDivision());
        speed = that->speed_;

        clbk_fun_heartbeat = that->clbk_fun_heartbeat_;
        clbk_fun_finished = that->clbk_fun_finished_;
      }
    }
    that->mutex_.unlock();

    if (pause_request || finished) {
      that->mutex_.lock();
      that->pause_request_ = false;
      that->is_playing_ = false;
      that->mutex_.unlock();

      if (finished) {
        if (clbk_fun_finished) clbk_fun_finished();
      }

      return 0;
    }

    // Note that the _heartbeatHelper is not protected by the _mutex.
    // It should be used only in Acynchronous::PlayerAsync::playerLoop()
    // and in PlayerImpl::goTo() (these two are exclusive).
    while ((that->heartbeat_helper_ + us.count()) >= 10000) {
      unsigned int partial = 10000 - that->heartbeat_helper_;
      that->heartbeat_helper_ = 0;
      us -= std::chrono::microseconds(partial);

      // update player data before sleep
      that->mutex_.lock();
      pause_request = that->pause_request_;
      speed = that->speed_;

      clbk_fun_heartbeat = that->clbk_fun_heartbeat_;
      clbk_fun_finished = that->clbk_fun_finished_;
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

      if (clbk_fun_heartbeat) clbk_fun_heartbeat();
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

void PlayerAsync::GoTo(const std::chrono::microseconds& pos) {
  //! @TODO If goTo is called from player's callback, different impl is needed

  mutex_.lock();
  bool was_playing = is_playing_;
  mutex_.unlock();

  if (was_playing) this->Pause();

  mutex_.lock();
  PlayerImpl::GoTo(pos);
  mutex_.unlock();

  if (was_playing) this->Play();
}

std::chrono::microseconds PlayerAsync::CurrentTimePos() {
  std::chrono::microseconds r;
  mutex_.lock();
  r = played_us_;
  mutex_.unlock();
  return r;
}

void PlayerAsync::SetFile(const File* file) {
  mutex_.lock();
  PlayerImpl::SetFile(file);
  mutex_.unlock();
}

void PlayerAsync::SetOutput(output::Abstract* output) {
  mutex_.lock();
  output_ = output;
  mutex_.unlock();
}

output::Abstract* PlayerAsync::output() {
  output::Abstract* r;
  mutex_.lock();
  r = output_;
  mutex_.unlock();
  return r;
}

bool PlayerAsync::Finished() {
  bool r;
  mutex_.lock();
  r = guts::PlayerImpl::Finished();
  mutex_.unlock();
  return r;
}

bool PlayerAsync::IsPlaying() {
  bool r;
  mutex_.lock();
  r = is_playing_;
  mutex_.unlock();
  return r;
}

bool PlayerAsync::IsPaused() {
  bool r;
  mutex_.lock();
  r = !is_playing_;
  mutex_.unlock();
  return r;
}

void PlayerAsync::SetSpeed(float speed) {
  mutex_.lock();
  speed_ = speed;
  mutex_.unlock();
}

float PlayerAsync::Speed() {
  float r;
  mutex_.lock();
  r = speed_;
  mutex_.unlock();

  return r;
}

void PlayerAsync::SetCallbackHeartbeat(const std::function<void()>& callback) {
  mutex_.lock();
  clbk_fun_heartbeat_ = callback;
  mutex_.unlock();
}

void PlayerAsync::SetCallbackFinished(const std::function<void()>& callback) {
  mutex_.lock();
  clbk_fun_finished_ = callback;
  mutex_.unlock();
}

}  // namespace player
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_PLAYER_PLAYER_ASYNC_HPP_
