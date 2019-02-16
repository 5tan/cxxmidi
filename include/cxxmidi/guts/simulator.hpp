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

  _tempo = 500000;  // default tempo
  _file = &file;
  this->InitPlayerState();

  while (!this->Finished()) {
    unsigned int trackNum = this->TrackPending();
    unsigned int eventNum = _playerState[trackNum].trackPointer;
    uint32_t dt = _playerState[trackNum].trackDt;
    r.AddUs(converters::Dt2us(dt, _tempo, _file->TimeDivision()));
    this->ExecEvent((*_file)[trackNum][eventNum]);
    this->UpdatePlayerState(trackNum, dt);
  }

  return r;
}

}  // namespace Guts
}  // namespace CxxMidi

#endif  // CXXMIDI_GUTS_SIMULATOR_HPP
