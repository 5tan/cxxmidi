#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>

#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/asynchronous.hpp>
#include <cxxmidi/time/point.hpp>

#include "playerfinishedcallback.h"
#include "playerheartbeatcallback.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

 private slots:
  void OnTimeSliderReleased();
  void OnTimeSliderPressed();
  void OnSpeedChange(double speed);
  void updateTimeCode(cxxmidi::time::Point time);
  void OnPauseClicked();
  void OnPlayClicked();
  void OpenFile(const QString& path);
  void OpenFile();
  void SetOutput(int num);
  void OnOutputSelected(QAction* action);
  void PlayerFinished();

 private:
  void CreateMenuBar();

  QActionGroup* _outputsActionGroup;
  Ui::MainWindow* _ui;

  cxxmidi::time::Point _currentTimePoint;
  cxxmidi::time::Point _finalTimePoint;

  cxxmidi::output::Default* _midiOutput;
  cxxmidi::player::Asynchronous* _midiPlayer;
  cxxmidi::File* _midiFile;

  PlayerHeartbeatCallback _playerHeartbeatCallback;
  PlayerFinishedCallback _playerFinishedCallback;

  bool _sliderLocked;
};

#endif  // MAINWINDOW_H
