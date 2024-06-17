#include "dynamometer.h"
#include <QPainter>
#include <QPaintEvent>
#include <QRadialGradient>
#include <QColor>
#include <QDebug>

namespace MyProject {
Dynamometer::Dynamometer(QWidget *parent) :
    QWidget(parent),
    m_value(0),
    m_maxValue(50),
    m_tackCount(10),
    m_showNeedle(true),
    m_diameter(0),
    m_cacheDirty(true),
    m_showChromeRing(true),
    m_chromeRingWidth(10),
    m_chromeRingColor(Qt::gray){

    qDebug() << "Dynamometer creato";
    } // Initialize m_cacheDirty to true

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

void Dynamometer::setDiameter(int diameter) {
    m_diameter = diameter;
    m_cacheDirty = true;
    update();
}

void Dynamometer::setShowNeedle(bool show) {
    m_showNeedle = show;
    update();
}

void Dynamometer::setPositionCenter(int x, int y) {
    m_center = QPoint(x, y);
    m_cacheDirty = true;
    update(); // Forza il ridisegno del widget
}


void Dynamometer::setShowChromeRing(bool show) {
    m_showChromeRing = show;
    m_cacheDirty = true;
    update();
}

void Dynamometer::setChromeRingWidth(int width) {
    m_chromeRingWidth = width;
    m_cacheDirty = true;
    update();
}

void Dynamometer::setChromeRingColor(const QColor &color) {
    m_chromeRingColor = color;
    m_cacheDirty = true;
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

    // Disegna l'anello cromato
    if (m_showChromeRing) {
        drawChromeRing(painter);
    }
}

void Dynamometer::drawGradientBackground(QPainter &painter) {

    // Abilitare l'anti-aliasing
    painter.setRenderHint(QPainter::Antialiasing);

    // Centro del gradiente
    QPointF center(m_gaugeCache.width()/2, m_gaugeCache.height()/2);


    // Raggio del gradiente
    qreal radius = m_diameter / 2;

    QRadialGradient  gradient(center, radius);

    // Aggiungiamo più colori per una sfumatura più dettagliata
    gradient.setColorAt(0.0, QColor(128, 179, 255));
    gradient.setColorAt(0.25, QColor(102, 153, 255));
    gradient.setColorAt(0.5, QColor(77, 128, 255));
    gradient.setColorAt(0.75, QColor(51, 102, 255));
    gradient.setColorAt(1.0, QColor(26, 77, 255));

    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);

    QRect gradientRect(center.x() - radius,center.y() - radius, m_diameter, m_diameter);
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
    /*int side = qMin(width(), height());
   painter.translate(width() / 2 + m_x, height() / 2 + m_y);
    painter.setPen(QPen(Qt::white, 2));
    for (int i = 0; i < m_tackCount; ++i) {
        painter.drawLine(0, -side / 2 + 10, 0, -side / 2 + 20);
        painter.rotate(360.0 / m_tackCount);
    }*/
    painter.restore();;
}



void Dynamometer::drawChromeRing(QPainter &painter) {

    int highResFactor = 2;  // Fattore di risoluzione più alta
    int highResWidth = m_gaugeCache.width() * highResFactor;
    int highResHeight = m_gaugeCache.height() * highResFactor;

    QPointF center(highResWidth / 2, highResHeight / 2);
    float innerRadius = (m_diameter / 2) * highResFactor;
    float outerRadius = innerRadius + (m_chromeRingWidth * highResFactor);

    QImage ringImage(highResWidth, highResHeight, QImage::Format_ARGB32);
    ringImage.fill(Qt::transparent);
    QPainter ringPainter(&ringImage);

    // Abilitare l'anti-aliasing
    ringPainter.setRenderHint(QPainter::Antialiasing, true);

    // Creare il gradiente radiale per l'anello cromato
    QRadialGradient gradient(center, outerRadius);
    float startGradient = innerRadius/outerRadius;
    float stopGradient = outerRadius/outerRadius;
    float centerGradient = (((outerRadius-innerRadius)/2) + innerRadius)/outerRadius;
    gradient.setColorAt((startGradient), QColor(125, 125, 125));
    gradient.setColorAt(centerGradient, QColor(190, 190, 190));
    gradient.setColorAt((stopGradient), QColor(64, 64, 64));

    ringPainter.setBrush(gradient);
    ringPainter.setPen(Qt::NoPen);

    ringPainter.drawEllipse(center, outerRadius, outerRadius);

    // Ritaglia il centro trasparente
    ringPainter.setCompositionMode(QPainter::CompositionMode_Clear);
    ringPainter.drawEllipse(center, innerRadius, innerRadius);

    // Riduci l'immagine a una risoluzione normale
    QImage finalImage = ringImage.scaled(m_gaugeCache.width(), m_gaugeCache.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter.drawImage(0, 0, finalImage);

}

Dynamometer::~Dynamometer() {

    // Dealloca le risorse qui, se necessario
    qDebug() << "Dynamometer distrutto";
}

}
