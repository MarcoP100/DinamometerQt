#include "mainwindow.h"
#include "backgroundwidget.h"
#include "dynamometer.h"
#include <QGridLayout>
#include <QTimer>
#include <QStackedWidget>

namespace MyProject {


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this)) {

    // Impostare le dimensioni della finestra
    setFixedSize(800, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    // Pagina 1 con Dynamometer
    m_backgroundWidget1 = new BackgroundWidget(stackedWidget);
    m_backgroundWidget1->setUseSvg(false);

    m_dynamometer = new Dynamometer(m_backgroundWidget1);
    int diameterDinamometer = 400;
    int x_centerDin = width()/2;
    int y_centerDin = height()/2;
    int x = x_centerDin - (diameterDinamometer / 2);
    int y = y_centerDin - (diameterDinamometer / 2);

    m_dynamometer->setGeometry(x, y, diameterDinamometer, diameterDinamometer); // Posiziona Dynamometer
    stackedWidget->addWidget(m_backgroundWidget1);

    // imposta il diametro del dinamometro
    m_dynamometer->setDiameter(diameterDinamometer);

    // Imposta le posizioni delle ghiere
    m_dynamometer->setPositionCenter(x_centerDin, y_centerDin);

    // Imposta altri parametri delle ghiere
   // m_dynamometer->setMaxValue(60);
   // m_dynamometer->setTackCount(12);
   // m_dynamometer->setShowNeedle(true);

    setCentralWidget(stackedWidget);



    // Simulazione aggiornamento valore
    /*QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        static int value = 0;
        value = (value + 1) % 100;
        m_dynamometer->setValue(value);
    });
    timer->start(1000);*/
}

MainWindow::~MainWindow() {
}

} // namespace MyProject
