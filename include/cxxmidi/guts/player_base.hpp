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

#ifndef INCLUDE_CXXMIDI_GUTS_PLAYER_BASE_HPP_
#define INCLUDE_CXXMIDI_GUTS_PLAYER_BASE_HPP_

#ifdef WIN32
#include <Windows.h>
#endif
#include <assert.h>

#include <algorithm>
#include <chrono>  // NOLINT() CPP11_INCLUDES
#include <cstdint>
#include <functional>
#include <vector>

namespace cxxmidi {
namespace output {
class Abstract;
}  // namespace output
class File;
class Event;
namespace guts {

class PlayerBase {
 public:
  inline PlayerBase();
  inline explicit PlayerBase(output::Abstract* output);
  // no virtual destructor: non-poymorphic class

  inline void GoTo(const std::chrono::microseconds& pos);
  inline std::chrono::microseconds CurrentTimePos() const { return played_us_; }

  inline void SetFile(const File* file);
  inline void SetOutput(output::Abstract* output);
  inline output::Abstract* output() { return output_; }

  inline bool Finished() const;

  inline bool IsPlaying() const { return is_playing_; }

  inline void SetSpeed(float speed) { speed_ = speed; }
  inline float GetSpeed() const { return speed_; }

  inline void SetCallbackHeartbeat(const std::function<void()>& callback);
  inline void SetCallbackFinished(const std::function<void()>& callback);

 protected:
  inline bool TrackFinished(size_t track_num) const;
  inline unsigned int TrackPending() const;

  uint32_t tempo_;  // [us per quarternote]
  bool is_playing_;
  float speed_;
  const File* file_;
  std::chrono::microseconds played_us_;

  inline void ExecEvent(const Event& event);

  class PlayerStateElem {
   public:
    inline PlayerStateElem(unsigned int track_ptr, uint32_t track_dt);

    unsigned int track_pointer_;
    uint32_t track_dt_;
  };

  std::vector<PlayerStateElem> player_state_;
  inline void InitPlayerState();
  inline void UpdatePlayerState(unsigned int track_num, unsigned int dt);
  output::Abstract* output_;

  int heartbeat_helper_;

  std::function<void()> clbk_fun_heartbeat_;
  std::function<void()> clbk_fun_finished_;

 private:
  static inline void SetupWindowsTimers();
};

}  // namespace guts
}  // namespace cxxmidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/event.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/guts/utils.hpp>
#include <cxxmidi/output/abstract.hpp>

namespace cxxmidi {
namespace guts {

PlayerBase::PlayerBase()
    : tempo_(500000),
      is_playing_(false),
      speed_(1.),
      file_(nullptr),
      played_us_(std::chrono::microseconds(0)),
      output_(0),
      heartbeat_helper_(0) {
  PlayerBase::SetupWindowsTimers();
}

PlayerBase::PlayerBase(output::Abstract* output)
    : tempo_(500000),
      is_playing_(false),
      speed_(1.),
      file_(nullptr),
      played_us_(std::chrono::microseconds(0)),
      output_(output),
      heartbeat_helper_(0) {
  PlayerBase::SetupWindowsTimers();
}

void PlayerBase::SetupWindowsTimers() {
#ifdef WIN32
  static bool once = true;
  if (once) {
    TIMECAPS tc;
    UINT wTimerRes;

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR) {
      wTimerRes = min(max(tc.wPeriodMin, 1 /* [ms] */), tc.wPeriodMax);
      timeBeginPeriod(wTimerRes);
    }

    once = false;
  }
#endif
}

void PlayerBase::SetFile(const File* file) {
  file_ = file;
  player_state_.clear();
  played_us_ = std::chrono::microseconds::zero();
  heartbeat_helper_ = 0;
  tempo_ = 500000;
  output_->Reset();
  InitPlayerState();
}

void PlayerBase::GoTo(const std::chrono::microseconds& pos) {
  if (!file_ || !output_) return;

  tempo_ = 500000;
  output_->Reset();
  InitPlayerState();
  played_us_ = std::chrono::microseconds::zero();
  heartbeat_helper_ = 0;

  while (!Finished()) {
    unsigned int track_mum = TrackPending();
    unsigned int event_num = player_state_[track_mum].track_pointer_;
    uint32_t dt = player_state_[track_mum].track_dt_;
    played_us_ += converters::Dt2us(dt, tempo_, file_->TimeDivision());

    Event event = (*file_)[track_mum][event_num];
    if (!event.IsVoiceCategory(Message::kNoteOn) &&
        !event.IsVoiceCategory(Message::kNoteOff))
      ExecEvent(event);

    UpdatePlayerState(track_mum, dt);

    if (played_us_ >= pos) break;
  }
}

PlayerBase::PlayerStateElem::PlayerStateElem(unsigned int track_ptr,
                                             uint32_t track_dt)
    : track_pointer_(track_ptr), track_dt_(track_dt) {}

void PlayerBase::SetOutput(output::Abstract* output) { output_ = output; }

void PlayerBase::InitPlayerState() {
  if (!file_) return;

  player_state_.clear();
  for (size_t i = 0; i < file_->Tracks(); i++)
    player_state_.push_back(PlayerStateElem(0, (*file_)[i][0].Dt()));
}

bool PlayerBase::Finished() const {
  for (size_t i = 0; i < player_state_.size(); i++)
    if (!TrackFinished(i)) return false;

  return true;
}

void PlayerBase::SetCallbackHeartbeat(const std::function<void()>& callback) {
  clbk_fun_heartbeat_ = callback;
}

void PlayerBase::SetCallbackFinished(const std::function<void()>& callback) {
  clbk_fun_finished_ = callback;
}

bool PlayerBase::TrackFinished(size_t track_num) const {
  return (player_state_[track_num].track_pointer_ >=
          (*file_)[track_num].size());
}

unsigned int PlayerBase::TrackPending() const {
  uint32_t dt = -1;
  size_t r = 0;

  for (size_t i = 0; i < player_state_.size(); i++) {
    if (!TrackFinished(i)) {
      if (player_state_[i].track_dt_ < dt) {
        dt = player_state_[i].track_dt_;
        r = i;
      }
    }
  }

  return static_cast<unsigned int>(r);
}

void PlayerBase::UpdatePlayerState(unsigned int track_num, unsigned int dt) {
  for (size_t i = 0; i < player_state_.size(); i++)
    if (!TrackFinished(i)) {
      if (i == track_num) {
        unsigned int num = ++player_state_[i].track_pointer_;
        if (!TrackFinished(i)) {
          player_state_[i].track_dt_ = (*file_)[i][num].Dt();
        }
      } else {
        player_state_[i].track_dt_ -= dt;
      }
    }
}

void PlayerBase::ExecEvent(const Event& event) {
  if (event.IsMeta()) {
    if (event.IsMeta(Event::kTempo))
      tempo_ = cxxmidi::utils::ExtractTempo(event[2], event[3], event[4]);

    return;  // ignore other META events (?)
  }

  if (output_) output_->SendMessage(&event);
}

}  // namespace guts
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_GUTS_PLAYER_BASE_HPP_
