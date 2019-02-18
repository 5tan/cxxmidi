#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>

#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/asynchronous.hpp>


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
  void UpdateTimeCode(std::chrono::microseconds time);
  void OnPauseClicked();
  void OnPlayClicked();
  void OpenFile(const QString& path);
  void OpenFile();
  void SetOutput(int num);
  void OnOutputSelected(QAction* action);
  void PlayerFinished();

 private:
  void CreateMenuBar();

  QActionGroup* outputs_action_group_;
  Ui::MainWindow* ui_;

  std::chrono::microseconds played_us_;
  std::chrono::microseconds total_us_;

  cxxmidi::output::Default* midi_output_;
  cxxmidi::player::Asynchronous* midi_player_;
  cxxmidi::File* midi_file_;

  PlayerHeartbeatCallback player_heartbeat_callback_;
  PlayerFinishedCallback player_finished_callback_;

  bool slider_locked_;
};

#endif  // MAINWINDOW_H
