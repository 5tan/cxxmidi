#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      _ui(new Ui::MainWindow),
      _midiOutput(new cxxmidi::output::Default(0)),
      _midiPlayer(new cxxmidi::player::Asynchronous(_midiOutput)),
      _midiFile(0),
      _playerHeartbeatCallback(_midiPlayer),
      _sliderLocked(false) {
  _ui->setupUi(this);

  this->CreateMenuBar();
  this->centralWidget()->setDisabled(true);
  this->resize(this->minimumSizeHint());

  _midiPlayer->SetCallbackHeartbeat(&_playerHeartbeatCallback);
  connect(&_playerHeartbeatCallback,
          SIGNAL(playerTimeChanged(cxxmidi::time::Point)), this,
          SLOT(updateTimeCode(cxxmidi::time::Point)), Qt::QueuedConnection);

  _midiPlayer->SetCallbackFinished(&_playerFinishedCallback);
  connect(&_playerFinishedCallback, SIGNAL(PlayerFinished()), this,
          SLOT(PlayerFinished()), Qt::QueuedConnection);

  connect(_ui->doubleSpinBoxSpeed, SIGNAL(valueChanged(double)), this,
          SLOT(OnSpeedChange(double)));

  connect(_ui->pushButtonPlay, SIGNAL(clicked()), this, SLOT(OnPlayClicked()));

  connect(_ui->pushButtonPause, SIGNAL(clicked()), this,
          SLOT(OnPauseClicked()));

  connect(_ui->sliderTimeline, SIGNAL(sliderPressed()), this,
          SLOT(OnTimeSliderPressed()));

  connect(_ui->sliderTimeline, SIGNAL(sliderReleased()), this,
          SLOT(OnTimeSliderReleased()));

  // first argument is file name
  if (QApplication::arguments().size() >= 2) {
    QString fileName = QApplication::arguments().at(1);
    this->OpenFile(fileName);
  }

  // second argument is output num
  if (QApplication::arguments().size() >= 3) {
    int num = QApplication::arguments().at(2).toInt();
    this->SetOutput(num);
    _outputsActionGroup->actions()[num]->setChecked(true);
  }

  // auto play
  if (QApplication::arguments().size() >= 2) _midiPlayer->Play();
}

MainWindow::~MainWindow() { delete _ui; }

void MainWindow::CreateMenuBar() {
  // file menu
  QMenu* file_menu = this->menuBar()->addMenu(tr("&File"));
  QAction* action = file_menu->addAction("&Open");

  connect(action, SIGNAL(triggered()), this, SLOT(OpenFile()));

  file_menu->addSeparator();

  action = file_menu->addAction("&Exit");
  connect(action, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

  // output menu
  QMenu* output_menu = this->menuBar()->addMenu(tr("&Output"));
  _outputsActionGroup = new QActionGroup(this);
  _outputsActionGroup->setExclusive(true);

  QString name;
  for (size_t i = 0; i < _midiOutput->GetPortCount(); i++) {
    name = QString("%1. %2").arg(i).arg(_midiOutput->GetPortName(i).c_str());
    action = _outputsActionGroup->addAction(name);
    action->setCheckable(true);
    if (!i)  // first is opened
      action->setChecked(true);
  }

  output_menu->addActions(_outputsActionGroup->actions());

  connect(_outputsActionGroup, SIGNAL(triggered(QAction*)), this,
          SLOT(OnOutputSelected(QAction*)));
}

void MainWindow::OnOutputSelected(QAction* action) {
  int num = action->text().split(".")[0].toInt();
  this->SetOutput(num);
}

void MainWindow::SetOutput(int num) {
  bool was_playing = _midiPlayer->IsPlaying();

  if (was_playing) _midiPlayer->Pause();
  cxxmidi::time::Point tp = _midiPlayer->CurrentTimePos();
  _midiOutput->OpenPort(num);
  _midiPlayer->GoTo(tp);
  if (was_playing) _midiPlayer->Play();
}

void MainWindow::OpenFile() {
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open file"), ".",
      tr("MIDI files (*.mid *.midi);;Any files (*)"));
  if (file_name.size()) {
    this->OpenFile(file_name);
    _midiPlayer->Play();
  }
}

void MainWindow::OpenFile(const QString& path) {
  if (_midiPlayer->IsPlaying()) _midiPlayer->Pause();

  if (_midiFile) delete _midiFile;

  _midiFile = new cxxmidi::File(path.toStdString().c_str());
  _midiPlayer->SetFile(_midiFile);

  _finalTimePoint = _midiFile->Duration().ToPoint();
  _ui->labelTotal->setText(_finalTimePoint.ToTimecode().c_str());

  this->centralWidget()->setDisabled(false);
}

void MainWindow::OnPlayClicked() { _midiPlayer->Play(); }

void MainWindow::OnPauseClicked() { _midiPlayer->Pause(); }

void MainWindow::PlayerFinished() {
  _midiPlayer->GoTo(cxxmidi::time::Point::Zero());
  _midiPlayer->Play();
}

void MainWindow::updateTimeCode(cxxmidi::time::Point time) {
  _currentTimePoint = time;
  _ui->labelTime->setText(time.ToTimecode().c_str());

  if (!_sliderLocked) {
    double norm_pos = _currentTimePoint / _finalTimePoint;
    _ui->sliderTimeline->setValue(100 * norm_pos);
  }
}

void MainWindow::OnSpeedChange(double speed) { _midiPlayer->SetSpeed(speed); }

void MainWindow::OnTimeSliderPressed() { _sliderLocked = true; }

void MainWindow::OnTimeSliderReleased() {
  double val = _ui->sliderTimeline->value();
  double size = _ui->sliderTimeline->maximum();
  double pos = val / size;
  cxxmidi::time::Point tp = _midiFile->Duration().ToPoint();
  tp *= pos;

  bool wasPlaying = _midiPlayer->IsPlaying();
  _midiPlayer->Pause();
  _midiPlayer->GoTo(tp);
  if (wasPlaying) _midiPlayer->Play();

  _sliderLocked = false;
}
