#ifndef PLAYERFINISHEDCALLBACK_H
#define PLAYERFINISHEDCALLBACK_H

#include <QObject>

#include <cxxmidi/player/abstract.hpp>
#include <cxxmidi/callback.hpp>

class PlayerFinishedCallback
        : public QObject
        , public CxxMidi::Callback<CxxMidi::Player::Abstract>
{
    Q_OBJECT

public:
    PlayerFinishedCallback(QObject *parent_=0);

    virtual void operator()(CxxMidi::Player::Abstract * player_);

signals:
    void playerFinished();
};

#endif // PLAYERFINISHEDCALLBACK_H
