// File: src/dynamometer.cpp

#include "dynamometer.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>

Dynamometer::Dynamometer(QWidget *parent) :
    QWidget(parent),
    m_value(0),
    m_x(0),
    m_y(0),
    m_gauge(":/images/dynamometer_forTest.png") {}

void Dynamometer::setValue(int value) {
    if (value != m_value) {
        m_value = value;
        update();
    }
}

void Dynamometer::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
    update();
}

QPoint Dynamometer::getPosition() const {
    return QPoint(m_x, m_y);
}

void Dynamometer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    // Usa le coordinate specificate per disegnare l'immagine
    painter.drawPixmap(m_x, m_y, m_gauge);

}

