#include <cxxmidi/callback.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

// there are 3 possible ways of registering a callback
void Example1CStyleCallbacks();
void Example2CppStyleCallbacks();
void Example3Cpp11StyleCallbacks();

int main(int /*argc*/, char ** /*argv*/) {
  Example1CStyleCallbacks();
  Example2CppStyleCallbacks();
  Example3Cpp11StyleCallbacks();
}

// *****************************************************************************
// Example 1: C style callbacks

void ClbkHearbeat(void *context) {
  cxxmidi::player::Abstract *player =
      reinterpret_cast<cxxmidi::player::Synchronous *>(context);

  std::cout << player->CurrentTimePos2().count() << std::endl;
}

void FunctionFinished(void *) { std::cout << "finished!" << std::endl; }

void Example1CStyleCallbacks() {
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.SetFile(&file);

  player.SetCallbackHeartbeat(ClbkHearbeat, reinterpret_cast<void *>(&player));
  player.SetCallbackFinished(FunctionFinished, 0);

  player.Play();
}

// *****************************************************************************
// Example 2: C++ style callbacks

class MyHeartbeatCallback : public cxxmidi::Callback {
 public:
  MyHeartbeatCallback(cxxmidi::player::Synchronous *player)
      : _player(player) {}

  virtual void operator()() {
    std::cout << _player->CurrentTimePos2().count() << " (MyHeartbeatCallback)"
              << std::endl;
  }

 private:
  cxxmidi::player::Synchronous *_player;
};

class MyFinishedCallback : public cxxmidi::Callback {
 public:
  MyFinishedCallback() {}

  virtual void operator()() {
    std::cout << "done! (MyFinishedCallback)" << std::endl;
  }
};

void Example2CppStyleCallbacks() {
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.SetFile(&file);

  MyHeartbeatCallback myHeartbeatCallback(&player);
  player.SetCallbackHeartbeat(&myHeartbeatCallback);

  MyFinishedCallback myFinishedCallback;
  player.SetCallbackFinished(&myFinishedCallback);

  player.Play();
}

// *****************************************************************************
// Example 3: C++11 style callbacks

void callbackHearbeat(cxxmidi::player::Synchronous *player) {
  std::cout << player->CurrentTimePos2().count() << " (C++11)" << std::endl;
}

void Example3Cpp11StyleCallbacks() {
#if __cplusplus > 199711L
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.SetFile(&file);

  auto myHeartbeatCallback = std::bind(callbackHearbeat, &player);
  player.SetCallbackHeartbeat(myHeartbeatCallback);

  player.SetCallbackFinished(
      []() { std::cout << "finished! (C++11)" << std::endl; });

  player.Play();
#endif  // __cplusplus > 199711L
}
