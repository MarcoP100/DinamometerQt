#include "dynamometer.h"
#include <QPainter>
#include <QPaintEvent>
#include <QRadialGradient>
#include <QColor>
#include <QDebug>
#include <QtMath>
#include <QPainterPath>

namespace MyProject {
Dynamometer::Dynamometer(QWidget *parent) :
    QWidget(parent),
    m_maxValue(50),
    m_showNeedle(true),
    m_diameter(0),
    m_cacheDirty(true),
    m_showChromeRing(true),
    m_chromeRingWidth(10),
    m_startAngle(0),
    m_endAngle(0),
    m_largeTacksCount(0),
    m_smallTacksBetween(0),
    m_largeTack(0,0,0,0),
    m_smallTack(0,0,0,0),
    m_numberRadius(0),
    m_largeTackPosition(0),
    m_innerRingRadius(0),
    m_innerRingWidth(0),
    m_outerRingRadius(0)
    {

    qDebug() << "Dynamometer creato";
    } // Initialize m_cacheDirty to true


void Dynamometer::setMaxValue(int maxValue) {
    m_maxValue = maxValue;
}

void Dynamometer::setShowNeedle(bool show) {
    m_showNeedle = show;
    update();
}

void Dynamometer::setDiameter(int diameter) {
    m_diameter = diameter;
}
void Dynamometer::setPositionCenter(int x, int y) {
    m_center = QPoint(x, y);

}

void Dynamometer::setShowChromeRing(bool show) {
    m_showChromeRing = show;

}

void Dynamometer::setChromeRingWidth(int width) {
    m_chromeRingWidth = width;

}

void Dynamometer::setLargeTacksCount(int count) {
    m_largeTacksCount = count;

}

void Dynamometer::setSmallTacksBetweenCount(int count) {
    m_smallTacksBetween = count;

}

void Dynamometer::setStartAngle(float angle) {
    m_startAngle = angle;
}

void Dynamometer::setEndAngle(float angle) {
    m_endAngle = angle;
}

void Dynamometer::setlargeTack(int length, int shadowTransparency, float shadowOffset, int width){
    m_largeTack = Tack(length,shadowTransparency,shadowOffset,width);

}

void Dynamometer::setsmallTack(int length, int shadowTransparency, float shadowOffset, int width){
    m_smallTack = Tack(length,shadowTransparency,shadowOffset,width);

}

void Dynamometer::setNumberRadius(int position) {
    m_numberRadius = position;
}

void Dynamometer::setInnerRing(int radius, int width){
    m_innerRingRadius = radius;
    m_innerRingWidth = width;
}



void Dynamometer::applyUpdates() {
    m_cacheDirty = true;
    update();

}

void Dynamometer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    if (m_cacheDirty) {
        generateGaugeCache();
        m_cacheDirty = false;
        qDebug() << "Dynamometer aggiornato";
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

    //Disegna tacche e numeri
    drawTacks(painter);
    drawNumbers(painter);
    drawInternalRings(painter);
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

    /*int side = qMin(width(), height());
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
    painter.drawConvexPolygon(needle, 3);*/
    painter.restore();
}

void Dynamometer::drawTacks(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    int x = m_gaugeCache.width() / 2;
    int y = m_gaugeCache.height() / 2;
    m_largeTackPosition = m_diameter / 2 - 5;

    // Calcola l'angolo incrementale tra le tacche
    float largeTackIncrement  = (m_endAngle - m_startAngle) / (m_largeTacksCount - 1);

    // Calcola l'angolo incrementale tra le tacche piccole
    float smallTackIncrement = largeTackIncrement / (m_smallTacksBetween + 1);

    painter.translate(x, y);
    for (int i = 0; i < m_largeTacksCount; ++i) {
        float largeTackAngle = m_startAngle + largeTackIncrement * i;
        m_largeTack.draw(painter, largeTackAngle, m_largeTackPosition);

        if (i < m_largeTacksCount - 1) {
            for (int j = 1; j <= m_smallTacksBetween; ++j) {
                float smallTackAngle = largeTackAngle + smallTackIncrement * j;
                m_smallTack.draw(painter, smallTackAngle, m_largeTackPosition - 2);
            }
        }
    }

    painter.restore();
}



void Dynamometer::drawChromeRing(QPainter &painter) {

    painter.save();
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
    // Disegnare il primo anello (bordo nero)
    ringPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    float ringWidth1 = 2; // Spessore del primo anello
    ringPainter.setPen(QPen(Qt::black, ringWidth1));
    ringPainter.setBrush(Qt::NoBrush);
    ringPainter.drawEllipse(center, innerRadius, innerRadius);


    // Riduci l'immagine a una risoluzione normale
    QImage finalImage = ringImage.scaled(m_gaugeCache.width(), m_gaugeCache.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter.drawImage(0, 0, finalImage);

    painter.restore();
}


void Dynamometer::drawNumbers(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    int x = m_gaugeCache.width() / 2;
    int y = m_gaugeCache.height() / 2;
    float numberRadius = m_numberRadius; // Posizione dei numeri

    // Calcola l'angolo incrementale tra le tacche
    float numbersIncrement  = (m_endAngle - m_startAngle) / (m_largeTacksCount - 1);

    int sizeFont = 22;
    QFont font("Noto Sans Thai UI", sizeFont); // Cambia "Arial" con il font desiderato e 12 con la dimensione desiderata
    //QFont font = painter.font();
    font.setWeight(QFont::Black);
    //font.setBold(true);
    //font.setPointSize(sizeFont); // Imposta la dimensione del font
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.translate(x, y);

    QFontMetrics metrics(font);

    for (int i = 0; i < m_largeTacksCount; ++i) {
        float numbersAngle = m_startAngle + numbersIncrement * i;
        float angleRad = qDegreesToRadians(numbersAngle);

        // calcolo il valore da visualizzare
        int value = static_cast<int>((m_maxValue / (m_largeTacksCount -1)) * i);
        QString text = QString::number(value);

        // posizione iniziale
        float textX = numberRadius * qCos(angleRad);
        float textY = numberRadius * qSin(angleRad);

        // Calcola il rettangolo di testo centrato sulla posizione calcolata


        int textWidth = metrics.horizontalAdvance(text);
        int textHeight = metrics.height();
        //int textAscent = metrics.ascent();
        //QRect textRect = metrics.boundingRect(text);
        QRect textRect(textX - textWidth / 2, textY - textHeight / 2, textWidth, textHeight); // Rettangolo per il testo
        //painter.drawText(textRect, Qt::AlignCenter, text);
        textRect.moveCenter(QPoint(textX, textY));


        // Riempie il rettangolo di testo con un colore semi-trasparente
        //painter.fillRect(textRect, QColor(0, 255, 0, 100));

        // Disegna il contorno rosso
        QPainterPath path;
        //path.addText(textRect.bottomLeft(), font, text);


        QPointF textPosition(textRect.center().x() - (textWidth / 2.0), textRect.center().y() + (sizeFont/2.0));
        path.addText(textPosition, font, text);

        //painter.fillPath(path, QBrush(Qt::red));
        // Disegna il riempimento
        //     painter.setPen(Qt::white);
        //    painter.fillPath(path, Qt::white);


        /*int shadowOffset = 6;
        float shadowX = shadowOffset * qCos(angleRad);  // Sposta l'ombra diagonalmente
        float shadowY = shadowOffset * qSin(angleRad);  // Sposta l'ombra diagonalmente
        //painter.drawText(textRect.translated(shadowX, shadowY), Qt::AlignCenter, text);
        painter.fillPath(path, Qt::white);
        painter.setPen(QPen(QColor(125, 125, 125, 255),1));
        painter.drawPath(path.translated(shadowX, shadowY));*/


        // Disegna il bordo chiaro superiore
        //painter.setPen(QPen(QColor(255, 255, 255, 150), 2));
        //painter.drawText(textRect.translated(-1, -1), Qt::AlignCenter, text);


        QPen pen(Qt::black);
        pen.setWidth(2); // Spessore del contorno
            painter.setPen(pen);
            painter.drawPath(path);

        // Crea il gradiente per il testo
        QLinearGradient gradient(textRect.topLeft(), textRect.bottomRight());
        gradient.setColorAt(0.0, QColor(50, 50, 50));
        gradient.setColorAt(0.5, QColor(255, 255, 255));
        gradient.setColorAt(1.0, QColor(50, 50, 50));
        //painter.setPen(QPen(QBrush(gradient), 1));
        painter.fillPath(path, gradient);
        // Disegna il testo con il gradiente
        //painter.drawText(textRect, Qt::AlignCenter, text);

    }
    painter.restore();
}


void Dynamometer::drawInternalRings(QPainter &painter){
    painter.save();

    int highResFactor = 2;  // Fattore di risoluzione più alta
    int highResWidth = m_gaugeCache.width() * highResFactor;
    int highResHeight = m_gaugeCache.height() * highResFactor;

    QPointF center(highResWidth / 2, highResHeight / 2);
    QImage ringImage(highResWidth, highResHeight, QImage::Format_ARGB32);
    ringImage.fill(Qt::transparent);
    QPainter ringPainter(&ringImage);

    // Abilitare l'anti-aliasing
    ringPainter.setRenderHint(QPainter::Antialiasing, true);

    /* anello esterno */
    m_outerRingRadius = (m_largeTackPosition - m_largeTack.getLength() - m_largeTack.getWidth());
    int outterRingRadiusHR = m_outerRingRadius * highResFactor;
    ringPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    int ringWidth1 = 1; // Spessore del primo anello
    ringPainter.setPen(QPen(Qt::black, ringWidth1  * highResFactor));
    ringPainter.setBrush(Qt::NoBrush);
    ringPainter.drawEllipse(center, outterRingRadiusHR, outterRingRadiusHR);


    /* anello interno */
    //ringPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    int innerRingRadiusHR = m_innerRingRadius * highResFactor;
    int innerRingWidthHR = m_innerRingWidth * highResFactor;
    QLinearGradient gradient(center.x() - innerRingRadiusHR, center.y() - innerRingRadiusHR, center.x() + innerRingRadiusHR, center.y() + innerRingRadiusHR);
    gradient.setColorAt(0.0, QColor(0, 0, 0));
    gradient.setColorAt(0.5, QColor(255, 255, 255));
    gradient.setColorAt(1.0, QColor(0, 0, 0));
    ringPainter.setBrush(Qt::NoBrush);
    ringPainter.setPen(QPen(QBrush(gradient), innerRingWidthHR));
    ringPainter.drawEllipse(center, innerRingRadiusHR, innerRingRadiusHR);

    // Riduci l'immagine a una risoluzione normale
    QImage finalImage = ringImage.scaled(m_gaugeCache.width(), m_gaugeCache.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter.drawImage(0, 0, finalImage);

    painter.restore();


}

Dynamometer::~Dynamometer() {

    // Dealloca le risorse qui, se necessario
    qDebug() << "Dynamometer distrutto";
}






/*************************/
// tacca. solo un esercizio
Tack::Tack(int length, int shadowTransparency, float shadowOffset, int width)
    : m_tackLength(length)
    , m_shadowTransparency(shadowTransparency)
    , m_shadowOffset(shadowOffset)
    , m_tackWidth(width) {}

void Tack::setLength(int length) {
    m_tackLength = length;
}

int Tack::getLength() const {
    return m_tackLength;
}

void Tack::setShadowTransparency(int transparency) {
    m_shadowTransparency = transparency;
}

void Tack::setShadowOffset(float offset) {
    m_shadowOffset = offset;
}

void Tack::setWidth(float width) {
    m_tackWidth = width;
}

int Tack::getWidth() const{
    return m_tackWidth;
}
void Tack::draw(QPainter &painter, float angle, float position) {
    float angleRad = qDegreesToRadians(angle);

    // Calcola le coordinate di partenza e di fine della tacca
    float x1 = (position - m_tackLength) * qCos(angleRad);
    float y1 = (position - m_tackLength) * qSin(angleRad);
    float x2 = (position) * qCos(angleRad);
    float y2 = (position) * qSin(angleRad);

    // Colore dell'ombra (più scuro)
    QColor shadowColor(0, 0, 0, m_shadowTransparency);

    // Disegna l'ombra della tacca
    painter.setPen(QPen(shadowColor, m_tackWidth));
    painter.drawLine(QPointF(x1 + m_shadowOffset, y1 + m_shadowOffset), QPointF(x2 + m_shadowOffset, y2 + m_shadowOffset));

    // Disegna la tacca
    QLinearGradient gradient(QPointF(x1, y1), QPointF(x2, y2));
    gradient.setColorAt(0.0, QColor(200, 200, 200));
    gradient.setColorAt(0.5, QColor(255, 255, 255));
    gradient.setColorAt(1.0, QColor(150, 150, 150));
    painter.setPen(QPen(gradient, m_tackWidth));
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
}

}
