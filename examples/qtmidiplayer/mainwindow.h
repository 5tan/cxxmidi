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
  void onTimeSliderReleased();
  void onTimeSliderPressed();
  void onSpeedChange(double speed_);
  void updateTimeCode(cxxmidi::time::Point time_);
  void onPauseClicked();
  void onPlayClicked();
  void openFile(const QString& path_);
  void openFile();
  void setOutput(int num_);
  void onOutputSelected(QAction* action_);
  void playerFinished();

 private:
  void createMenuBar();

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
