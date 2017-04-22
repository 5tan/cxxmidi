#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>
#include <cxxmidi/callback.hpp>

int main(int /*argc*/, char ** /*argv*/)
{
    CxxMidi::Output::Default output(1);
    CxxMidi::Player::Synchronous player(&output);

    CxxMidi::File file("music/chopin.mid");
    player.setFile(&file);

    player.play();
}
