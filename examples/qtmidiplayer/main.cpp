#include <QApplication>

#include "mainwindow.h"

int main(int argc, char** argv) {
  qRegisterMetaType<std::chrono::microseconds>("std::chrono::microseconds");

  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
