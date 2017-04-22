#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _midiOutput(new CxxMidi::Output::Default(0))
    , _midiPlayer(new CxxMidi::Player::Asynchronous(_midiOutput))
    , _midiFile(0)
    , _playerHeartbeatCallback(_midiPlayer)
    , _sliderLocked(false)
{
    _ui->setupUi(this);

    this->createMenuBar();
    this->centralWidget()->setDisabled(true);
    this->resize(this->minimumSizeHint());

    _midiPlayer->setCallbackHeartbeat(&_playerHeartbeatCallback);
    connect(&_playerHeartbeatCallback,SIGNAL(playerTimeChanged(CxxMidi::Time::Point)),
            this,SLOT(updateTimeCode(CxxMidi::Time::Point)),Qt::QueuedConnection);

    _midiPlayer->setCallbackFinished(&_playerFinishedCallback);
    connect(&_playerFinishedCallback,SIGNAL(playerFinished()),
            this,SLOT(playerFinished()),Qt::QueuedConnection);

    connect(_ui->doubleSpinBoxSpeed,SIGNAL(valueChanged(double)),
            this,SLOT(onSpeedChange(double)));

    connect(_ui->pushButtonPlay,SIGNAL(clicked()),
            this,SLOT(onPlayClicked()));

    connect(_ui->pushButtonPause,SIGNAL(clicked()),
            this,SLOT(onPauseClicked()));

    connect(_ui->sliderTimeline,SIGNAL(sliderPressed()),
            this,SLOT(onTimeSliderPressed()));

    connect(_ui->sliderTimeline,SIGNAL(sliderReleased()),
            this,SLOT(onTimeSliderReleased()));


    // first argument is file name
    if(QApplication::arguments().size() >=2 )
    {
        QString fileName = QApplication::arguments().at(1);
        this->openFile(fileName);
    }

    // second argument is output num
    if(QApplication::arguments().size() >=3)
    {
        int num = QApplication::arguments().at(2).toInt();
        this->setOutput(num);
        _outputsActionGroup->actions()[num]->setChecked(true);
    }

    // auto play
    if( QApplication::arguments().size() >= 2)
        _midiPlayer->play();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::createMenuBar()
{
    // file menu
    QMenu* fileMenu= this->menuBar()->addMenu(tr("&File"));
    QAction *action = fileMenu->addAction("&Open");

    connect(action,SIGNAL(triggered()),
            this,SLOT(openFile()));

    fileMenu->addSeparator();

    action = fileMenu->addAction("&Exit");
    connect(action,SIGNAL(triggered()),
            QApplication::instance(),SLOT(quit()));

    // output menu
    QMenu* outputMenu= this->menuBar()->addMenu(tr("&Output"));
    _outputsActionGroup = new QActionGroup(this);
    _outputsActionGroup->setExclusive(true);

    QString name;
    for(size_t i=0; i<_midiOutput->getPortCount();i++)
    {
        name=QString("%1. %2").arg(i).arg(_midiOutput->getPortName(i).c_str());
        action = _outputsActionGroup->addAction(name);
        action->setCheckable(true);
        if(!i) // first is opened
            action->setChecked(true);
    }

    outputMenu->addActions(_outputsActionGroup->actions());

    connect(_outputsActionGroup,SIGNAL(triggered(QAction*)),
            this,SLOT(onOutputSelected(QAction*)));
}


void MainWindow::onOutputSelected(QAction* action_)
{
    int num = action_->text().split(".")[0].toInt();
    this->setOutput(num);
}

void MainWindow::setOutput(int num_)
{
    bool wasPlaying = _midiPlayer->isPlaying();

    if(wasPlaying)
        _midiPlayer->pause();
    CxxMidi::Time::Point tp = _midiPlayer->currentTimePos();
    _midiOutput->openPort(num_);
    _midiPlayer->goTo(tp);
    if(wasPlaying)
        _midiPlayer->play();
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"), ".", tr("MIDI files (*.mid *.midi);;Any files (*)"));
    if( fileName.size() )
    {
        this->openFile(fileName);
        _midiPlayer->play();
    }
}

void MainWindow::openFile(const QString& path_)
{
    if(_midiPlayer->isPlaying())
        _midiPlayer->pause();

    if(_midiFile)
        delete _midiFile;

    _midiFile = new CxxMidi::File(path_.toStdString().c_str());
    _midiPlayer->setFile(_midiFile);

    _finalTimePoint = _midiFile->duration().toPoint();
    _ui->labelTotal->setText(_finalTimePoint.toTimecode().c_str());

    this->centralWidget()->setDisabled(false);
}

void MainWindow::onPlayClicked()
{
    _midiPlayer->play();
}

void MainWindow::onPauseClicked()
{
    _midiPlayer->pause();
}

void MainWindow::playerFinished()
{
    _midiPlayer->goTo(CxxMidi::Time::Point::zero());
    _midiPlayer->play();
}

void MainWindow::updateTimeCode(CxxMidi::Time::Point time_)
{
    _currentTimePoint = time_;
    _ui->labelTime->setText(time_.toTimecode().c_str());

    if(!_sliderLocked)
    {
        double normPos = _currentTimePoint / _finalTimePoint;
        _ui->sliderTimeline->setValue(100*normPos);
    }
}

void MainWindow::onSpeedChange(double speed_)
{
    _midiPlayer->setSpeed(speed_);
}

void MainWindow::onTimeSliderPressed()
{
    _sliderLocked = true;
}

void MainWindow::onTimeSliderReleased()
{
    double val = _ui->sliderTimeline->value();
    double size = _ui->sliderTimeline->maximum();
    double pos = val/size;
    CxxMidi::Time::Point tp =  _midiFile->duration().toPoint();
    tp*=pos;

    bool wasPlaying = _midiPlayer->isPlaying();
    _midiPlayer->pause();
    _midiPlayer->goTo(tp);
    if(wasPlaying)
        _midiPlayer->play();

    _sliderLocked = false;
}
