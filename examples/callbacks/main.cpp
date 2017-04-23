#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>
#include <cxxmidi/callback.hpp>

// there are 3 possible ways of registering a callback
void example1_cStyleCallbacks();
void example2_cppStyleCallbacks();
void example3_cpp11StyleCallbacks();

int main(int /*argc*/, char ** /*argv*/)
{
    example1_cStyleCallbacks();
    example2_cppStyleCallbacks();
    example3_cpp11StyleCallbacks();
}

// *****************************************************************************
// Example 1: C style callbacks

void clbkHearbeat(void *context_)
{
    CxxMidi::Player::Abstract *player =
            reinterpret_cast<CxxMidi::Player::Synchronous *>(context_);

    std::cerr << player->currentTimePos() << std::endl;
}

void functionFinished(void*)
{
    std::cerr << "finished!" << std::endl;
}

void example1_cStyleCallbacks()
{
    CxxMidi::Output::Default output(0);
    CxxMidi::Player::Synchronous player(&output);

    CxxMidi::File file("music/c_major_scale.mid");
    player.setFile(&file);

    player.setCallbackHeartbeat(clbkHearbeat, reinterpret_cast<void*>(&player));
    player.setCallbackFinished(functionFinished,0);

    player.play();
}

// *****************************************************************************
// Example 2: C++ style callbacks

class MyHeartbeatCallback : public CxxMidi::Callback
{
public:
    MyHeartbeatCallback(CxxMidi::Player::Synchronous *player_)
        : _player(player_)
    {}

    virtual void operator()()
    {
        std::cerr << _player->currentTimePos()
                  << " (MyHeartbeatCallback)"
                  << std::endl;
    }

private:
    CxxMidi::Player::Synchronous *_player;
};

class MyFinishedCallback : public CxxMidi::Callback
{
public:
    MyFinishedCallback(){}

    virtual void operator()()
    {
        std::cerr << "done! (MyFinishedCallback)" << std::endl;
    }

};

void example2_cppStyleCallbacks()
{
    CxxMidi::Output::Default output(0);
    CxxMidi::Player::Synchronous player(&output);

    CxxMidi::File file("music/c_major_scale.mid");
    player.setFile(&file);

    MyHeartbeatCallback myHeartbeatCallback(&player);
    player.setCallbackHeartbeat(&myHeartbeatCallback);

    MyFinishedCallback myFinishedCallback;
    player.setCallbackFinished(&myFinishedCallback);

    player.play();
}

// *****************************************************************************
// Example 3: C++11 style callbacks

void callbackHearbeat(CxxMidi::Player::Synchronous *player_)
{
    std::cerr << player_->currentTimePos()
              << " (C++11)"
              << std::endl;
}

void example3_cpp11StyleCallbacks()
{
#if __cplusplus > 199711L
    CxxMidi::Output::Default output(0);
    CxxMidi::Player::Synchronous player(&output);

    CxxMidi::File file("music/c_major_scale.mid");
    player.setFile(&file);

    auto myHeartbeatCallback = std::bind(callbackHearbeat, &player);
    player.setCallbackHeartbeat(myHeartbeatCallback);

    player.setCallbackFinished([]()
    {
        std::cerr << "finished! (C++11)" << std::endl;
    });

    player.play();
#endif // __cplusplus > 199711L
}
