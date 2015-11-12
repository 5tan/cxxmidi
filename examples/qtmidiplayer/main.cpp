#include <QApplication>

#include "mainwindow.h"

int main(int argc, char ** argv)
{
    qRegisterMetaType<CxxMidi::Time::Point>("CxxMidi::Time::Point");

    QApplication app(argc,argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
