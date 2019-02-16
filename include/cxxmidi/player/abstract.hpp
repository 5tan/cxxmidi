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
  inline Abstract(output::Abstract* output);
  inline virtual ~Abstract();

  virtual void Play() = 0;
  virtual void Pause() = 0;

  inline void GoTo(const time::Point& pos);
  inline time::Point CurrentTimePos() const { return _currentTimePos; }

  inline void SetFile(const File* file);
  inline void SetOutput(output::Abstract* output);
  inline output::Abstract* output() { return _output; }

  inline bool Finished() const;

  inline bool IsPlaying() const { return _isPlaying; }
  inline bool IsPaused() const { return !_isPlaying; }

  inline void SetSpeed(float speed) { _speed = speed; }
  inline float Speed() const { return _speed; }

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

  uint32_t _tempo;  // [us per quarternote]
  bool _isPlaying;
  float _speed;
  const File* _file;
  time::Point _currentTimePos;

  inline void ExecEvent(const Event& event);

  class PlayerStateElem {
   public:
    inline PlayerStateElem(unsigned int track_ptr, uint32_t track_dt);

    unsigned int trackPointer;
    uint32_t trackDt;
  };

  std::vector<PlayerStateElem> _playerState;
  inline void InitPlayerState();
  inline void UpdatePlayerState(unsigned int track_num, unsigned int dt);
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

Abstract::Abstract(output::Abstract* output)
    : _tempo(500000),
      _isPlaying(false),
      _speed(1),
      _file(0),
      _output(output),
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

void Abstract::SetFile(const File* file) {
  _file = file;
  _playerState.clear();
  _currentTimePos = time::Point::Zero();
  _heartbeatHelper = 0;
  _tempo = 500000;
  _output->Reset();
  this->InitPlayerState();
}

void Abstract::GoTo(const time::Point& pos) {
  if (!_file || !_output) return;

  _tempo = 500000;
  _output->Reset();
  this->InitPlayerState();
  _currentTimePos = time::Point::Zero();
  _heartbeatHelper = 0;

  while (!this->Finished()) {
    unsigned int track_mum = this->TrackPending();
    unsigned int event_num = _playerState[track_mum].trackPointer;
    uint32_t dt = _playerState[track_mum].trackDt;
    unsigned int us = converters::Dt2us(dt, _tempo, _file->TimeDivision());
    _currentTimePos.AddUs(us);

    Event event = (*_file)[track_mum][event_num];
    if (event[0] != Message::NoteOn) this->ExecEvent(event);

    this->UpdatePlayerState(track_mum, dt);

    if (_currentTimePos >= pos) break;
  }
}

Abstract::PlayerStateElem::PlayerStateElem(unsigned int track_ptr,
                                           uint32_t track_dt)
    : trackPointer(track_ptr), trackDt(track_dt) {}

void Abstract::SetOutput(output::Abstract* output) { _output = output; }

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

void Abstract::SetCallbackHeartbeat(void (*callback)(void*), void* context) {
  _clbkFunPtrHeartbeat = callback;
  _clbkFunCtxHeartbeat = context;
}

void Abstract::SetCallbackFinished(void (*callback)(void*), void* context) {
  _clbkFunPtrFinished = callback;
  _clbkFunCtxFinished = context;
}

void Abstract::SetCallbackHeartbeat(Callback* callback) {
  _clbkObjPtrHeartbeat = callback;
}

void Abstract::SetCallbackFinished(Callback* callback) {
  _clbkObjPtrFinished = callback;
}

#if __cplusplus > 199711L
void Abstract::SetCallbackHeartbeat(const std::function<void()>& callback) {
  _clbkFunHeartbeat = callback;
}

void Abstract::SetCallbackFinished(const std::function<void()>& callback) {
  _clbkFunFinished = callback;
}
#endif  // __cplusplus > 199711L

bool Abstract::TrackFinished(size_t track_num) const {
  return (_playerState[track_num].trackPointer >= (*_file)[track_num].size());
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

void Abstract::UpdatePlayerState(unsigned int track_num, unsigned int dt) {
  for (size_t i = 0; i < _playerState.size(); i++)
    if (!this->TrackFinished(i)) {
      if (i == track_num) {
        unsigned int num = ++_playerState[i].trackPointer;
        if (!this->TrackFinished(i))
          _playerState[i].trackDt = (*_file)[i][num].Dt();
      } else
        _playerState[i].trackDt -= dt;
    }
}

void Abstract::ExecEvent(const Event& event) {
  if (event.IsMeta()) {
    if (event.IsMeta(Event::Tempo))
      _tempo = cxxmidi::utils::ExtractTempo(event[2], event[3], event[4]);

    return;  // ignore other META events (?)
  }

  if (_output) _output->SendMessage(&event);
}

}  // namespace Player
}  // namespace CxxMidi

#endif  // CXXMIDI_PLAYER_ABSTRACT_HPP
