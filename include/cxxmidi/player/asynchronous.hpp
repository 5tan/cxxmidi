#ifndef CXXMIDI_PLAYER_ASYNCHRONOUS_HPP
#define CXXMIDI_PLAYER_ASYNCHRONOUS_HPP

#include <cxxmidi/player/abstract.hpp>
#include <cxxmidi/guts/thread.hpp>
#include <cxxmidi/guts/mutex.hpp>

namespace CxxMidi {
class File;
namespace Player {

class Asynchronous : private Player::Abstract
{

public:
    inline Asynchronous();
    inline Asynchronous(Output::Abstract *output_);
    inline virtual ~Asynchronous();

    inline virtual void play();
    inline virtual void pause();

    inline void goTo(const Time::Point& pos_);
    inline Time::Point currentTimePos();

    inline void setFile(const File* file_);
    inline void setOutput(Output::Abstract *output_);
    inline Output::Abstract* output();

    inline bool finished();

    inline bool isPlaying();
    inline bool isPaused();

    inline void setSpeed(float speed_);
    inline float speed();

    inline void setCallbackHeartbeat(Callback<Player::Abstract> *callbackHeartbeat_);
    inline void setCallbackFinished(Callback<Player::Abstract> *callbackFinished_);

private:

    bool _pauseRequest;

    inline static void* playerLoop(void *caller_);

    CxxMidi::Guts::Mutex _mutex;
    CxxMidi::Guts::Thread _thread;
};

} // Player
} // CxxMidi

#include <assert.h>

#include <cxxmidi/utils.hpp>
#include <cxxmidi/sleep.hpp>
#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>

namespace CxxMidi {
namespace Player {

Asynchronous::Asynchronous(Output::Abstract* output_)
    : Abstract(output_)
    , _pauseRequest(false)
{
}

Asynchronous::~Asynchronous()
{
}

void Asynchronous::play()
{
    bool reject = false;

    _mutex.lock();
    reject = _isPlaying || (!_output) || (!_file);
    if(!reject)
    {
        _pauseRequest = false;
        _isPlaying = true;
    }
    _mutex.unlock();

    if(reject)
        return;

    _thread = CxxMidi::Guts::Thread(Asynchronous::playerLoop,this);
}

void Asynchronous::pause()
{
    _mutex.lock();
    _pauseRequest = true;
    _mutex.unlock();

    _thread.join();
}


void *Asynchronous::playerLoop(void * caller_)
{
    Asynchronous* that = reinterpret_cast<Asynchronous*>(caller_);

    bool finished = false;
    bool pauseRequest = false;
    unsigned int trackNum = 0;
    unsigned int eventNum = 0;
    uint32_t dt = 0;
    unsigned int us = 0;
    float speed = 0;
    Callback<Player::Abstract>* callbackHeartbeat;
    Callback<Player::Abstract>* callbackFinished;

    while(true)
    {
        // copy player data
        that-> _mutex.lock();
        if(!(pauseRequest = that->_pauseRequest))
        {
            if(!(finished = that->Abstract::finished()))
            {
                trackNum = that->Abstract::trackPending();
                eventNum = that->_playerState[trackNum].trackPointer;
                dt = that->_playerState[trackNum].trackDt;
                us = Converters::dt2us(dt,that->_tempo,that->_file->timeDivision());
                speed = that->_speed;
                callbackHeartbeat = that->_callbackHeartbeat;
                callbackFinished = that->_callbackFinished;
            }
        }
        that-> _mutex.unlock();

        if(pauseRequest || finished)
        {
            that->_mutex.lock();
            that->_pauseRequest = false;
            that->_isPlaying = false;
            that->_mutex.unlock();

            if(finished)
                if(callbackFinished)
                    (*callbackFinished)(that);

            return 0;
        }

        // Note that the _heartbeatHelper is not protected by the _mutex.
        // It should be used only in Acynchronous::Asynchronous::playerLoop()
        // and in Abstract::goTo() (these two are exclusive).
        while ( (that->_heartbeatHelper + us) >= 10000)
        {
            unsigned int partial = 10000 - that->_heartbeatHelper;
            that->_heartbeatHelper =0;
            us -= partial;

            // update player data before sleep
            that->_mutex.lock();
            pauseRequest = that->_pauseRequest;
            speed = that->_speed;
            callbackHeartbeat = that->_callbackHeartbeat;
            callbackFinished = that->_callbackFinished;
            that->_mutex.unlock();

            if(pauseRequest)
            {
                that->_mutex.lock();
                that->_pauseRequest = false;
                that->_isPlaying = false;
                that->_mutex.unlock();
                return 0;
            }

            Sleep::us(partial/speed);
            that->_mutex.lock();
            that->_currentTimePos.addUs(partial);
            that->_mutex.unlock();

            if(callbackHeartbeat)
                (*callbackHeartbeat)(that);
        }

        Sleep::us(us/speed);
        that->_heartbeatHelper += us;

        that->_mutex.lock();
        that->_currentTimePos.addUs(us);
        that->execEvent((*that->_file)[trackNum][eventNum]);
        that->updatePlayerState(trackNum,dt);
        that->_mutex.unlock();
    }

    return 0;
}

void Asynchronous::goTo(const Time::Point& pos_)
{
    _mutex.lock();
    bool wasPlaying = _isPlaying;
    _mutex.unlock();

    if(wasPlaying)
        this->pause();

    _mutex.lock();
    Abstract::goTo(pos_);
    _mutex.unlock();

    if(wasPlaying)
        this->play();
}

Time::Point Asynchronous::currentTimePos()
{
    Time::Point r;
    _mutex.lock();
    r = _currentTimePos;
    _mutex.unlock();
    return r;
}

void Asynchronous::setFile(const File* file_)
{
    _mutex.lock();
    Abstract::setFile(file_);
    _mutex.unlock();
}

void Asynchronous::setOutput(Output::Abstract *output_)
{
    _mutex.lock();
    _output = output_;
    _mutex.unlock();
}

Output::Abstract* Asynchronous::output()
{
    Output::Abstract* r;
    _mutex.lock();
    r = _output;
    _mutex.unlock();
    return r;
}

bool Asynchronous::finished()
{
    bool r;
    _mutex.lock();
    r = Player::Abstract::finished();
    _mutex.unlock();
    return r;
}

bool Asynchronous::isPlaying()
{
    bool r;
    _mutex.lock();
    r = _isPlaying;
    _mutex.unlock();
    return r;
}

bool Asynchronous::isPaused()
{
    bool r;
    _mutex.lock();
    r = !_isPlaying;
    _mutex.unlock();
    return r;
}

void Asynchronous::setSpeed(float speed_)
{
    _mutex.lock();
    _speed = speed_;
    _mutex.unlock();
}

float Asynchronous::speed()
{
    float r;
    _mutex.lock();
    r = _speed;
    _mutex.unlock();

    return r;
}

void Asynchronous::setCallbackHeartbeat(Callback<Player::Abstract>* callbackHeartbeat_)
{
    _mutex.lock();
    _callbackHeartbeat = callbackHeartbeat_;
    _mutex.unlock();
}

void Asynchronous::setCallbackFinished(Callback<Player::Abstract> *callbackFinished_)
{
    _mutex.lock();
    _callbackFinished = callbackFinished_;
    _mutex.unlock();
}

} // Player
} // CxxMidi

#endif // CXXMIDI_PLAYER_ASYNCHRONOUS_HPP
