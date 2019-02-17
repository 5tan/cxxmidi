#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      midi_output_(new cxxmidi::output::Default(0)),
      midi_player_(new cxxmidi::player::Asynchronous(midi_output_)),
      midi_file_(0),
      player_heartbeat_callback_(midi_player_),
      slider_locked_(false) {
  ui_->setupUi(this);

  this->CreateMenuBar();
  this->centralWidget()->setDisabled(true);
  this->resize(this->minimumSizeHint());

  midi_player_->SetCallbackHeartbeat(&player_heartbeat_callback_);
  connect(&player_heartbeat_callback_,
          &PlayerHeartbeatCallback::PlayerTimeChanged,
          this, &MainWindow::UpdateTimeCode, Qt::QueuedConnection);

  midi_player_->SetCallbackFinished(&player_finished_callback_);
  connect(&player_finished_callback_, &PlayerFinishedCallback::PlayerFinished,
          this,
          &MainWindow::PlayerFinished, Qt::QueuedConnection);

  connect(ui_->doubleSpinBoxSpeed, qOverload<double>(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::OnSpeedChange);

  connect(ui_->pushButtonPlay, &QPushButton::clicked,
          this, &MainWindow::OnPlayClicked);

  connect(ui_->pushButtonPause, &QPushButton::clicked, this,
          &MainWindow::OnPauseClicked);

  connect(ui_->sliderTimeline, &QSlider::sliderPressed, this,
          &MainWindow::OnTimeSliderPressed);

  connect(ui_->sliderTimeline, &QSlider::sliderReleased, this,
          &MainWindow::OnTimeSliderReleased);

  // first argument is file name
  if (QApplication::arguments().size() >= 2) {
    QString fileName = QApplication::arguments().at(1);
    this->OpenFile(fileName);
  }

  // second argument is output num
  if (QApplication::arguments().size() >= 3) {
    int num = QApplication::arguments().at(2).toInt();
    this->SetOutput(num);
    outputs_action_group_->actions()[num]->setChecked(true);
  }

  // auto play
  if (QApplication::arguments().size() >= 2) midi_player_->Play();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::CreateMenuBar() {
  // file menu
  QMenu* file_menu = this->menuBar()->addMenu(tr("&File"));
  QAction* action = file_menu->addAction("&Open");

  connect(action, &QAction::triggered, this, [this](bool){ OpenFile(); });

  file_menu->addSeparator();

  action = file_menu->addAction("&Exit");
  connect(action, &QAction::triggered, QApplication::instance(), &QApplication::quit);

  // output menu
  QMenu* output_menu = this->menuBar()->addMenu(tr("&Output"));
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
  this->SetOutput(num);
}

void MainWindow::SetOutput(int num) {
  bool was_playing = midi_player_->IsPlaying();

  if (was_playing) midi_player_->Pause();
  cxxmidi::time::Point tp = midi_player_->CurrentTimePos();
  midi_output_->OpenPort(num);
  midi_player_->GoTo(tp);
  if (was_playing) midi_player_->Play();
}

void MainWindow::OpenFile() {
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open file"), ".",
      tr("MIDI files (*.mid *.midi);;Any files (*)"));
  if (file_name.size()) {
    this->OpenFile(file_name);
    midi_player_->Play();
  }
}

void MainWindow::OpenFile(const QString& path) {
  if (midi_player_->IsPlaying()) midi_player_->Pause();

  if (midi_file_) delete midi_file_;

  midi_file_ = new cxxmidi::File(path.toStdString().c_str());
  midi_player_->SetFile(midi_file_);

  final_time_Point_ = midi_file_->Duration().ToPoint();
  ui_->labelTotal->setText(final_time_Point_.ToTimecode().c_str());

  this->centralWidget()->setDisabled(false);
}

void MainWindow::OnPlayClicked() { midi_player_->Play(); }

void MainWindow::OnPauseClicked() { midi_player_->Pause(); }

void MainWindow::PlayerFinished() {
  midi_player_->GoTo(cxxmidi::time::Point::Zero());
  midi_player_->Play();
}

void MainWindow::UpdateTimeCode(cxxmidi::time::Point time) {
  current_time_point_ = time;
  ui_->labelTime->setText(time.ToTimecode().c_str());

  if (!slider_locked_) {
    double norm_pos = current_time_point_ / final_time_Point_;
    ui_->sliderTimeline->setValue(100 * norm_pos);
  }
}

void MainWindow::OnSpeedChange(double speed) { midi_player_->SetSpeed(speed); }

void MainWindow::OnTimeSliderPressed() { slider_locked_ = true; }

void MainWindow::OnTimeSliderReleased() {
  double val = ui_->sliderTimeline->value();
  double size = ui_->sliderTimeline->maximum();
  double pos = val / size;
  cxxmidi::time::Point tp = midi_file_->Duration().ToPoint();
  tp *= pos;

  bool wasPlaying = midi_player_->IsPlaying();
  midi_player_->Pause();
  midi_player_->GoTo(tp);
  if (wasPlaying) midi_player_->Play();

  slider_locked_ = false;
}
