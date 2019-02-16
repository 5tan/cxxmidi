#ifndef PLAYERFINISHEDCALLBACK_H
#define PLAYERFINISHEDCALLBACK_H

#include <QObject>

#include <cxxmidi/callback.hpp>
#include <cxxmidi/player/abstract.hpp>

class PlayerFinishedCallback : public QObject, public cxxmidi::Callback {
  Q_OBJECT

 public:
  PlayerFinishedCallback(QObject *parent_ = 0);

  virtual void operator()();

 signals:
  void playerFinished();
};

#endif  // PLAYERFINISHEDCALLBACK_H
