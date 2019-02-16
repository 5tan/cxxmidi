#include "playerfinishedcallback.h"

PlayerFinishedCallback::PlayerFinishedCallback(QObject *parent)
    : QObject(parent) {}

void PlayerFinishedCallback::operator()() { /*emit*/ this->PlayerFinished(); }
