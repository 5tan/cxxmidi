#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent_) :
    QMainWindow(parent_),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    if(QApplication::instance()->arguments().size() >=2 )
    {
        QString fileName =QApplication::instance()->arguments().at(1);
        _file = CxxMidi::File(fileName.toStdString().c_str());
    }

    _fileModel.setFile(&_file);
    _fileView.setModel(&_fileModel);

    connect(&_fileView,SIGNAL(activated(QModelIndex)),
            this,SLOT(onTrackSelected(QModelIndex)));
    connect(&_fileView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(onTrackSelected(QModelIndex)));
    connect(&_fileView,SIGNAL(requestAddTrack(int)),
            this,SLOT(onRequestAddTrack(int)));
    connect(&_fileView,SIGNAL(requestDeleteTrack(int)),
            this,SLOT(onRequestDeleteTrack(int)));
    connect(&_trackView,SIGNAL(requestAddEvent(int)),
            this,SLOT(onRequestAddEvent(int)));
    connect(&_trackView,SIGNAL(requestDeleteEvent(int)),
            this,SLOT(onRequestDeleteEvent(int)));

    _trackModel.setTrack(0);
    _trackView.setModel(&_trackModel);

    QSplitter * splitter = new QSplitter;
    splitter->addWidget(&_fileView);
    splitter->addWidget(&_trackView);
    splitter->setStretchFactor(1,2);

    this->setCentralWidget(splitter);

    this->createMenu();
}

void MainWindow::createMenu()
{
    QMenu* fileMenu= this->menuBar()->addMenu(tr("&File"));

    QAction *action = fileMenu->addAction(tr("&Open"));
    connect(action,SIGNAL(triggered()),
            this,SLOT(onOpenFile()));

    action = fileMenu->addAction(tr("&Save as"));
    connect(action,SIGNAL(triggered()),
            this,SLOT(onSaveAs()));

    fileMenu->addSeparator();

    action = fileMenu->addAction(tr("&Exit"));
    QObject::connect(action,SIGNAL(triggered()),
                     QApplication::instance(),SLOT(quit()));
    //! @TODO ask if save changes
}

void MainWindow::onRequestAddTrack(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.setTrack(0); // address may change
    _fileModel.addTrack(num_);
}

void MainWindow::onRequestDeleteTrack(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.setTrack(0); // address may change
    _fileModel.removeTrack(num_);
}

void MainWindow::onRequestAddEvent(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.addEvent(num_);
}

void MainWindow::onRequestDeleteEvent(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.removeEvent(num_);
}

void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open file"), ".", tr("MIDI files (*.mid *.midi);;Any files (*)"));

    _file = CxxMidi::File(fileName.toStdString().c_str());
    _fileModel.setFile(&_file);
}

void MainWindow::onSaveAs()
{
    QFileDialog dialog(NULL, tr("Save As"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("MIDI files (*.mid *.midi);;Any files (*)"));
    if (dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);
        if(QFileInfo(fileName).suffix().isEmpty())
            fileName += ".mid";

        //! @TODO ask if overwrite

        _file.saveAs(fileName.toStdString().c_str());
    }
}

void MainWindow::onTrackSelected(QModelIndex index_)
{
    _trackModel.setTrack(&_file.at(index_.row()));
    _trackView.setModel(&_trackModel);
}

MainWindow::~MainWindow()
{
    delete _ui;
}
