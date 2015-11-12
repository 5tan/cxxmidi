#include "playerfinishedcallback.h"

PlayerFinishedCallback::PlayerFinishedCallback(QObject *parent_)
    : QObject(parent_)
{
}

void PlayerFinishedCallback::operator()(CxxMidi::Player::Abstract *)
{
    /*emit*/ this->playerFinished();
}
