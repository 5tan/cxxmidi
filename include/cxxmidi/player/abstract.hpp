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

#ifndef CXXMIDI_PLAYER_ABSTRACT_HPP
#define CXXMIDI_PLAYER_ABSTRACT_HPP

#include <cstdint>
#include <chrono>
#include <cxxmidi/callback.hpp>

#include <vector>

#ifdef WIN32
#include <Windows.h>
#endif

#if __cplusplus > 199711L
#include <functional>
#endif  // __cplusplus > 199711L

namespace cxxmidi {
namespace output {
class Abstract;
}  // namespace output
class File;
class Event;
namespace player {

class Abstract {
 public:
  // Abstract(const Abstract&); // default is ok
  // Abstract &operator=(const Abstract &); // default is ok
#if __cplusplus > 199711L
  Abstract(Abstract&&) = default;
  Abstract& operator=(Abstract&&) = default;
#endif  // __cplusplus > 199711L

  inline Abstract();
  inline Abstract(output::Abstract* output);
  inline virtual ~Abstract();

  virtual void Play() = 0;
  virtual void Pause() = 0;

  inline void GoTo(const std::chrono::microseconds& pos);
  inline std::chrono::microseconds CurrentTimePos() const { return played_us_; }

  inline void SetFile(const File* file);
  inline void SetOutput(output::Abstract* output);
  inline output::Abstract* output() { return output_; }

  inline bool Finished() const;

  inline bool IsPlaying() const { return is_playing_; }
  inline bool IsPaused() const { return !is_playing_; }

  inline void SetSpeed(float speed) { speed_ = speed; }
  inline float Speed() const { return speed_; }

  inline void SetCallbackHeartbeat(void (*callback)(void*), void* context);
  inline void SetCallbackFinished(void (*callback)(void*), void* context);

  inline void SetCallbackHeartbeat(Callback* callback);
  inline void SetCallbackFinished(Callback* callback);
#if __cplusplus > 199711L
  inline void SetCallbackHeartbeat(const std::function<void()>& callback);
  inline void SetCallbackFinished(const std::function<void()>& callback);
#endif  // __cplusplus > 199711L

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

  // C style callbacks
  void (*clbk_fun_ptr_heartbeat_)(void*);
  void* clbk_fun_ctx_heartbeat_;
  void (*_clbkFunPtrFinished)(void*);
  void* clbk_fun_ctx_finished_;

  // C++ style callbacs
  Callback* clbk_obj_ptr_heartbeat_;
  Callback* clbk_obj_ptr_finished_;

#if __cplusplus > 199711L
  // C++11 style callbacks
  std::function<void()> clbk_fun_heartbeat_;
  std::function<void()> clbk_fun_finished_;
#endif  // __cplusplus > 199711L

 private:
  static inline void SetupWindowsTimers();
};

}  // namespace player
}  // namespace cxxmidi

#include <assert.h>

#include <cxxmidi/converters.hpp>
#include <cxxmidi/event.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/abstract.hpp>
#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace player {

Abstract::Abstract()
    : tempo_(500000),
      is_playing_(false),
      speed_(1),
      file_(0),
      output_(0),
      heartbeat_helper_(0),
      clbk_fun_ptr_heartbeat_(0),
      clbk_fun_ctx_heartbeat_(0),
      _clbkFunPtrFinished(0),
      clbk_fun_ctx_finished_(0),
      clbk_obj_ptr_heartbeat_(0),
      clbk_obj_ptr_finished_(0) {
  Abstract::SetupWindowsTimers();
}

Abstract::Abstract(output::Abstract* output)
    : tempo_(500000),
      is_playing_(false),
      speed_(1),
      file_(0),
      output_(output),
      heartbeat_helper_(0),
      clbk_fun_ptr_heartbeat_(0),
      clbk_fun_ctx_heartbeat_(0),
      _clbkFunPtrFinished(0),
      clbk_fun_ctx_finished_(0),
      clbk_obj_ptr_heartbeat_(0),
      clbk_obj_ptr_finished_(0) {
  Abstract::SetupWindowsTimers();
}

void Abstract::SetupWindowsTimers() {
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

Abstract::~Abstract() {}

void Abstract::SetFile(const File* file) {
  file_ = file;
  player_state_.clear();
  played_us_ = std::chrono::microseconds::zero();
  heartbeat_helper_ = 0;
  tempo_ = 500000;
  output_->Reset();
  this->InitPlayerState();
}

void Abstract::GoTo(const std::chrono::microseconds &pos) {
  if (!file_ || !output_) return;

  tempo_ = 500000;
  output_->Reset();
  this->InitPlayerState();
  played_us_ = std::chrono::microseconds::zero();
  heartbeat_helper_ = 0;

  while (!this->Finished()) {
    unsigned int track_mum = this->TrackPending();
    unsigned int event_num = player_state_[track_mum].track_pointer_;
    uint32_t dt = player_state_[track_mum].track_dt_;
    played_us_ += converters::Dt2us(dt, tempo_, file_->TimeDivision());

    Event event = (*file_)[track_mum][event_num];
    if (event[0] != Message::kNoteOn) this->ExecEvent(event);

    this->UpdatePlayerState(track_mum, dt);

    if (played_us_ >= pos) break;
  }
}

Abstract::PlayerStateElem::PlayerStateElem(unsigned int track_ptr,
                                           uint32_t track_dt)
    : track_pointer_(track_ptr), track_dt_(track_dt) {}

void Abstract::SetOutput(output::Abstract* output) { output_ = output; }

void Abstract::InitPlayerState() {
  if (!file_) return;

  player_state_.clear();
  for (size_t i = 0; i < file_->Tracks(); i++)
    player_state_.push_back(PlayerStateElem(0, (*file_)[i][0].Dt()));
}

bool Abstract::Finished() const {
  for (size_t i = 0; i < player_state_.size(); i++)
    if (!this->TrackFinished(i)) return false;

  return true;
}

void Abstract::SetCallbackHeartbeat(void (*callback)(void*), void* context) {
  clbk_fun_ptr_heartbeat_ = callback;
  clbk_fun_ctx_heartbeat_ = context;
}

void Abstract::SetCallbackFinished(void (*callback)(void*), void* context) {
  _clbkFunPtrFinished = callback;
  clbk_fun_ctx_finished_ = context;
}

void Abstract::SetCallbackHeartbeat(Callback* callback) {
  clbk_obj_ptr_heartbeat_ = callback;
}

void Abstract::SetCallbackFinished(Callback* callback) {
  clbk_obj_ptr_finished_ = callback;
}

#if __cplusplus > 199711L
void Abstract::SetCallbackHeartbeat(const std::function<void()>& callback) {
  clbk_fun_heartbeat_ = callback;
}

void Abstract::SetCallbackFinished(const std::function<void()>& callback) {
  clbk_fun_finished_ = callback;
}
#endif  // __cplusplus > 199711L

bool Abstract::TrackFinished(size_t track_num) const {
  return (player_state_[track_num].track_pointer_ >= (*file_)[track_num].size());
}

unsigned int Abstract::TrackPending() const {
  uint32_t dt = -1;
  size_t r = 0;

  for (size_t i = 0; i < player_state_.size(); i++)
    if (!this->TrackFinished(i))
      if (player_state_[i].track_dt_ < dt) {
        dt = player_state_[i].track_dt_;
        r = i;
      }

  return static_cast<unsigned int>(r);
}

void Abstract::UpdatePlayerState(unsigned int track_num, unsigned int dt) {
  for (size_t i = 0; i < player_state_.size(); i++)
    if (!this->TrackFinished(i)) {
      if (i == track_num) {
        unsigned int num = ++player_state_[i].track_pointer_;
        if (!this->TrackFinished(i))
          player_state_[i].track_dt_ = (*file_)[i][num].Dt();
      } else
        player_state_[i].track_dt_ -= dt;
    }
}

void Abstract::ExecEvent(const Event& event) {
  if (event.IsMeta()) {
    if (event.IsMeta(Event::Tempo))
      tempo_ = cxxmidi::utils::ExtractTempo(event[2], event[3], event[4]);

    return;  // ignore other META events (?)
  }

  if (output_) output_->SendMessage(&event);
}

}  // namespace player
}  // namespace cxxmidi

#endif  // CXXMIDI_PLAYER_ABSTRACT_HPP
