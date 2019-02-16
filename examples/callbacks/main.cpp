#include <cxxmidi/callback.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

// there are 3 possible ways of registering a callback
void example1_cStyleCallbacks();
void example2_cppStyleCallbacks();
void example3_cpp11StyleCallbacks();

int main(int /*argc*/, char ** /*argv*/) {
  example1_cStyleCallbacks();
  example2_cppStyleCallbacks();
  example3_cpp11StyleCallbacks();
}

// *****************************************************************************
// Example 1: C style callbacks

void clbkHearbeat(void *context_) {
  cxxmidi::player::Abstract *player =
      reinterpret_cast<cxxmidi::player::Synchronous *>(context_);

  std::cerr << player->currentTimePos() << std::endl;
}

void functionFinished(void *) { std::cerr << "finished!" << std::endl; }

void example1_cStyleCallbacks() {
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.setFile(&file);

  player.setCallbackHeartbeat(clbkHearbeat, reinterpret_cast<void *>(&player));
  player.setCallbackFinished(functionFinished, 0);

  player.play();
}

// *****************************************************************************
// Example 2: C++ style callbacks

class MyHeartbeatCallback : public cxxmidi::Callback {
 public:
  MyHeartbeatCallback(cxxmidi::player::Synchronous *player_)
      : _player(player_) {}

  virtual void operator()() {
    std::cerr << _player->currentTimePos() << " (MyHeartbeatCallback)"
              << std::endl;
  }

 private:
  cxxmidi::player::Synchronous *_player;
};

class MyFinishedCallback : public cxxmidi::Callback {
 public:
  MyFinishedCallback() {}

  virtual void operator()() {
    std::cerr << "done! (MyFinishedCallback)" << std::endl;
  }
};

void example2_cppStyleCallbacks() {
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.setFile(&file);

  MyHeartbeatCallback myHeartbeatCallback(&player);
  player.setCallbackHeartbeat(&myHeartbeatCallback);

  MyFinishedCallback myFinishedCallback;
  player.setCallbackFinished(&myFinishedCallback);

  player.play();
}

// *****************************************************************************
// Example 3: C++11 style callbacks

void callbackHearbeat(cxxmidi::player::Synchronous *player_) {
  std::cerr << player_->currentTimePos() << " (C++11)" << std::endl;
}

void example3_cpp11StyleCallbacks() {
#if __cplusplus > 199711L
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);

  cxxmidi::File file("music/c_major_scale.mid");
  player.setFile(&file);

  auto myHeartbeatCallback = std::bind(callbackHearbeat, &player);
  player.setCallbackHeartbeat(myHeartbeatCallback);

  player.setCallbackFinished(
      []() { std::cerr << "finished! (C++11)" << std::endl; });

  player.play();
#endif  // __cplusplus > 199711L
}
