#ifndef PLAYERHEARTBEATCALLBACK_H
#define PLAYERHEARTBEATCALLBACK_H

#include <QObject>

#include <cxxmidi/callback.hpp>
#include <cxxmidi/player/abstract.hpp>
#include <cxxmidi/time/point.hpp>

class PlayerHeartbeatCallback : public QObject, public cxxmidi::Callback {
  Q_OBJECT

 public:
  PlayerHeartbeatCallback(cxxmidi::player::Abstract *player,
                          QObject *parent = 0);
  virtual void operator()();

 signals:
  void PlayerTimeChanged(cxxmidi::time::Point time);

 private:
  cxxmidi::player::Abstract *player_;
};

#endif  // PLAYERHEARTBEATCALLBACK_H
