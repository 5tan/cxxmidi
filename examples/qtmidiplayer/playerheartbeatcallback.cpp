#include "playerheartbeatcallback.h"

#include <cxxmidi/player/abstract.hpp>

PlayerHeartbeatCallback::PlayerHeartbeatCallback(cxxmidi::player::Abstract *player, QObject *parent)
    : QObject(parent), _player(player) {}

void PlayerHeartbeatCallback::operator()() {
  /*emit*/ this->PlayerTimeChanged(_player->CurrentTimePos());
}
