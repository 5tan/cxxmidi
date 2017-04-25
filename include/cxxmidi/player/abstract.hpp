#ifndef CXXMIDI_PLAYER_ABSTRACT_HPP
#define CXXMIDI_PLAYER_ABSTRACT_HPP

#include <vector>
#include <cxxmidi/guts/stdint.hpp>
#include <cxxmidi/time/point.hpp>
#include <cxxmidi/callback.hpp>

#ifdef WIN32
#   include <Windows.h>
#endif

#if __cplusplus > 199711L
#   include <functional>
#endif // __cplusplus > 199711L

namespace CxxMidi {
namespace Time {
class Period;
} // namespace Time
namespace Output {
class Abstract;
} // namespace Output
class File;
class Event;
namespace Player {

class Abstract
{
public:

    // Abstract(const Abstract&); // default is ok
    // Abstract &operator=(const Abstract &); // default is ok
#if __cplusplus > 199711L
    Abstract(Abstract&&) = default;
    Abstract& operator=(Abstract&&) = default;
#endif // __cplusplus > 199711L

    inline Abstract();
    inline Abstract(Output::Abstract *output_);
    inline virtual ~Abstract();

    virtual void play() = 0;
    virtual void pause() = 0;

    inline void goTo(const Time::Point& pos_);
    inline Time::Point currentTimePos() const { return _currentTimePos; }

    inline void setFile(const File* file_);
    inline void setOutput(Output::Abstract *output_);
    inline Output::Abstract* output() { return _output;}

    inline bool finished() const;

    inline bool isPlaying() const { return _isPlaying; }
    inline bool isPaused() const { return !_isPlaying; }

    inline void setSpeed(float speed_) { _speed = speed_; }
    inline float speed() const { return _speed; }

    inline void setCallbackHeartbeat(void(*callback_)(void *), void* context_);
    inline void setCallbackFinished(void(*callback_)(void *), void* context_);

    inline void setCallbackHeartbeat(Callback* callback_);
    inline void setCallbackFinished(Callback* callback_);
#if __cplusplus > 199711L
    inline void setCallbackHeartbeat(const std::function<void()>& callback_);
    inline void setCallbackFinished(const std::function<void()>& callback_);
#endif // __cplusplus > 199711L

protected:

    inline bool trackFinished(size_t trackNum_) const;
    inline unsigned int trackPending() const;

    uint32_t _tempo; // [us per quarternote]
    bool _isPlaying;
    float _speed;
    const File* _file;
    Time::Point _currentTimePos;

    inline void execEvent(const Event & event_);

    class PlayerStateElem
    {
    public:
        inline PlayerStateElem(unsigned int trackPointer_,
                               uint32_t trackDt_);

        unsigned int trackPointer;
        uint32_t trackDt;
    };

    std::vector<PlayerStateElem> _playerState;
    inline void initPlayerState();
    inline void updatePlayerState(unsigned int trackNum_,
                                  unsigned int dt_);
    Output::Abstract* _output;

    int _heartbeatHelper;

    // C style callbacks
    void(*_clbkFunPtrHeartbeat)(void *);
    void* _clbkFunCtxHeartbeat;
    void(*_clbkFunPtrFinished)(void *);
    void* _clbkFunCtxFinished;

    // C++ style callbacs
    Callback* _clbkObjPtrHeartbeat;
    Callback* _clbkObjPtrFinished;

#if __cplusplus > 199711L
    // C++11 style callbacks
    std::function<void()> _clbkFunHeartbeat;
    std::function<void()> _clbkFunFinished;
#endif // __cplusplus > 199711L

private:
    static inline void setupWindowsTimers();
};

} // namespace Player
} // namespace CxxMidi

#include <assert.h>

#include <cxxmidi/utils.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/event.hpp>
#include <cxxmidi/output/abstract.hpp>
#include <cxxmidi/converters.hpp>
#include <cxxmidi/time/period.hpp>

namespace CxxMidi {
namespace Player {

Abstract::Abstract()
    : _tempo(500000)
    , _isPlaying(false)
    , _speed(1)
    , _file(0)
    , _output(0)
    , _heartbeatHelper(0)
    , _clbkFunPtrHeartbeat(0)
    , _clbkFunCtxHeartbeat(0)
    , _clbkFunPtrFinished(0)
    , _clbkFunCtxFinished(0)
    , _clbkObjPtrHeartbeat(0)
    , _clbkObjPtrFinished(0)
{
    Abstract::setupWindowsTimers();
}

Abstract::Abstract(Output::Abstract* output_)
    : _tempo(500000)
    , _isPlaying(false)
    , _speed(1)
    , _file(0)
    , _output(output_)
    , _heartbeatHelper(0)
    , _clbkFunPtrHeartbeat(0)
    , _clbkFunCtxHeartbeat(0)
    , _clbkFunPtrFinished(0)
    , _clbkFunCtxFinished(0)
    , _clbkObjPtrHeartbeat(0)
    , _clbkObjPtrFinished(0)
{
    Abstract::setupWindowsTimers();
}

void Abstract::setupWindowsTimers()
{
#ifdef WIN32
    static bool once = true;
    if(once)
    {
        TIMECAPS tc;
        UINT     wTimerRes;

        if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            wTimerRes = min(max(tc.wPeriodMin, 1 /* [ms] */), tc.wPeriodMax);
            timeBeginPeriod(wTimerRes);
        }

        once = false;
    }
#endif
}

Abstract::~Abstract() {}

void Abstract::setFile(const File* file_)
{
    _file = file_;
    _playerState.clear();
    _currentTimePos = Time::Point::zero();
    _heartbeatHelper = 0;
    _tempo = 500000;
    _output->reset();
    this->initPlayerState();
}

void Abstract::goTo(const Time::Point & pos_)
{
    if(!_file || !_output)
        return;

    _tempo = 500000;
    _output->reset();
    this->initPlayerState();
    _currentTimePos = Time::Point::zero();
    _heartbeatHelper = 0;

    while(!this->finished())
    {
        unsigned int trackNum = this->trackPending();
        unsigned int eventNum = _playerState[trackNum].trackPointer;
        uint32_t dt = _playerState[trackNum].trackDt;
        unsigned int us = Converters::dt2us(dt,_tempo,_file->timeDivision());
        _currentTimePos.addUs(us);

        Event event = (*_file)[trackNum][eventNum];
        if( event[0] != Message::NoteOn)
            this->execEvent(event);

        this->updatePlayerState(trackNum,dt);

        if(_currentTimePos >= pos_)
            break;
    }
}

Abstract::PlayerStateElem::PlayerStateElem(unsigned int trackPointer_,
                                           uint32_t trackDt_)
    : trackPointer(trackPointer_)
    , trackDt(trackDt_) {}

void Abstract::setOutput(Output::Abstract* output_)
{
    _output = output_;
}

void Abstract::initPlayerState()
{
    if(!_file)
        return;

    _playerState.clear();
    for(size_t i=0;i<_file->tracks();i++)
        _playerState.push_back(PlayerStateElem(0, (*_file)[i][0].dt()));
}

bool Abstract::finished() const
{
    for(size_t i=0;i<_playerState.size();i++)
        if(!this->trackFinished(i))
            return false;

    return true;
}

void Abstract::setCallbackHeartbeat(void(*callback_)(void *),
                                           void* context_)
{
    _clbkFunPtrHeartbeat = callback_;
    _clbkFunCtxHeartbeat = context_;
}

void Abstract::setCallbackFinished(void(*callback_)(void *)
                                          , void* context_)
{
    _clbkFunPtrFinished = callback_;
    _clbkFunCtxFinished = context_;
}

void Abstract::setCallbackHeartbeat(Callback* callback_)
{
    _clbkObjPtrHeartbeat = callback_;
}

void Abstract::setCallbackFinished(Callback* callback_)
{
    _clbkObjPtrFinished = callback_;
}

#if __cplusplus > 199711L
void Abstract::setCallbackHeartbeat(const std::function<void()>& callback_)
{
    _clbkFunHeartbeat = callback_;
}

void Abstract::setCallbackFinished(const std::function<void()>& callback_)
{
    _clbkFunFinished = callback_;
}
#endif // __cplusplus > 199711L

bool Abstract::trackFinished(size_t trackNum_) const
{
    return (_playerState[trackNum_].trackPointer >= (*_file)[trackNum_].size());
}

unsigned int Abstract::trackPending() const
{
    uint32_t dt = -1;
    size_t r=0;

    for(size_t i=0;i<_playerState.size();i++)
        if(!this->trackFinished(i))
            if(_playerState[i].trackDt < dt )
            {
                dt = _playerState[i].trackDt;
                r = i;
            }

    return static_cast<unsigned int>(r);
}

void Abstract::updatePlayerState(unsigned int trackNum_,
                                 unsigned int dt_)
{
    for(size_t i=0;i<_playerState.size();i++)
        if(!this->trackFinished(i))
        {
            if(i==trackNum_)
            {
                unsigned int num = ++_playerState[i].trackPointer;
                if(!this->trackFinished(i))
                    _playerState[i].trackDt = (*_file)[i][num].dt();
            }
            else
                _playerState[i].trackDt -= dt_;
        }
}

void Abstract::execEvent(const Event &event_)
{
    if( event_.isMeta() )
    {
        if( event_.isMeta(Event::Tempo) )
            _tempo = CxxMidi::Utils::extractTempo(
                        event_[2],
                    event_[3],
                    event_[4]);

        return; // ignore other META events (?)
    }

    if(_output)
        _output->sendMessage(&event_);
}

} // namespace Player
} // namespace CxxMidi

#endif // CXXMIDI_PLAYER_ABSTRACT_HPP
