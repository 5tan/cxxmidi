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
  inline time::Duration duration(const File &file_);

 private:
  inline virtual void play() {}
  inline virtual void play(const File * /*file_*/) {}
  inline virtual void pause() {}
  inline virtual void resume() {}
};

}  // namespace Guts
}  // namespace CxxMidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/time/duration.hpp>

namespace cxxmidi {
namespace guts {

time::Duration Simulator::duration(const File &file_) {
  time::Duration r;

  _tempo = 500000;  // default tempo
  _file = &file_;
  this->initPlayerState();

  while (!this->finished()) {
    unsigned int trackNum = this->trackPending();
    unsigned int eventNum = _playerState[trackNum].trackPointer;
    uint32_t dt = _playerState[trackNum].trackDt;
    r.addUs(converters::dt2us(dt, _tempo, _file->timeDivision()));
    this->execEvent((*_file)[trackNum][eventNum]);
    this->updatePlayerState(trackNum, dt);
  }

  return r;
}

}  // namespace Guts
}  // namespace CxxMidi

#endif  // CXXMIDI_GUTS_SIMULATOR_HPP
