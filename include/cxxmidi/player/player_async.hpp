/* *****************************************************************************
Copyright (c) 2018 5tan

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
#include <mutex>     // NOLINT() CPP11_INCLUDES
#include <optional>  // NOLINT() CPP11_INCLUDES
#include <thread>    // NOLINT() CPP11_INCLUDES

#include <cxxmidi/internal/player_base.hpp>

namespace cxxmidi {
class File;
namespace player {

class PlayerAsync : public internal::PlayerBase {
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

  inline void SetSpeed(float speed);
  inline float Speed();

  inline void SetHeartbeatInterval(unsigned int interval_us);
  inline unsigned int HeartbeatInterval();

  inline void SetCallbackHeartbeat(const std::function<void()>& callback);
  inline void SetCallbackFinished(const std::function<void()>& callback);

 private:
  bool pause_request_;

  inline void PlayerLoop();

  std::mutex mutex_;
  std::optional<std::thread> thread_;
};

}  // namespace player
}  // namespace cxxmidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/internal/utils.hpp>

namespace cxxmidi {
namespace player {

PlayerAsync::PlayerAsync(output::Abstract* output)
    : PlayerBase(output), pause_request_(false) {}

PlayerAsync::~PlayerAsync() {
  if (thread_ && thread_->joinable()) {
    thread_->join();
  }
}

void PlayerAsync::Play() {
  bool reject = false;

  {
    // cppcheck-suppress unreadVariable RAII
    std::scoped_lock lock(mutex_);
    reject = is_playing_ || (!output_) || (!file_);
    if (!reject) {
      pause_request_ = false;
      is_playing_ = true;
    }
  }

  if (reject) return;

  if (thread_ && thread_->joinable()) {
    thread_->join();
  }
  thread_.emplace([this]() { PlayerLoop(); });
}

void PlayerAsync::Pause() {
  {
    // cppcheck-suppress unreadVariable RAII
    std::scoped_lock lock(mutex_);
    pause_request_ = true;
  }

  if (thread_ && thread_->joinable()) {
    thread_->join();
  }
  thread_.reset();
}

void PlayerAsync::PlayerLoop() {
  bool finished = false;
  unsigned int track_num = 0;
  unsigned int event_num = 0;
  uint32_t dt = 0;
  auto us = std::chrono::microseconds::zero();
  float speed = 0;
  unsigned int heartbeat_interval_us = 0;

  std::function<void()> clbk_fun_heartbeat;
  std::function<void()> clbk_fun_finished;

  while (true) {
    bool pause_request = false;
    // copy player data
    {
      // cppcheck-suppress unreadVariable RAII
      std::scoped_lock lock(mutex_);
      if (!(pause_request = pause_request_)) {
        if (!(finished = PlayerBase::Finished())) {
          track_num = PlayerBase::TrackPending();
          event_num = player_state_[track_num].track_pointer_;
          dt = player_state_[track_num].track_dt_;
          us = converters::Dt2us(dt, tempo_, file_->TimeDivision());
          speed = speed_;
          heartbeat_interval_us = heartbeat_interval_us_;

          clbk_fun_heartbeat = clbk_fun_heartbeat_;
          clbk_fun_finished = clbk_fun_finished_;
        }
      }
    }

    if (pause_request || finished) {
      {
        // cppcheck-suppress unreadVariable RAII
        std::scoped_lock lock(mutex_);
        pause_request_ = false;
        is_playing_ = false;
      }

      if (finished) {
        if (clbk_fun_finished) clbk_fun_finished();
      }

      return;
    }

    // Note that the _heartbeatHelper is not protected by the _mutex.
    // It should be used only in Acynchronous::PlayerAsync::playerLoop()
    // and in PlayerImpl::goTo() (these two are exclusive).
    while ((heartbeat_helper_ + us.count()) >= heartbeat_interval_us) {
      unsigned int partial = heartbeat_interval_us - heartbeat_helper_;
      heartbeat_helper_ = 0;
      us -= std::chrono::microseconds(partial);

      // update player data before sleep
      {
        // cppcheck-suppress unreadVariable RAII
        std::scoped_lock lock(mutex_);
        pause_request = pause_request_;
        speed = speed_;

        clbk_fun_heartbeat = clbk_fun_heartbeat_;
        clbk_fun_finished = clbk_fun_finished_;
      }

      if (pause_request) {
        {
          // cppcheck-suppress unreadVariable RAII
          std::scoped_lock lock(mutex_);
          pause_request_ = false;
          is_playing_ = false;
        }
        return;
      }

      unsigned int wait = partial / speed;
      std::this_thread::sleep_for(std::chrono::microseconds(wait));
      {
        // cppcheck-suppress unreadVariable RAII
        std::scoped_lock lock(mutex_);
        played_us_ += std::chrono::microseconds(partial);
      }

      if (clbk_fun_heartbeat) clbk_fun_heartbeat();
    }

    unsigned int wait = us.count() / speed;
    std::this_thread::sleep_for(std::chrono::microseconds(wait));
    heartbeat_helper_ += us.count();

    {
      // cppcheck-suppress unreadVariable RAII
      std::scoped_lock lock(mutex_);
      played_us_ += std::chrono::microseconds(us);
      ExecEvent((*file_)[track_num][event_num]);
      UpdatePlayerState(track_num, dt);
    }
  }
}

void PlayerAsync::GoTo(const std::chrono::microseconds& pos) {
  //! @TODO If goTo is called from player's callback, different impl is needed

  bool was_playing;
  {
    // cppcheck-suppress unreadVariable RAII
    std::scoped_lock lock(mutex_);
    was_playing = is_playing_;
  }

  if (was_playing) Pause();

  {
    // cppcheck-suppress unreadVariable RAII
    std::scoped_lock lock(mutex_);
    PlayerBase::GoTo(pos);
  }

  if (was_playing) Play();
}

std::chrono::microseconds PlayerAsync::CurrentTimePos() {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  return played_us_;
}

void PlayerAsync::SetFile(const File* file) {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  PlayerBase::SetFile(file);
}

void PlayerAsync::SetOutput(output::Abstract* output) {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  output_ = output;
}

output::Abstract* PlayerAsync::output() {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  return output_;
}

bool PlayerAsync::Finished() {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  return internal::PlayerBase::Finished();
}

bool PlayerAsync::IsPlaying() {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  return is_playing_;
}

void PlayerAsync::SetSpeed(float speed) {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  speed_ = speed;
}

float PlayerAsync::Speed() {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  return speed_;
}

void PlayerAsync::SetHeartbeatInterval(unsigned int interval_us) {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  heartbeat_interval_us_ = interval_us;
}

unsigned int PlayerAsync::HeartbeatInterval() {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  return heartbeat_interval_us_;
}

void PlayerAsync::SetCallbackHeartbeat(const std::function<void()>& callback) {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  clbk_fun_heartbeat_ = callback;
}

void PlayerAsync::SetCallbackFinished(const std::function<void()>& callback) {
  // cppcheck-suppress unreadVariable RAII
  std::scoped_lock lock(mutex_);
  clbk_fun_finished_ = callback;
}

}  // namespace player
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_PLAYER_PLAYER_ASYNC_HPP_
