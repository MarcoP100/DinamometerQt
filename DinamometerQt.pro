QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DinamometerQt
TEMPLATE = app

SOURCES += src/main.cpp \
           src/backgroundwidget.cpp \
           src/mainwindow.cpp \
           src/dynamometer.cpp

HEADERS += include/mainwindow.h \
           include/backgroundwidget.h \
           include/dynamometer.h

RESOURCES += resources/images.qrc

INCLUDEPATH += include
