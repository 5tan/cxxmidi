#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/asynchronous.hpp>
#include <cxxmidi/callback.hpp>

int main(int /*argc*/, char ** /*argv*/)
{
    CxxMidi::Output::Default output(0);
    CxxMidi::Player::Asynchronous player(&output);

    CxxMidi::File file("/home/sch/sample.mid");
    player.setFile(&file);

    player.play();
    CxxMidi::Sleep::us(1000 * 1000 * 5); // 5 sec
    player.pause();
}
