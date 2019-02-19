#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

int main(int /*argc*/, char** /*argv*/) {
  cxxmidi::output::Default output(1);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/chopin.mid");
  player.SetFile(&file);

  player.Play();
}
