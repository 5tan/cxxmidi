#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "filemodel.h"
#include "fileview.h"
#include "trackmodel.h"
#include "trackview.h"

#include <cxxmidi/file.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:
  void OnTrackSelected(QModelIndex index_);
  void OnOpenFile();
  void OnSaveAs();
  void OnRequestAddTrack(int num_);
  void OnRequestDeleteTrack(int num_);
  void OnRequestAddEvent(int num_);
  void OnRequestDeleteEvent(int num_);

 private:
  void CreateMenu();

  Ui::MainWindow *_ui;

  FileModel _fileModel;
  FileView _fileView;
  TrackModel _trackModel;
  TrackView _trackView;

  cxxmidi::File _file;
};

#endif  // MAINWINDOW_H
