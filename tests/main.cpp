#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>
#include <cxxmidi/callback.hpp>
#include <cxxmidi/guts/compiler.hpp>

class PlayerHeartbeatCallback : public CxxMidi::Callback<CxxMidi::Player::Abstract>
{
public:
    PlayerHeartbeatCallback(){}

    virtual void operator()(CxxMidi::Player::Abstract* player_)
    {
        std::cerr << player_->currentTimePos() << std::endl;
    }
};

class PlayerFinishedCallback : public CxxMidi::Callback<CxxMidi::Player::Abstract>
{
public:
    PlayerFinishedCallback(){}

    virtual void operator()(CxxMidi::Player::Abstract*)
    {
        std::cerr << "done!" << std::endl;
    }

};

int main(int /*argc*/, char ** /*argv*/)
{
    CxxMidi::Output::Default output(0);
    CxxMidi::Player::Synchronous player(&output);

#ifdef _WIN32
    CxxMidi::File file("C:\\sample.mid");
#endif // _WIN32
#ifdef __unix
    CxxMidi::File file("/home/sch/sample.mid");
#endif // __unix
    player.setFile(&file);

    PlayerHeartbeatCallback playerHeartbeatCallback;
    player.setCallbackHeartbeat(&playerHeartbeatCallback);

    PlayerFinishedCallback playerFinishedCallback;
    player.setCallbackFinished(&playerFinishedCallback);

    player.play();
}
