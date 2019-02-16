#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent_)
    : QMainWindow(parent_), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);

  if (QApplication::instance()->arguments().size() >= 2) {
    QString file_name = QApplication::instance()->arguments().at(1);
    _file = cxxmidi::File(file_name.toStdString().c_str());
  }

  file_model_.SetFile(&_file);
  file_view_.setModel(&file_model_);

  connect(&file_view_, SIGNAL(activated(QModelIndex)), this,
          SLOT(OnTrackSelected(QModelIndex)));
  connect(&file_view_, SIGNAL(clicked(QModelIndex)), this,
          SLOT(OnTrackSelected(QModelIndex)));
  connect(&file_view_, SIGNAL(requestAddTrack(int)), this,
          SLOT(OnRequestAddTrack(int)));
  connect(&file_view_, SIGNAL(requestDeleteTrack(int)), this,
          SLOT(OnRequestDeleteTrack(int)));
  connect(&track_view_, SIGNAL(requestAddEvent(int)), this,
          SLOT(OnRequestAddEvent(int)));
  connect(&track_view_, SIGNAL(requestDeleteEvent(int)), this,
          SLOT(OnRequestDeleteEvent(int)));

  track_model_.SetTrack(0);
  track_view_.setModel(&track_model_);

  QSplitter *splitter = new QSplitter;
  splitter->addWidget(&file_view_);
  splitter->addWidget(&track_view_);
  splitter->setStretchFactor(1, 2);

  this->setCentralWidget(splitter);

  this->CreateMenu();
}

void MainWindow::CreateMenu() {
  QMenu *file_menu = this->menuBar()->addMenu(tr("&File"));

  QAction *action = file_menu->addAction(tr("&Open"));
  connect(action, SIGNAL(triggered()), this, SLOT(OnOpenFile()));

  action = file_menu->addAction(tr("&Save as"));
  connect(action, SIGNAL(triggered()), this, SLOT(OnSaveAs()));

  file_menu->addSeparator();

  action = file_menu->addAction(tr("&Exit"));
  QObject::connect(action, SIGNAL(triggered()), QApplication::instance(),
                   SLOT(quit()));
  //! @TODO ask if save changes
}

void MainWindow::OnRequestAddTrack(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  track_model_.SetTrack(0);  // address may change
  file_model_.AddTrack(num);
}

void MainWindow::OnRequestDeleteTrack(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  track_model_.SetTrack(0);  // address may change
  file_model_.RemoveTrack(num);
}

void MainWindow::OnRequestAddEvent(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  track_model_.AddEvent(num);
}

void MainWindow::OnRequestDeleteEvent(int num) {
  //! @TODO there should be a command history (Undo/Redo)

  track_model_.RemoveEvent(num);
}

void MainWindow::OnOpenFile() {
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open file"), ".",
      tr("MIDI files (*.mid *.midi);;Any files (*)"));

  _file = cxxmidi::File(file_name.toStdString().c_str());
  file_model_.SetFile(&_file);
}

void MainWindow::OnSaveAs() {
  QFileDialog dialog(NULL, tr("Save As"));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setNameFilter(tr("MIDI files (*.mid *.midi);;Any files (*)"));
  if (dialog.exec()) {
    QString file_name = dialog.selectedFiles().at(0);
    if (QFileInfo(file_name).suffix().isEmpty()) file_name += ".mid";

    //! @TODO ask if overwrite

    _file.SaveAs(file_name.toStdString().c_str());
  }
}

void MainWindow::OnTrackSelected(QModelIndex index) {
  track_model_.SetTrack(&_file.at(index.row()));
  track_view_.setModel(&track_model_);
}

MainWindow::~MainWindow() { delete ui_; }
