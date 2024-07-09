#include <QApplication>
#include "mainwindow.h"

//#include <QDebug>
//#include <QLoggingCategory>


int main(int argc, char *argv[]) {

    //QLoggingCategory::setFilterRules(QStringLiteral("qt.*=true"));

    QApplication app(argc, argv);

    MyProject::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
