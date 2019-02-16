#ifndef CXXMIDI_PLAYER_SYNCHRONOUS_HPP
#define CXXMIDI_PLAYER_SYNCHRONOUS_HPP

#include <cxxmidi/player/abstract.hpp>

namespace cxxmidi {
class File;
namespace player {

class Synchronous : public player::Abstract {
 public:
  inline Synchronous(output::Abstract* output);
  inline virtual ~Synchronous();

  // Synchronous(const Synchronous&); // default is ok
  // Synchronous &operator=(const Synchronous &); // default is ok
#if __cplusplus > 199711L
  Synchronous(Synchronous&&) = default;
  Synchronous& operator=(Synchronous&&) = default;
#endif  // __cplusplus > 199711L

  inline virtual void Play();

 private:
  inline void PlayerLoop();
  inline virtual void Pause() {}
};

}  // namespace Player
}  // namespace CxxMidi

#include <assert.h>

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/sleep.hpp>
#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace player {

Synchronous::Synchronous(output::Abstract* output) : Abstract(output) {}

Synchronous::~Synchronous() {}

void Synchronous::Play() {
  if (!_output || !_file) return;

  this->PlayerLoop();
}

void Synchronous::PlayerLoop() {
  while (!this->Finished()) {
    unsigned int trackNum = this->TrackPending();
    unsigned int eventNum = _playerState[trackNum].trackPointer;
    uint32_t dt = _playerState[trackNum].trackDt;
    unsigned int us = converters::Dt2us(dt, _tempo, _file->TimeDivision());

    while ((_heartbeatHelper + us) >= 10000) {
      unsigned int partial = 10000 - _heartbeatHelper;
      _heartbeatHelper = 0;
      us -= partial;

      sleep::SleepUs(partial / _speed);
      _currentTimePos.AddUs(partial);

      if (_clbkFunPtrHeartbeat) (*_clbkFunPtrHeartbeat)(_clbkFunCtxHeartbeat);

      if (_clbkObjPtrHeartbeat) (*_clbkObjPtrHeartbeat)();

#if __cplusplus > 199711L
      if (_clbkFunHeartbeat) _clbkFunHeartbeat();
#endif  // __cplusplus > 199711L
    }

    sleep::SleepUs(us / _speed);
    _heartbeatHelper += us;
    _currentTimePos.AddUs(us);
    this->ExecEvent((*_file)[trackNum][eventNum]);
    this->UpdatePlayerState(trackNum, dt);
  }

  if (_clbkFunPtrFinished) (*_clbkFunPtrFinished)(_clbkFunCtxFinished);

  if (_clbkObjPtrFinished) (*_clbkObjPtrFinished)();

#if __cplusplus > 199711L
  if (_clbkFunFinished) _clbkFunFinished();
#endif  // __cplusplus > 199711L
}

}  // namespace Player
}  // namespace CxxMidi

#endif  // CXXMIDI_PLAYER_SYNCHRONOUS_HPP
