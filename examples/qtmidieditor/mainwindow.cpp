#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent_)
    : QMainWindow(parent_), _ui(new Ui::MainWindow) {
  _ui->setupUi(this);

  if (QApplication::instance()->arguments().size() >= 2) {
    QString fileName = QApplication::instance()->arguments().at(1);
    _file = cxxmidi::File(fileName.toStdString().c_str());
  }

  _fileModel.SetFile(&_file);
  _fileView.setModel(&_fileModel);

  connect(&_fileView, SIGNAL(activated(QModelIndex)), this,
          SLOT(OnTrackSelected(QModelIndex)));
  connect(&_fileView, SIGNAL(clicked(QModelIndex)), this,
          SLOT(OnTrackSelected(QModelIndex)));
  connect(&_fileView, SIGNAL(requestAddTrack(int)), this,
          SLOT(OnRequestAddTrack(int)));
  connect(&_fileView, SIGNAL(requestDeleteTrack(int)), this,
          SLOT(OnRequestDeleteTrack(int)));
  connect(&_trackView, SIGNAL(requestAddEvent(int)), this,
          SLOT(OnRequestAddEvent(int)));
  connect(&_trackView, SIGNAL(requestDeleteEvent(int)), this,
          SLOT(OnRequestDeleteEvent(int)));

  _trackModel.SetTrack(0);
  _trackView.setModel(&_trackModel);

  QSplitter *splitter = new QSplitter;
  splitter->addWidget(&_fileView);
  splitter->addWidget(&_trackView);
  splitter->setStretchFactor(1, 2);

  this->setCentralWidget(splitter);

  this->CreateMenu();
}

void MainWindow::CreateMenu() {
  QMenu *fileMenu = this->menuBar()->addMenu(tr("&File"));

  QAction *action = fileMenu->addAction(tr("&Open"));
  connect(action, SIGNAL(triggered()), this, SLOT(OnOpenFile()));

  action = fileMenu->addAction(tr("&Save as"));
  connect(action, SIGNAL(triggered()), this, SLOT(OnSaveAs()));

  fileMenu->addSeparator();

  action = fileMenu->addAction(tr("&Exit"));
  QObject::connect(action, SIGNAL(triggered()), QApplication::instance(),
                   SLOT(quit()));
  //! @TODO ask if save changes
}

void MainWindow::OnRequestAddTrack(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  _trackModel.SetTrack(0);  // address may change
  _fileModel.AddTrack(num);
}

void MainWindow::OnRequestDeleteTrack(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  _trackModel.SetTrack(0);  // address may change
  _fileModel.RemoveTrack(num);
}

void MainWindow::OnRequestAddEvent(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  _trackModel.AddEvent(num);
}

void MainWindow::OnRequestDeleteEvent(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  _trackModel.RemoveEvent(num);
}

void MainWindow::OnOpenFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open file"), ".",
      tr("MIDI files (*.mid *.midi);;Any files (*)"));

  _file = cxxmidi::File(fileName.toStdString().c_str());
  _fileModel.SetFile(&_file);
}

void MainWindow::OnSaveAs() {
  QFileDialog dialog(NULL, tr("Save As"));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setNameFilter(tr("MIDI files (*.mid *.midi);;Any files (*)"));
  if (dialog.exec()) {
    QString fileName = dialog.selectedFiles().at(0);
    if (QFileInfo(fileName).suffix().isEmpty()) fileName += ".mid";

    //! @TODO ask if overwrite

    _file.SaveAs(fileName.toStdString().c_str());
  }
}

void MainWindow::OnTrackSelected(QModelIndex index) {
  _trackModel.SetTrack(&_file.at(index.row()));
  _trackView.setModel(&_trackModel);
}

MainWindow::~MainWindow() { delete _ui; }
