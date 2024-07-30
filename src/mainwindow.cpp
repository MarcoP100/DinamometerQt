#include "mainwindow.h"
#include "backgroundwidget.h"
#include "dynamometer.h"
#include <QGridLayout>
#include <QTimer>
#include <QStackedWidget>
#include <QDebug>

namespace MyProject {


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
    , m_backgroundWidget1(new BackgroundWidget(this))
    , m_dynamometer(new Dynamometer(m_backgroundWidget1))
     {


   

    // Assicurati che la finestra principale supporti la trasparenza
    //setAttribute(Qt::WA_TranslucentBackground);
    //setAttribute(Qt::WA_NoSystemBackground);
    //setAutoFillBackground(false);
    //setStyleSheet("background:transparent;");
     // Imposta uno sfondo temporaneo rosso per distinguere meglio
    //setStyleSheet("background-color: red;");

    // Configura il QStackedWidget per la trasparenza
    stackedWidget->setAttribute(Qt::WA_TranslucentBackground);
    stackedWidget->setAttribute(Qt::WA_NoSystemBackground);
    stackedWidget->setAutoFillBackground(false);
    stackedWidget->setStyleSheet("background:transparent;");

     // Configura il BackgroundWidget per la trasparenza
    /*m_backgroundWidget1->setAttribute(Qt::WA_TranslucentBackground);
    m_backgroundWidget1->setAttribute(Qt::WA_NoSystemBackground);
    m_backgroundWidget1->setAutoFillBackground(false);
    m_backgroundWidget1->setStyleSheet("background:transparent;");*/
    

    // Impostare le dimensioni della finestra
    setFixedSize(800, 480);
    setWindowFlags(Qt::FramelessWindowHint);

    // Pagina 1 con Dynamometer
    //m_backgroundWidget1 = new BackgroundWidget(stackedWidget);
    m_backgroundWidget1->setUseSvg(false);
    
    stackedWidget->addWidget(m_backgroundWidget1);
   int diameterDinamometer = 350;
    int chromeRingWidht = 10;
    int x_centerDin = width()/2;
    int y_centerDin = height()/2;
    int x = x_centerDin - (diameterDinamometer / 2) - chromeRingWidht;
    int y = y_centerDin - (diameterDinamometer / 2) - chromeRingWidht;
    int widgetDinWidth  = diameterDinamometer + (chromeRingWidht * 2);
    int widgetDinHeight  = diameterDinamometer + (chromeRingWidht * 2);
    m_dynamometer->setStyleSheet("background: transparent;");
    m_dynamometer->setGeometry(x, y, widgetDinWidth, widgetDinHeight); // Posiziona Dynamometer
    
    
    qDebug() << "stackedWidget";
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

    m_dynamometer->setlargeTack(12,150,2,5);
    m_dynamometer->setsmallTack(7,150,2,2);

    int numberRadius = (diameterDinamometer / 2) - 38;
    m_dynamometer->setNumberRadius(numberRadius);
    m_dynamometer->setInnerRing(120,2);
    m_dynamometer->setNeedle(45.0, Qt::yellow);
    qDebug() << "fine metodi";
    // aggiornamento ghiera
   m_dynamometer->applyUpdates();
   setCentralWidget(stackedWidget);
    qDebug() << "set widget";
    m_dynamometer->setShowNeedle(true);
    //m_dynamometer->setAngleNeedle(0);

    m_dynamometer->setNotchLenght(2);
    m_dynamometer->setShowNotch(true);
    //m_dynamometer->setAngleNotch(0);

    // Simulazione aggiornamento valore
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        static int value = 0;
        static int directionMov = 0;
        if (value <= -240.0)
            directionMov = 0;
        else if (value >= 60.0)
            directionMov = 1;
        if (directionMov == 1)
            value -= 5;
        if (directionMov == 0)
            value += 5;
        m_dynamometer->setAngleNeedle(value);
    });
    timer->start(100);


    QTimer *timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, [this]() {
        static int value2 = 0;
        static int directionMov2 = 0;
        if (value2 <= -240.0)
            directionMov2 = 0;
        else if (value2 >= 60.0)
            directionMov2 = 1;
        if (directionMov2 == 1)
            value2 -= 2;
        if (directionMov2 == 0)
            value2 += 2;
        m_dynamometer->setAngleNotch(value2);
    });
    timer2->start(100);

    
}

MainWindow::~MainWindow() {

}

} // namespace MyProject
