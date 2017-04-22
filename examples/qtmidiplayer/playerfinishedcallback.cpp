#include "playerfinishedcallback.h"

PlayerFinishedCallback::PlayerFinishedCallback(QObject *parent_)
    : QObject(parent_)
{
}

void PlayerFinishedCallback::operator()()
{
    /*emit*/ this->playerFinished();
}
