#include "dynamometer.h"
#include <QPainter>
#include <QPaintEvent>
#include <QConicalGradient>

Dynamometer::Dynamometer(QWidget *parent) :
    QWidget(parent),
    m_value(0),
    m_maxValue(50),
    m_tackCount(10),
    m_showNeedle(true),
    m_x(0),
    m_y(0),
    m_cacheDirty(true) {} // Initialize m_cacheDirty to true

void Dynamometer::setValue(int value) {
    if (value != m_value) {
        m_value = value;
        update();
    }
}

void Dynamometer::setMaxValue(int maxValue) {
    m_maxValue = maxValue;
    m_cacheDirty = true;
    update();
}

void Dynamometer::setTackCount(int count) {
    m_tackCount = count;
    m_cacheDirty = true;
    update();
}

void Dynamometer::setShowNeedle(bool show) {
    m_showNeedle = show;
    update();
}

void Dynamometer::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
    update();
}

void Dynamometer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

        QPainter painter(this);

        if (m_cacheDirty) {
            generateGaugeCache();
            m_cacheDirty = false;
        }

        // Disegna la cache della ghiera centrata nel widget
        int x = (width() - m_gaugeCache.width()) / 2;
        int y = (height() - m_gaugeCache.height()) / 2;
        painter.drawPixmap(0, 0, m_gaugeCache);

        // Disegna la lancetta se necessario
        /*if (m_showNeedle) {
            drawNeedle(painter);
        }*/
}

void Dynamometer::generateGaugeCache() {
    m_gaugeCache = QPixmap(size());
    m_gaugeCache.fill(Qt::transparent); // Rende la cache trasparente

    QPainter painter(&m_gaugeCache);

    // Disegna la sfumatura di sfondo
    drawGradientBackground(painter);

     // Disegna la ghiera
    //drawGauge(painter);

    // Disegna le tacche
    //drawTacks(painter);
}

void Dynamometer::drawGradientBackground(QPainter &painter) {
    QConicalGradient gradient(rect().center(), -90);

        // Aggiungiamo più colori per una sfumatura più dettagliata
        gradient.setColorAt(0.0, QColor(128, 179, 255));
        gradient.setColorAt(0.25, QColor(102, 153, 255));
        gradient.setColorAt(0.5, QColor(77, 128, 255));
        gradient.setColorAt(0.75, QColor(51, 102, 255));
        gradient.setColorAt(1.0, QColor(26, 77, 255));

        painter.setBrush(QBrush(gradient));
        painter.setPen(Qt::NoPen);

        int diameter = qMin(width(), height());
        QRect gradientRect((width() - diameter) / 2, (height() - diameter) / 2, diameter, diameter);
        painter.drawEllipse(gradientRect);
}

void Dynamometer::drawGauge(QPainter &painter) {
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 4));
    int diameter = qMin(width(), height()) - 40;
    QRect gaugeRect((width() - diameter) / 2, (height() - diameter) / 2, diameter, diameter);
    painter.drawEllipse(gaugeRect);
}

void Dynamometer::drawNeedle(QPainter &painter) {
    painter.save();

    int side = qMin(width(), height());
    int x = width() / 2;
    int y = height() / 2;

    painter.translate(x, y);
    painter.rotate(180.0 * m_value / m_maxValue);
    painter.setBrush(Qt::yellow);

    static const QPoint needle[3] = {
        QPoint(0, -10),
        QPoint(5, 0),
        QPoint(0, side / 2 - 10)
    };
    painter.drawConvexPolygon(needle, 3);
    painter.restore();
}

void Dynamometer::drawTacks(QPainter &painter) {
    painter.save();
    int side = qMin(width(), height());
    painter.translate(width() / 2 + m_x, height() / 2 + m_y);
    painter.setPen(QPen(Qt::white, 2));
    for (int i = 0; i < m_tackCount; ++i) {
        painter.drawLine(0, -side / 2 + 10, 0, -side / 2 + 20);
        painter.rotate(360.0 / m_tackCount);
    }
    painter.restore();
}
