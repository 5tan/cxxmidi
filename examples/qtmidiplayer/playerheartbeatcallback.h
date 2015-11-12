#ifndef PLAYERHEARTBEATCALLBACK_H
#define PLAYERHEARTBEATCALLBACK_H

#include <QObject>

#include <cxxmidi/player/abstract.hpp>
#include <cxxmidi/callback.hpp>
#include <cxxmidi/time/point.hpp>

class PlayerHeartbeatCallback
        : public QObject
        , public CxxMidi::Callback<CxxMidi::Player::Abstract>
{
    Q_OBJECT

public:
    PlayerHeartbeatCallback(QObject *parent_=0);
    virtual void operator ()(CxxMidi::Player::Abstract *player_);

signals:
    void playerTimeChanged(CxxMidi::Time::Point time_);
};

#endif // PLAYERHEARTBEATCALLBACK_H
