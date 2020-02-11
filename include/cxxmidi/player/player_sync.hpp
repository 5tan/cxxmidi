
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

#ifndef INCLUDE_CXXMIDI_PLAYER_PLAYER_SYNC_HPP_
#define INCLUDE_CXXMIDI_PLAYER_PLAYER_SYNC_HPP_

#include <assert.h>
#include <thread>  // NOLINT() CPP11_INCLUDES

#include <cxxmidi/guts/player_base.hpp>

namespace cxxmidi {
class File;
namespace player {

class PlayerSync : public guts::PlayerBase {
 public:
  inline explicit PlayerSync(output::Abstract* output);

  inline void Play();

 private:
  inline void PlayerLoop();
};

}  // namespace player
}  // namespace cxxmidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/guts/utils.hpp>

namespace cxxmidi {
namespace player {

PlayerSync::PlayerSync(output::Abstract* output) : guts::PlayerBase(output) {}

void PlayerSync::Play() {
  if (!output_ || !file_) return;

  PlayerLoop();
}

void PlayerSync::PlayerLoop() {
  while (!Finished()) {
    unsigned int track_num = TrackPending();
    unsigned int event_num = player_state_[track_num].track_pointer_;
    uint32_t dt = player_state_[track_num].track_dt_;
    auto us = converters::Dt2us(dt, tempo_, file_->TimeDivision());

    while ((heartbeat_helper_ + us.count()) >= 10000) {
      unsigned int partial = 10000 - heartbeat_helper_;
      heartbeat_helper_ = 0;
      us -= std::chrono::microseconds(partial);

      unsigned int wait = partial / speed_;
      std::this_thread::sleep_for(std::chrono::microseconds(wait));
      played_us_ += std::chrono::microseconds(partial);

      if (clbk_fun_heartbeat_) clbk_fun_heartbeat_();
    }

    unsigned int wait = us.count() / speed_;
    std::this_thread::sleep_for(std::chrono::microseconds(wait));
    heartbeat_helper_ += us.count();
    played_us_ += std::chrono::microseconds(us);
    ExecEvent((*file_)[track_num][event_num]);
    UpdatePlayerState(track_num, dt);
  }

  if (clbk_fun_finished_) clbk_fun_finished_();
}

}  // namespace player
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_PLAYER_PLAYER_SYNC_HPP_
