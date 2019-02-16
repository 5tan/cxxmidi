#ifndef CXXMIDI_PLAYER_ABSTRACT_HPP
#define CXXMIDI_PLAYER_ABSTRACT_HPP

#include <cstdint>
#include <cxxmidi/callback.hpp>
#include <cxxmidi/time/point.hpp>
#include <vector>

#ifdef WIN32
#include <Windows.h>
#endif

#if __cplusplus > 199711L
#include <functional>
#endif  // __cplusplus > 199711L

namespace cxxmidi {
namespace time {
class Period;
}  // namespace Time
namespace output {
class Abstract;
}  // namespace Output
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
  inline Abstract(output::Abstract* output_);
  inline virtual ~Abstract();

  virtual void Play() = 0;
  virtual void Pause() = 0;

  inline void GoTo(const time::Point& pos_);
  inline time::Point CurrentTimePos() const { return _currentTimePos; }

  inline void SetFile(const File* file_);
  inline void SetOutput(output::Abstract* output_);
  inline output::Abstract* output() { return _output; }

  inline bool Finished() const;

  inline bool IsPlaying() const { return _isPlaying; }
  inline bool IsPaused() const { return !_isPlaying; }

  inline void SetSpeed(float speed_) { _speed = speed_; }
  inline float Speed() const { return _speed; }

  inline void SetCallbackHeartbeat(void (*callback_)(void*), void* context_);
  inline void SetCallbackFinished(void (*callback_)(void*), void* context_);

  inline void SetCallbackHeartbeat(Callback* callback_);
  inline void SetCallbackFinished(Callback* callback_);
#if __cplusplus > 199711L
  inline void SetCallbackHeartbeat(const std::function<void()>& callback_);
  inline void SetCallbackFinished(const std::function<void()>& callback_);
#endif  // __cplusplus > 199711L

 protected:
  inline bool TrackFinished(size_t trackNum_) const;
  inline unsigned int TrackPending() const;

  uint32_t _tempo;  // [us per quarternote]
  bool _isPlaying;
  float _speed;
  const File* _file;
  time::Point _currentTimePos;

  inline void ExecEvent(const Event& event_);

  class PlayerStateElem {
   public:
    inline PlayerStateElem(unsigned int trackPointer_, uint32_t trackDt_);

    unsigned int trackPointer;
    uint32_t trackDt;
  };

  std::vector<PlayerStateElem> _playerState;
  inline void InitPlayerState();
  inline void UpdatePlayerState(unsigned int trackNum_, unsigned int dt_);
  output::Abstract* _output;

  int _heartbeatHelper;

  // C style callbacks
  void (*_clbkFunPtrHeartbeat)(void*);
  void* _clbkFunCtxHeartbeat;
  void (*_clbkFunPtrFinished)(void*);
  void* _clbkFunCtxFinished;

  // C++ style callbacs
  Callback* _clbkObjPtrHeartbeat;
  Callback* _clbkObjPtrFinished;

#if __cplusplus > 199711L
  // C++11 style callbacks
  std::function<void()> _clbkFunHeartbeat;
  std::function<void()> _clbkFunFinished;
#endif  // __cplusplus > 199711L

 private:
  static inline void SetupWindowsTimers();
};

}  // namespace Player
}  // namespace CxxMidi

#include <assert.h>

#include <cxxmidi/converters.hpp>
#include <cxxmidi/event.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/abstract.hpp>
#include <cxxmidi/time/period.hpp>
#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace player {

Abstract::Abstract()
    : _tempo(500000),
      _isPlaying(false),
      _speed(1),
      _file(0),
      _output(0),
      _heartbeatHelper(0),
      _clbkFunPtrHeartbeat(0),
      _clbkFunCtxHeartbeat(0),
      _clbkFunPtrFinished(0),
      _clbkFunCtxFinished(0),
      _clbkObjPtrHeartbeat(0),
      _clbkObjPtrFinished(0) {
  Abstract::SetupWindowsTimers();
}

Abstract::Abstract(output::Abstract* output_)
    : _tempo(500000),
      _isPlaying(false),
      _speed(1),
      _file(0),
      _output(output_),
      _heartbeatHelper(0),
      _clbkFunPtrHeartbeat(0),
      _clbkFunCtxHeartbeat(0),
      _clbkFunPtrFinished(0),
      _clbkFunCtxFinished(0),
      _clbkObjPtrHeartbeat(0),
      _clbkObjPtrFinished(0) {
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

void Abstract::SetFile(const File* file_) {
  _file = file_;
  _playerState.clear();
  _currentTimePos = time::Point::Zero();
  _heartbeatHelper = 0;
  _tempo = 500000;
  _output->Reset();
  this->InitPlayerState();
}

void Abstract::GoTo(const time::Point& pos_) {
  if (!_file || !_output) return;

  _tempo = 500000;
  _output->Reset();
  this->InitPlayerState();
  _currentTimePos = time::Point::Zero();
  _heartbeatHelper = 0;

  while (!this->Finished()) {
    unsigned int trackNum = this->TrackPending();
    unsigned int eventNum = _playerState[trackNum].trackPointer;
    uint32_t dt = _playerState[trackNum].trackDt;
    unsigned int us = converters::Dt2us(dt, _tempo, _file->TimeDivision());
    _currentTimePos.AddUs(us);

    Event event = (*_file)[trackNum][eventNum];
    if (event[0] != Message::NoteOn) this->ExecEvent(event);

    this->UpdatePlayerState(trackNum, dt);

    if (_currentTimePos >= pos_) break;
  }
}

Abstract::PlayerStateElem::PlayerStateElem(unsigned int trackPointer_,
                                           uint32_t trackDt_)
    : trackPointer(trackPointer_), trackDt(trackDt_) {}

void Abstract::SetOutput(output::Abstract* output_) { _output = output_; }

void Abstract::InitPlayerState() {
  if (!_file) return;

  _playerState.clear();
  for (size_t i = 0; i < _file->Tracks(); i++)
    _playerState.push_back(PlayerStateElem(0, (*_file)[i][0].Dt()));
}

bool Abstract::Finished() const {
  for (size_t i = 0; i < _playerState.size(); i++)
    if (!this->TrackFinished(i)) return false;

  return true;
}

void Abstract::SetCallbackHeartbeat(void (*callback_)(void*), void* context_) {
  _clbkFunPtrHeartbeat = callback_;
  _clbkFunCtxHeartbeat = context_;
}

void Abstract::SetCallbackFinished(void (*callback_)(void*), void* context_) {
  _clbkFunPtrFinished = callback_;
  _clbkFunCtxFinished = context_;
}

void Abstract::SetCallbackHeartbeat(Callback* callback_) {
  _clbkObjPtrHeartbeat = callback_;
}

void Abstract::SetCallbackFinished(Callback* callback_) {
  _clbkObjPtrFinished = callback_;
}

#if __cplusplus > 199711L
void Abstract::SetCallbackHeartbeat(const std::function<void()>& callback_) {
  _clbkFunHeartbeat = callback_;
}

void Abstract::SetCallbackFinished(const std::function<void()>& callback_) {
  _clbkFunFinished = callback_;
}
#endif  // __cplusplus > 199711L

bool Abstract::TrackFinished(size_t trackNum_) const {
  return (_playerState[trackNum_].trackPointer >= (*_file)[trackNum_].size());
}

unsigned int Abstract::TrackPending() const {
  uint32_t dt = -1;
  size_t r = 0;

  for (size_t i = 0; i < _playerState.size(); i++)
    if (!this->TrackFinished(i))
      if (_playerState[i].trackDt < dt) {
        dt = _playerState[i].trackDt;
        r = i;
      }

  return static_cast<unsigned int>(r);
}

void Abstract::UpdatePlayerState(unsigned int trackNum_, unsigned int dt_) {
  for (size_t i = 0; i < _playerState.size(); i++)
    if (!this->TrackFinished(i)) {
      if (i == trackNum_) {
        unsigned int num = ++_playerState[i].trackPointer;
        if (!this->TrackFinished(i))
          _playerState[i].trackDt = (*_file)[i][num].Dt();
      } else
        _playerState[i].trackDt -= dt_;
    }
}

void Abstract::ExecEvent(const Event& event_) {
  if (event_.IsMeta()) {
    if (event_.IsMeta(Event::Tempo))
      _tempo = cxxmidi::utils::ExtractTempo(event_[2], event_[3], event_[4]);

    return;  // ignore other META events (?)
  }

  if (_output) _output->SendMessage(&event_);
}

}  // namespace Player
}  // namespace CxxMidi

#endif  // CXXMIDI_PLAYER_ABSTRACT_HPP
