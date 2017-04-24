#ifndef CXXMIDI_PLAYER_SYNCHRONOUS_HPP
#define CXXMIDI_PLAYER_SYNCHRONOUS_HPP

#include <cxxmidi/player/abstract.hpp>

namespace CxxMidi {
class File;
namespace Player {

class Synchronous : public Player::Abstract
{
public:
    inline Synchronous(Output::Abstract *output_);
    inline virtual ~Synchronous();

    // Synchronous(const Synchronous&); // default is ok
    // Synchronous &operator=(const Synchronous &); // default is ok
#if __cplusplus > 199711L
    Synchronous(Synchronous&&) = default;
    Synchronous& operator=(Synchronous&&) = default;
#endif // __cplusplus > 199711L

    inline virtual void play() ;

private:

    inline void playerLoop();
    inline virtual void pause(){}

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

Synchronous::Synchronous(Output::Abstract* output_)
    : Abstract(output_)
{
}

Synchronous::~Synchronous()
{
}

void Synchronous::play()
{
    if(!_output || !_file)
        return;

    this->playerLoop();
}

void Synchronous::playerLoop()
{

    while(!this->finished())
    {
        unsigned int trackNum = this->trackPending();
        unsigned int eventNum = _playerState[trackNum].trackPointer;
        uint32_t dt = _playerState[trackNum].trackDt;
        unsigned int us = Converters::dt2us(dt,_tempo,_file->timeDivision());

        while ( (_heartbeatHelper + us) >= 10000)
        {
            unsigned int partial = 10000 - _heartbeatHelper;
            _heartbeatHelper =0;
            us -= partial;

            Sleep::us(partial/_speed);
            _currentTimePos.addUs(partial);

            if(_clbkFunPtrHeartbeat)
                (*_clbkFunPtrHeartbeat)(_clbkFunCtxHeartbeat);

            if(_clbkObjPtrHeartbeat)
                (*_clbkObjPtrHeartbeat)();

#if __cplusplus > 199711L
            if(_clbkFunHeartbeat)
                _clbkFunHeartbeat();
#endif // __cplusplus > 199711L
        }

        Sleep::us(us/_speed);
        _heartbeatHelper += us;
        _currentTimePos.addUs(us);
        this->execEvent((*_file)[trackNum][eventNum]);
        this->updatePlayerState(trackNum,dt);
    }

    if(_clbkFunPtrFinished)
        (*_clbkFunPtrFinished)(_clbkFunCtxFinished);

    if(_clbkObjPtrFinished)
        (*_clbkObjPtrFinished)();

#if __cplusplus > 199711L
            if(_clbkFunFinished)
                _clbkFunFinished();
#endif // __cplusplus > 199711L
}

} // Player
} // CxxMidi

#endif // CXXMIDI_PLAYER_SYNCHRONOUS_HPP
