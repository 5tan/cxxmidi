#include "playerheartbeatcallback.h"

#include <cxxmidi/player/abstract.hpp>

PlayerHeartbeatCallback::PlayerHeartbeatCallback(cxxmidi::player::Abstract *player, QObject *parent)
    : QObject(parent), player_(player) {}

void PlayerHeartbeatCallback::operator()() {
  /*emit*/ this->PlayerTimeChanged2(player_->CurrentTimePos2());
}
