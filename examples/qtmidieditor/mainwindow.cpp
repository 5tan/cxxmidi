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

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>

#include "mainwindow.h"     // NOLINT(build/include_subdir) INCLUDE_NO_DIR
#include "ui_mainwindow.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR

MainWindow::MainWindow(QWidget *parent_)
    : QMainWindow(parent_), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);

  if (QApplication::instance()->arguments().size() >= 2) {
    QString file_name = QApplication::instance()->arguments().at(1);
    _file = cxxmidi::File(file_name.toStdString().c_str());
  }

  file_model_.SetFile(&_file);
  file_view_.setModel(&file_model_);

  connect(&file_view_, &FileView::activated, this,
          &MainWindow::OnTrackSelected);
  connect(&file_view_, &FileView::clicked, this, &MainWindow::OnTrackSelected);
  connect(&file_view_, &FileView::RequestAddTrack, this,
          &MainWindow::OnRequestAddTrack);
  connect(&file_view_, &FileView::RequestDeleteTrack, this,
          &MainWindow::OnRequestDeleteTrack);
  connect(&track_view_, &TrackView::RequestAddEvent, this,
          &MainWindow::OnRequestAddEvent);
  connect(&track_view_, &TrackView::RequestDeleteEvent, this,
          &MainWindow::OnRequestDeleteEvent);

  track_model_.SetTrack(0);
  track_view_.setModel(&track_model_);

  QSplitter *splitter = new QSplitter;
  splitter->addWidget(&file_view_);
  splitter->addWidget(&track_view_);
  splitter->setStretchFactor(1, 2);

  setCentralWidget(splitter);

  CreateMenu();
}

void MainWindow::CreateMenu() {
  QMenu *file_menu = menuBar()->addMenu(tr("&File"));

  QAction *action = file_menu->addAction(tr("&Open"));
  connect(action, &QAction::triggered, this, &MainWindow::OnOpenFile);

  action = file_menu->addAction(tr("&Save as"));
  connect(action, &QAction::triggered, this, &MainWindow::OnSaveAs);

  file_menu->addSeparator();

  action = file_menu->addAction(tr("&Exit"));
  QObject::connect(action, &QAction::triggered, QApplication::instance(),
                   &QApplication::quit);
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
