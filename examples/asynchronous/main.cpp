#include <cxxmidi/callback.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/asynchronous.hpp>

int main(int /*argc*/, char** /*argv*/) {
  cxxmidi::output::Default output(1);
  cxxmidi::player::Asynchronous player(&output);

  cxxmidi::File file("/home/sch/sample.mid");
  player.SetFile(&file);

  player.Play();
  std::this_thread::sleep_for(std::chrono::seconds(15));
  player.Pause();
}
