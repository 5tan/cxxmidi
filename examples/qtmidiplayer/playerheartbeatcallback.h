#ifndef PLAYERHEARTBEATCALLBACK_H
#define PLAYERHEARTBEATCALLBACK_H

#include <QObject>

#include <cxxmidi/player/abstract.hpp>
#include <cxxmidi/callback.hpp>
#include <cxxmidi/time/point.hpp>

class PlayerHeartbeatCallback
        : public QObject
        , public CxxMidi::Callback
{
    Q_OBJECT

public:
    PlayerHeartbeatCallback(CxxMidi::Player::Abstract *player_,QObject *parent_=0);
    virtual void operator ()();

signals:
    void playerTimeChanged(CxxMidi::Time::Point time_);

private:
    CxxMidi::Player::Abstract *_player;
};

#endif // PLAYERHEARTBEATCALLBACK_H
