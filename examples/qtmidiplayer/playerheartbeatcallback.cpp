#include "playerheartbeatcallback.h"

#include <cxxmidi/player/abstract.hpp>

PlayerHeartbeatCallback::PlayerHeartbeatCallback(QObject *parent_)
    : QObject(parent_)
{
}

void PlayerHeartbeatCallback::operator ()(CxxMidi::Player::Abstract* player_)
{
    /*emit*/ this->playerTimeChanged(player_->currentTimePos());
}
