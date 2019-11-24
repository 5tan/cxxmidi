/* *****************************************************************************
Copyright (c) 2018 Stan Chlebicki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************** */

#ifndef EXAMPLES_QTMIDIPLAYER_MAINWINDOW_H_
#define EXAMPLES_QTMIDIPLAYER_MAINWINDOW_H_

#include <QActionGroup>
#include <QMainWindow>

#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/player_async.hpp>

namespace Ui {
class MainWindow;
}  // namespace Ui

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

 signals:  // NOLINT(whitespace/indent) SIGNAL_SLOT_SPECIFIER
  void PlayerFinished();
  void PlayerTimeChanged(std::chrono::microseconds time);

 private slots:  // NOLINT(whitespace/indent) SIGNAL_SLOT_SPECIFIER
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
  void OnPlayerFinished();

 private:
  void CreateMenuBar();

  QActionGroup* outputs_action_group_;
  Ui::MainWindow* ui_;

  std::chrono::microseconds played_us_;
  std::chrono::microseconds total_us_;

  cxxmidi::output::Default* midi_output_;
  cxxmidi::player::PlayerAsync* midi_player_;
  cxxmidi::File* midi_file_;

  bool slider_locked_;
};

#endif  // EXAMPLES_QTMIDIPLAYER_MAINWINDOW_H_
