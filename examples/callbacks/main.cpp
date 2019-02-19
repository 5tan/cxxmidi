#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

int main(int, char**) {
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.SetFile(&file);

  player.SetCallbackHeartbeat([]() { std::cout << "beep!" << std::endl; });

  player.SetCallbackFinished(
      []() { std::cout << "finished!" << std::endl; });

  player.Play();
}
