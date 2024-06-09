// File: include/mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backgroundwidget.h"
#include "dynamometer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    BackgroundWidget *m_backgroundWidget;
    Dynamometer *m_dynamometer;
};

#endif // MAINWINDOW_H

