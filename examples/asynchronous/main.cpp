#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/asynchronous.hpp>
#include <cxxmidi/callback.hpp>

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
    CxxMidi::Player::Asynchronous player(&output);

    CxxMidi::File file("/home/sch/sample.mid");
    player.setFile(&file);

    PlayerHeartbeatCallback playerHeartbeatCallback;
    player.setCallbackHeartbeat(&playerHeartbeatCallback);

    PlayerFinishedCallback playerFinishedCallback;
    player.setCallbackFinished(&playerFinishedCallback);

    player.play();
    CxxMidi::Sleep::us(1000 * 1000 * 5); // 5 sec
    player.pause();
}
