#include <QApplication>
#include "mainwindow.h"
#include <QScreen>

//#include <QDebug>
//#include <QLoggingCategory>


int main(int argc, char *argv[]) {

    // Imposta gli attributi prima di creare l'istanza di QApplication
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.*=true"));

    

    QApplication app(argc, argv);

    // Imposta il DPI manualmente
    foreach (QScreen *screen, QGuiApplication::screens()) {
        screen->setProperty("logicalDpiX", 96);
        screen->setProperty("logicalDpiY", 96);
    }

    MyProject::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
