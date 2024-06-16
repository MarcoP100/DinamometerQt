#include "dynamometer.h"
#include <QPainter>
#include <QPaintEvent>
#include <QRadialGradient>
#include <QColor>
#include <QDebug>

#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

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
    QPointF center(m_gaugeCache.width() / 2, m_gaugeCache.height() / 2);
    float innerRadius = m_diameter / 2;
    float outerRadius = innerRadius + m_chromeRingWidth;

    QImage ringImage(m_gaugeCache.width(), m_gaugeCache.height(), QImage::Format_ARGB32);
    ringImage.fill(Qt::transparent);
    QPainter ringPainter(&ringImage);


    for (int i = 0; i < m_chromeRingWidth; ++i) {
        int colorValue = 200 * (1 - static_cast<double>(i) / m_chromeRingWidth);
        QColor color(colorValue, colorValue, colorValue);
        ringPainter.setPen(QPen(color));
        //ringPainter.setRenderHint(QPainter::Antialiasing);
        ringPainter.drawEllipse(center, outerRadius - i, outerRadius - i);
    }



    // Apply a slight blur to the gradient
    QImage blurredImage = ringImage;
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(3);
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(blurredImage));
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(m_gaugeCache.width(), m_gaugeCache.height(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter finalPainter(&result);
    scene.render(&finalPainter);

    // Enhance the contrast to make it more metallic
    QImage enhancedImage = result;
    QPainter enhancePainter(&enhancedImage);
    enhancePainter.setCompositionMode(QPainter::CompositionMode_Multiply);


    // Creare una maschera per l'anello
    QRegion outerRegion = QRegion(center.x() - outerRadius, center.y() - outerRadius, outerRadius * 2, outerRadius * 2, QRegion::Ellipse);
    QRegion innerRegion = QRegion(center.x() - innerRadius, center.y() - innerRadius, innerRadius * 2, innerRadius * 2, QRegion::Ellipse);
    QRegion ringRegion = outerRegion.subtracted(innerRegion);
    enhancePainter.setClipRegion(ringRegion);

    enhancePainter.fillRect(enhancedImage.rect(), QColor(200, 200, 200));
    enhancePainter.end();

       painter.setRenderHint(QPainter::Antialiasing);
    painter.drawImage(0, 0, enhancedImage);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, innerRadius, innerRadius);

    // Pulire la memoria allocata
    delete blur;

}

Dynamometer::~Dynamometer() {

    // Dealloca le risorse qui, se necessario
    qDebug() << "Dynamometer distrutto";
}

}
