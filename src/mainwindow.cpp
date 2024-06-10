// File: src/mainwindow.cpp

#include "../include/mainwindow.h"
#include "backgroundwidget.h"
#include "dynamometer.h"
#include <QGridLayout>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_backgroundWidget(new BackgroundWidget(this)),
    m_dynamometer(new Dynamometer(this)) {

    // Impostare le dimensioni della finestra
    setFixedSize(800, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Usa QGridLayout per sovrapporre i widget
    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    gridLayout->addWidget(m_backgroundWidget, 0, 0);
    gridLayout->addWidget(m_dynamometer, 0, 0);

    // Imposta le posizioni delle ghiere
    m_dynamometer->setPosition(100, 100); // Posizione della prima ghiera

    // Imposta altri parametri delle ghiere
    m_dynamometer->setMaxValue(60);
    m_dynamometer->setTackCount(12);
    m_dynamometer->setShowNeedle(true);

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
