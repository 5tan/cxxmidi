#ifndef CXXMIDI_PLAYER_SYNCHRONOUS_HPP
#define CXXMIDI_PLAYER_SYNCHRONOUS_HPP

#include <cxxmidi/player/abstract.hpp>
#include <thread>

namespace cxxmidi {
class File;
namespace player {

class Synchronous : public player::Abstract {
 public:
  inline Synchronous(output::Abstract* output);
  inline virtual ~Synchronous();

  // Synchronous(const Synchronous&); // default is ok
  // Synchronous &operator=(const Synchronous &); // default is ok
#if __cplusplus > 199711L
  Synchronous(Synchronous&&) = default;
  Synchronous& operator=(Synchronous&&) = default;
#endif  // __cplusplus > 199711L

  inline virtual void Play();

 private:
  inline void PlayerLoop();
  inline virtual void Pause() {}
};

}  // namespace Player
}  // namespace CxxMidi

#include <assert.h>

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/sleep.hpp>
#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace player {

Synchronous::Synchronous(output::Abstract* output) : Abstract(output) {}

Synchronous::~Synchronous() {}

void Synchronous::Play() {
  if (!output_ || !file_) return;

  this->PlayerLoop();
}

void Synchronous::PlayerLoop() {
  while (!this->Finished()) {
    unsigned int track_num = this->TrackPending();
    unsigned int event_num = player_state_[track_num].track_pointer_;
    uint32_t dt = player_state_[track_num].track_dt_;
    auto us = converters::Dt2us2(dt, tempo_, file_->TimeDivision());

    while ((heartbeat_helper_ + us.count()) >= 10000) {
      unsigned int partial = 10000 - heartbeat_helper_;
      heartbeat_helper_ = 0;
      us -= std::chrono::microseconds(partial);

      unsigned int wait = partial / speed_;
      std::this_thread::sleep_for(std::chrono::microseconds(wait));
      played_us_ += std::chrono::microseconds(partial);

      if (clbk_fun_ptr_heartbeat_) (*clbk_fun_ptr_heartbeat_)(clbk_fun_ctx_heartbeat_);

      if (clbk_obj_ptr_heartbeat_) (*clbk_obj_ptr_heartbeat_)();

#if __cplusplus > 199711L
      if (clbk_fun_heartbeat_) clbk_fun_heartbeat_();
#endif  // __cplusplus > 199711L
    }

    unsigned int wait = us.count() / speed_;
    std::this_thread::sleep_for(std::chrono::microseconds(wait));
    heartbeat_helper_ += us.count();
    played_us_ += std::chrono::microseconds(us);
    this->ExecEvent((*file_)[track_num][event_num]);
    this->UpdatePlayerState(track_num, dt);
  }

  if (_clbkFunPtrFinished) (*_clbkFunPtrFinished)(clbk_fun_ctx_finished_);

  if (clbk_obj_ptr_finished_) (*clbk_obj_ptr_finished_)();

#if __cplusplus > 199711L
  if (clbk_fun_finished_) clbk_fun_finished_();
#endif  // __cplusplus > 199711L
}

}  // namespace Player
}  // namespace CxxMidi

#endif  // CXXMIDI_PLAYER_SYNCHRONOUS_HPP
