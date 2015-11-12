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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent_ = 0);
    ~MainWindow();

private slots:
    void onTrackSelected(QModelIndex index_);
    void onOpenFile();
    void onSaveAs();
    void onRequestAddTrack(int num_);
    void onRequestDeleteTrack(int num_);
    void onRequestAddEvent(int num_);
    void onRequestDeleteEvent(int num_);

private:

    void createMenu();

    Ui::MainWindow *_ui;

    FileModel _fileModel;
    FileView _fileView;
    TrackModel _trackModel;
    TrackView _trackView;

    CxxMidi::File _file;
};

#endif // MAINWINDOW_H
