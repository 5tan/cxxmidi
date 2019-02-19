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

#ifndef INCLUDE_CXXMIDI_GUTS_SIMULATOR_HPP_
#define INCLUDE_CXXMIDI_GUTS_SIMULATOR_HPP_

#include <cxxmidi/player/abstract.hpp>

namespace cxxmidi {
namespace time {
class Duration;
}
namespace guts {

class Simulator : public player::Abstract {
 public:
  inline std::chrono::microseconds Duration(const File &file);

 private:
  inline virtual void Play() {}
  inline virtual void Play(const File * /*file*/) {}
  inline virtual void Pause() {}
  inline virtual void Resume() {}
};

}  // namespace guts
}  // namespace cxxmidi

#include <cxxmidi/converters.hpp>


namespace cxxmidi {
namespace guts {

std::chrono::microseconds Simulator::Duration(const File &file) {
  auto r = std::chrono::microseconds::zero();

  tempo_ = 500000;  // default tempo
  file_ = &file;
  this->InitPlayerState();

  while (!this->Finished()) {
    unsigned int trackNum = this->TrackPending();
    unsigned int eventNum = player_state_[trackNum].track_pointer_;
    uint32_t dt = player_state_[trackNum].track_dt_;
    r += converters::Dt2us(dt, tempo_, file_->TimeDivision());
    this->ExecEvent((*file_)[trackNum][eventNum]);
    this->UpdatePlayerState(trackNum, dt);
  }

  return r;
}

}  // namespace guts
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_GUTS_SIMULATOR_HPP_
