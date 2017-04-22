#include "playerheartbeatcallback.h"

#include <cxxmidi/player/abstract.hpp>

PlayerHeartbeatCallback::PlayerHeartbeatCallback(
        CxxMidi::Player::Abstract *player_,
        QObject *parent_)
    : QObject(parent_)
    , _player(player_)
{
}

void PlayerHeartbeatCallback::operator ()()
{
    /*emit*/ this->playerTimeChanged(_player->currentTimePos());
}
