#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MyProject::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
