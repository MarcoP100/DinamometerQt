// File: include/mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "backgroundwidget.h"
#include "dynamometer.h"


namespace MyProject {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MyProject::BackgroundWidget *m_backgroundWidget;
    MyProject::Dynamometer *m_dynamometer;
    void setupUi();
};

}

#endif // MAINWINDOW_H

