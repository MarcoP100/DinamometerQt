#include <QApplication>
#include "mainwindow.h"
#include <QSurfaceFormat>

//#include <QDebug>
//#include <QLoggingCategory>


int main(int argc, char *argv[]) {

     QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    //format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setVersion(3, 2); // Assicurati di usare una versione di OpenGL che supporti la trasparenza
    QSurfaceFormat::setDefaultFormat(format);

    MyProject::MainWindow mainWindow;
     mainWindow.show();
     return app.exec();
    
}
