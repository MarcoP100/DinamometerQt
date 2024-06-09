# File: DinamometerQt.pro

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DinamometerQt
TEMPLATE = app

SOURCES += src/main.cpp \
           src/mainwindow.cpp

HEADERS += include/mainwindow.h

FORMS += src/mainwindow.ui \
    src/mainwindow.ui

# Directory per le immagini
RESOURCES += images.qrc

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
