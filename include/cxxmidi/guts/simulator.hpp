#ifndef CXXMIDI_GUTS_SIMULATOR_HPP
#define CXXMIDI_GUTS_SIMULATOR_HPP

#include <cxxmidi/player/abstract.hpp>

namespace cxxmidi {
namespace time {
class Duration;
}
namespace guts {

class Simulator : public player::Abstract {
 public:
  inline time::Duration Duration(const File &file);
  inline std::chrono::microseconds Duration2(const File &file);

 private:
  inline virtual void Play() {}
  inline virtual void Play(const File * /*file*/) {}
  inline virtual void Pause() {}
  inline virtual void Resume() {}
};

}  // namespace Guts
}  // namespace CxxMidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/time/duration.hpp>

namespace cxxmidi {
namespace guts {

time::Duration Simulator::Duration(const File &file) {
  time::Duration r;

  tempo_ = 500000;  // default tempo
  file_ = &file;
  this->InitPlayerState();

  while (!this->Finished()) {
    unsigned int trackNum = this->TrackPending();
    unsigned int eventNum = player_state_[trackNum].track_pointer_;
    uint32_t dt = player_state_[trackNum].track_dt_;
    r.AddUs(converters::Dt2us(dt, tempo_, file_->TimeDivision()));
    this->ExecEvent((*file_)[trackNum][eventNum]);
    this->UpdatePlayerState(trackNum, dt);
  }

  return r;
}

std::chrono::microseconds Simulator::Duration2(const File &file) {
  auto r = std::chrono::microseconds::zero();

  tempo_ = 500000;  // default tempo
  file_ = &file;
  this->InitPlayerState();

  while (!this->Finished()) {
    unsigned int trackNum = this->TrackPending();
    unsigned int eventNum = player_state_[trackNum].track_pointer_;
    uint32_t dt = player_state_[trackNum].track_dt_;
    r += converters::Dt2us2(dt, tempo_, file_->TimeDivision());
    this->ExecEvent((*file_)[trackNum][eventNum]);
    this->UpdatePlayerState(trackNum, dt);
  }

  return r;
}

}  // namespace Guts
}  // namespace CxxMidi

#endif  // CXXMIDI_GUTS_SIMULATOR_HPP
