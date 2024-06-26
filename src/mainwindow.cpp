#include "mainwindow.h"
#include "backgroundwidget.h"
#include "dynamometer.h"
#include <QGridLayout>
#include <QTimer>
#include <QStackedWidget>

namespace MyProject {


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
    , m_backgroundWidget1(new BackgroundWidget(this))
    , m_dynamometer(new Dynamometer(m_backgroundWidget1))
     {

    // Impostare le dimensioni della finestra
    setFixedSize(800, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    // Pagina 1 con Dynamometer
    //m_backgroundWidget1 = new BackgroundWidget(stackedWidget);
    m_backgroundWidget1->setUseSvg(false);

    int diameterDinamometer = 400;
    int chromeRingWidht = 25;
    int x_centerDin = width()/2;
    int y_centerDin = height()/2;
    int x = x_centerDin - (diameterDinamometer / 2) - chromeRingWidht;
    int y = y_centerDin - (diameterDinamometer / 2) - chromeRingWidht;
    int widgetDinWidth  = diameterDinamometer + (chromeRingWidht * 2);
    int widgetDinHeight  = diameterDinamometer + (chromeRingWidht * 2);

    m_dynamometer->setGeometry(x, y, widgetDinWidth, widgetDinHeight); // Posiziona Dynamometer
    stackedWidget->addWidget(m_backgroundWidget1);

    // imposta il diametro del dinamometro
    m_dynamometer->setDiameter(diameterDinamometer);
    // Imposta le posizioni delle ghiere
    m_dynamometer->setPositionCenter(x_centerDin, y_centerDin);
    // Imposta la visibilità e i parametri dell'anello cromato
    m_dynamometer->setShowChromeRing(true);
    m_dynamometer->setChromeRingWidth(chromeRingWidht);
    // Imposta altri parametri delle ghiere
    m_dynamometer->setMaxValue(50);
    m_dynamometer->setLargeTacksCount(11);
    m_dynamometer->setSmallTacksBetweenCount(3);
    m_dynamometer->setStartAngle(-240.0);
    m_dynamometer->setEndAngle(60.0);

    m_dynamometer->setlargeTack(20,150,2,5);
    m_dynamometer->setsmallTack(10,150,2,2);

    int numberRadius = (diameterDinamometer / 2) - 50;
    m_dynamometer->setNumberRadius(numberRadius);
    m_dynamometer->setInnerRing(125,4);
    m_dynamometer->setNeedle(45.0, Qt::yellow);

    // aggiornamento ghiera
    m_dynamometer->applyUpdates();
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
