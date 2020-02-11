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

#include <QFileDialog>

#include "mainwindow.h"     // NOLINT(build/include_subdir) INCLUDE_NO_DIR
#include "ui_mainwindow.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      played_us_(std::chrono::microseconds::zero()),
      total_us_(std::chrono::microseconds::zero()),
      midi_output_(new cxxmidi::output::Default(0)),
      midi_player_(new cxxmidi::player::PlayerAsync(midi_output_)),
      midi_file_(0),
      slider_locked_(false) {
  ui_->setupUi(this);

  CreateMenuBar();
  centralWidget()->setDisabled(true);
  resize(minimumSizeHint());

  midi_player_->SetCallbackHeartbeat(
      [this]() { PlayerTimeChanged(midi_player_->CurrentTimePos()); });
  midi_player_->SetCallbackFinished([this]() { PlayerFinished(); });
  connect(this, &MainWindow::PlayerTimeChanged, this,
          &MainWindow::UpdateTimeCode, Qt::QueuedConnection);
  connect(this, &MainWindow::PlayerFinished, this,
          &MainWindow::OnPlayerFinished, Qt::QueuedConnection);

  connect(ui_->doubleSpinBoxSpeed,
          qOverload<double>(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::OnSpeedChange);

  connect(ui_->pushButtonPlay, &QPushButton::clicked, this,
          &MainWindow::OnPlayClicked);

  connect(ui_->pushButtonPause, &QPushButton::clicked, this,
          &MainWindow::OnPauseClicked);

  connect(ui_->sliderTimeline, &QSlider::sliderPressed, this,
          &MainWindow::OnTimeSliderPressed);

  connect(ui_->sliderTimeline, &QSlider::sliderReleased, this,
          &MainWindow::OnTimeSliderReleased);

  // first argument is file name
  if (QApplication::arguments().size() >= 2) {
    QString fileName = QApplication::arguments().at(1);
    OpenFile(fileName);
  }

  // second argument is output num
  if (QApplication::arguments().size() >= 3) {
    int num = QApplication::arguments().at(2).toInt();
    SetOutput(num);
    outputs_action_group_->actions()[num]->setChecked(true);
  }

  // auto play
  if (QApplication::arguments().size() >= 2) midi_player_->Play();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::CreateMenuBar() {
  // file menu
  QMenu* file_menu = menuBar()->addMenu(tr("&File"));
  QAction* action = file_menu->addAction("&Open");

  connect(action, &QAction::triggered, this, [this](bool) { OpenFile(); });

  file_menu->addSeparator();

  action = file_menu->addAction("&Exit");
  connect(action, &QAction::triggered, QApplication::instance(),
          &QApplication::quit);

  // output menu
  QMenu* output_menu = menuBar()->addMenu(tr("&Output"));
  outputs_action_group_ = new QActionGroup(this);
  outputs_action_group_->setExclusive(true);

  QString name;
  for (size_t i = 0; i < midi_output_->GetPortCount(); i++) {
    name = QString("%1. %2").arg(i).arg(midi_output_->GetPortName(i).c_str());
    action = outputs_action_group_->addAction(name);
    action->setCheckable(true);
    if (!i)  // first is opened
      action->setChecked(true);
  }

  output_menu->addActions(outputs_action_group_->actions());

  connect(outputs_action_group_, &QActionGroup::triggered, this,
          &MainWindow::OnOutputSelected);
}

void MainWindow::OnOutputSelected(QAction* action) {
  int num = action->text().split(".")[0].toInt();
  SetOutput(num);
}

void MainWindow::SetOutput(int num) {
  bool was_playing = midi_player_->IsPlaying();

  if (was_playing) midi_player_->Pause();
  auto tp = midi_player_->CurrentTimePos();
  midi_output_->OpenPort(num);
  midi_player_->GoTo(tp);
  if (was_playing) midi_player_->Play();
}

void MainWindow::OpenFile() {
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open file"), ".",
      tr("MIDI files (*.mid *.midi);;Any files (*)"));
  if (file_name.size()) {
    OpenFile(file_name);
    midi_player_->Play();
  }
}

void MainWindow::OpenFile(const QString& path) {
  if (midi_player_->IsPlaying()) midi_player_->Pause();

  if (midi_file_) delete midi_file_;

  midi_file_ = new cxxmidi::File(path.toStdString().c_str());
  midi_player_->SetFile(midi_file_);

  total_us_ = midi_file_->Duration();
  ui_->labelTotal->setText(std::to_string(total_us_.count()).c_str());

  centralWidget()->setDisabled(false);
}

void MainWindow::OnPlayClicked() { midi_player_->Play(); }

void MainWindow::OnPauseClicked() { midi_player_->Pause(); }

void MainWindow::OnPlayerFinished() {
  midi_player_->GoTo(std::chrono::microseconds::zero());
  midi_player_->Play();
}

void MainWindow::UpdateTimeCode(std::chrono::microseconds time) {
  played_us_ = time;
  ui_->labelTime->setText(std::to_string(time.count()).c_str());

  if (!slider_locked_) {
    auto norm_pos = static_cast<double>(played_us_.count()) / total_us_.count();
    ui_->sliderTimeline->setValue(100 * norm_pos);
  }
}

void MainWindow::OnSpeedChange(double speed) { midi_player_->SetSpeed(speed); }

void MainWindow::OnTimeSliderPressed() { slider_locked_ = true; }

void MainWindow::OnTimeSliderReleased() {
  double val = ui_->sliderTimeline->value();
  double size = ui_->sliderTimeline->maximum();
  double pos = val / size;
  auto tp = static_cast<double>(midi_file_->Duration().count()) * pos;
  auto tp2 = static_cast<std::chrono::microseconds::rep>(tp);

  bool wasPlaying = midi_player_->IsPlaying();
  midi_player_->Pause();
  midi_player_->GoTo(std::chrono::microseconds(tp2));
  if (wasPlaying) midi_player_->Play();

  slider_locked_ = false;
}
