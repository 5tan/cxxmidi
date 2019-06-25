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

#ifndef EXAMPLES_QTMIDIEDITOR_MAINWINDOW_H_
#define EXAMPLES_QTMIDIEDITOR_MAINWINDOW_H_

#include <QMainWindow>

#include "filemodel.h"   // NOLINT(build/include_subdir) INCLUDE_NO_DIR
#include "fileview.h"    // NOLINT(build/include_subdir) INCLUDE_NO_DIR
#include "trackmodel.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR
#include "trackview.h"   // NOLINT(build/include_subdir) INCLUDE_NO_DIR

#include <cxxmidi/file.hpp>

namespace Ui {
class MainWindow;
}  // namespace Ui

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:  // NOLINT(whitespace/indent) SIGNAL_SLOT_SPECIFIER
  void OnTrackSelected(QModelIndex index);
  void OnOpenFile();
  void OnSaveAs();
  void OnRequestAddTrack(int num);
  void OnRequestDeleteTrack(int num);
  void OnRequestAddEvent(int num);
  void OnRequestDeleteEvent(int num);

 private:
  void CreateMenu();

  Ui::MainWindow *ui_;

  FileModel file_model_;
  FileView file_view_;
  TrackModel track_model_;
  TrackView track_view_;

  cxxmidi::File _file;
};

#endif  // EXAMPLES_QTMIDIEDITOR_MAINWINDOW_H_
