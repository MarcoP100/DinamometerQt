#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "backgroundwidget.h"
#include "dynamometer.h"


namespace MyProject {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    BackgroundWidget *m_backgroundWidget1;
    Dynamometer *m_dynamometer;

};

}

#endif // MAINWINDOW_H

