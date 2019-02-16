#include <cxxmidi/callback.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/asynchronous.hpp>

int main(int /*argc*/, char** /*argv*/) {
  cxxmidi::output::Default output(0);
  cxxmidi::player::Asynchronous player(&output);

  cxxmidi::File file("/home/sch/sample.mid");
  player.SetFile(&file);

  player.Play();
  cxxmidi::sleep::SleepUs(1000 * 1000 * 5);  // 5 sec
  player.Pause();
}
