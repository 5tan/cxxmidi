#ifndef CXXMIDI_GUTS_SIMULATOR_HPP
#define CXXMIDI_GUTS_SIMULATOR_HPP

#include <cxxmidi/player/abstract.hpp>

namespace CxxMidi {
namespace Time {
class Duration;
}
namespace Guts {

class Simulator : public Player::Abstract
{
public:

    inline Time::Duration duration(const File &file_);

private:
    inline virtual void play(){}
    inline virtual void play(const File* /*file_*/){}
    inline virtual void pause(){}
    inline virtual void resume(){}
};

} // namespace Guts
} // namespace CxxMidi

#include <cxxmidi/converters.hpp>
#include <cxxmidi/time/duration.hpp>

namespace CxxMidi {
namespace Guts {

Time::Duration Simulator::duration(const File &file_)
{
    Time::Duration r;

    _tempo = 500000; // default tempo
    _file = &file_;
    this->initPlayerState();

    while(!this->finished())
    {
        unsigned int trackNum = this->trackPending();
        unsigned int eventNum = _playerState[trackNum].trackPointer;
        uint32_t dt = _playerState[trackNum].trackDt;
        r.addUs( Converters::dt2us(dt,_tempo,_file->timeDivision()) );
        this->execEvent((*_file)[trackNum][eventNum]);
        this->updatePlayerState(trackNum,dt);
    }

    return r;
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_SIMULATOR_HPP
