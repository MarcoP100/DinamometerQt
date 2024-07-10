#include "dynamometer.h"
#include <QPainter>
#include <QPaintEvent>
#include <QRadialGradient>
#include <QColor>
#include <QDebug>
#include <QtMath>
#include <QPainterPath>
#include <QFontDatabase>
#include <QFont>
#include <QMessageBox>

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
    m_outerRingRadius(0),
    m_needle(0.0,Qt::white)   {

    qDebug() << "Dynamometer creato";
    } // Initialize m_cacheDirty to true


void Dynamometer::setMaxValue(float maxValue) {
    m_maxValue = maxValue;
}

void Dynamometer::setShowNeedle(bool show) {
    m_showNeedle = show;
    update();
}

void Dynamometer::setDiameter(float diameter) {
    m_diameter = diameter;
}
void Dynamometer::setPositionCenter(float x, float y) {
    m_center = QPoint(x, y);

}

void Dynamometer::setShowChromeRing(bool show) {
    m_showChromeRing = show;

}

void Dynamometer::setChromeRingWidth(float width) {
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

void Dynamometer::setlargeTack(float length, float shadowTransparency, float shadowOffset, float width){
    m_largeTack = Tack(length,shadowTransparency,shadowOffset,width);

}

void Dynamometer::setsmallTack(float length, float shadowTransparency, float shadowOffset, float width){
    m_smallTack = Tack(length,shadowTransparency,shadowOffset,width);

}

void Dynamometer::setNumberRadius(float position) {
    m_numberRadius = position;
}

void Dynamometer::setInnerRing(float radius, float width){
    m_innerRingRadius = radius;
    m_innerRingWidth = width;
}

void Dynamometer::setNeedle(float angle, QColor color){
    m_needle.setTipAngle(angle);
    m_needle.setColor(color);
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
    m_needle.draw(painter, m_outerRingRadius, m_innerRingRadius, QPointF (m_gaugeCache.width()/2, m_gaugeCache.height()/2));
}

void Dynamometer::drawGradientBackground(QPainter &painter) {

    // Abilitare l'anti-aliasing
    painter.setRenderHint(QPainter::Antialiasing);

    // Centro del gradiente
    const QPointF center(m_gaugeCache.width()/2, m_gaugeCache.height()/2);


    // Raggio del gradiente
    const qreal radius = m_diameter / 2;

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

/*void Dynamometer::drawNeedle(QPainter &painter) {
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
}*/

void Dynamometer::drawTacks(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    const float x = m_gaugeCache.width() / 2;
    const float y = m_gaugeCache.height() / 2;
    m_largeTackPosition = m_diameter / 2 - 5;

    // Calcola l'angolo incrementale tra le tacche
    const float largeTackIncrement  = (m_endAngle - m_startAngle) / (m_largeTacksCount - 1);

    // Calcola l'angolo incrementale tra le tacche piccole
    const float smallTackIncrement = largeTackIncrement / (m_smallTacksBetween + 1);

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
    const float highResFactor = 2;  // Fattore di risoluzione più alta
    const float highResWidth = m_gaugeCache.width() * highResFactor;
    const float highResHeight = m_gaugeCache.height() * highResFactor;

    const QPointF center(highResWidth / 2, highResHeight / 2);
    const float innerRadius = (m_diameter / 2) * highResFactor;
    const float outerRadius = innerRadius + (m_chromeRingWidth * highResFactor);

    QImage ringImage(highResWidth, highResHeight, QImage::Format_ARGB32);
    ringImage.fill(Qt::transparent);
    QPainter ringPainter(&ringImage);

    // Abilitare l'anti-aliasing
    ringPainter.setRenderHint(QPainter::Antialiasing, true);

    drawRadialGradient(ringPainter, center, innerRadius, outerRadius);
    drawInnerRing(ringPainter, center, innerRadius, highResFactor);

    // Riduci l'immagine a una risoluzione normale
    QImage finalImage = ringImage.scaled(m_gaugeCache.width(), m_gaugeCache.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter.drawImage(0, 0, finalImage);

    painter.restore();
}

void Dynamometer::drawRadialGradient(QPainter &painter, const QPointF &center, float innerRadius, float outerRadius) {
    QRadialGradient gradient(center, outerRadius);
    gradient.setColorAt(innerRadius / outerRadius, QColor(125, 125, 125));
    gradient.setColorAt((((outerRadius - innerRadius) / 2) + innerRadius) / outerRadius, QColor(190, 190, 190));
    gradient.setColorAt(1.0, QColor(64, 64, 64));
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, outerRadius, outerRadius);
}


void Dynamometer::drawInnerRing(QPainter &painter, const QPointF &center, float innerRadius, int highResFactor) {
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    const float ringWidth1 = 5 * highResFactor;
    painter.setPen(QPen(Qt::black, ringWidth1));
    painter.drawEllipse(center, innerRadius, innerRadius);
}

void Dynamometer::drawNumbers(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    const float x = m_gaugeCache.width() / 2;
    const float y = m_gaugeCache.height() / 2;
    const float numberRadius = m_numberRadius; // Posizione dei numeri

    // Calcola l'angolo incrementale tra le tacche
    const float numbersIncrement  = (m_endAngle - m_startAngle) / (m_largeTacksCount - 1);


    // Carica il font dall'archivio delle risorse
    int fontId = QFontDatabase::addApplicationFont(":/fonts/NotoSans-Bold.ttf");
    const int sizeFont = 22;
    QFont font("Noto Sans", sizeFont);
    if (fontId == -1) {
        QMessageBox::warning(nullptr, "Warning", "Could not load font!");

    } else {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        font.setFamily(fontFamily); 
        
    }

    font.setWeight(QFont::Black);
    font.setPointSize(22);
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
        float textWidth = metrics.horizontalAdvance(text);
        float textHeight = metrics.height();
        QRect textRect(textX - textWidth / 2, textY - textHeight / 2, textWidth, textHeight); // Rettangolo per il testo
        textRect.moveCenter(QPoint(textX, textY));

        // Disegna il contorno rosso
        QPainterPath path;
        QPointF textPosition(textRect.center().x() - (textWidth / 2.0), textRect.center().y() + (sizeFont/2.0));
        path.addText(textPosition, font, text);

        QPen pen(Qt::black);
        pen.setWidth(2); // Spessore del contorno
            painter.setPen(pen);
            painter.drawPath(path);

        // Crea il gradiente per il testo
        QLinearGradient gradient(textRect.topLeft(), textRect.bottomRight());
        gradient.setColorAt(0.0, QColor(50, 50, 50));
        gradient.setColorAt(0.5, QColor(255, 255, 255));
        gradient.setColorAt(1.0, QColor(50, 50, 50));
        painter.fillPath(path, gradient);

    }
    painter.restore();
}


void Dynamometer::drawInternalRings(QPainter &painter){
    painter.save();

    const float highResFactor = 5;  // Fattore di risoluzione più alta
    const float highResWidth = m_gaugeCache.width() * highResFactor;
    const float highResHeight = m_gaugeCache.height() * highResFactor;

    const QPointF center(highResWidth / 2, highResHeight / 2);
    QImage ringImage(highResWidth, highResHeight, QImage::Format_ARGB32);
    ringImage.fill(Qt::transparent);
    QPainter ringPainter(&ringImage);

    // Abilitare l'anti-aliasing
    ringPainter.setRenderHint(QPainter::Antialiasing, true);

    /* anello esterno */
    m_outerRingRadius = (m_largeTackPosition - m_largeTack.getLength() - m_largeTack.getWidth());
    const float outterRingRadiusHR = m_outerRingRadius * highResFactor;
    ringPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    const float ringWidth1 = 1; // Spessore del primo anello
    ringPainter.setPen(QPen(Qt::black, ringWidth1  * highResFactor));
    ringPainter.setBrush(Qt::NoBrush);
    ringPainter.drawEllipse(center, outterRingRadiusHR, outterRingRadiusHR);


    /* anello interno */
    //ringPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    const float innerRingRadiusHR = m_innerRingRadius * highResFactor;
    const float innerRingWidthHR = m_innerRingWidth * highResFactor;
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

//lancetta
Needle::Needle(float angle, QColor color):
    m_tipAngle(angle),
    m_color(color)
    {}

void Needle::setTipAngle(float angle) {
    m_tipAngle = angle;
}

void Needle::setColor(QColor color) {
    m_color = color;
}


void Needle::draw(QPainter &painter, float outerRadius, float innerRadius, QPointF center){
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.translate(center);
    painter.rotate(0);

     QPolygonF needlePolygon;
    float B_angle = qDegreesToRadians(m_tipAngle / 2.0);
    float temp = std::sin(B_angle);
    float temp2 = temp  * outerRadius / innerRadius;
    float A_angle = M_PI - std::asin(temp2);
    float C_angle = M_PI - B_angle - A_angle;

    float C_angle_deg = qRadiansToDegrees(C_angle);

    QPointF leftBase(innerRadius * std::cos(C_angle), - innerRadius * std::sin(C_angle));
    QPointF tip(outerRadius, 0);
    QPointF rightBase(innerRadius * std::cos(C_angle), innerRadius * std::sin(C_angle));

    needlePolygon << leftBase << tip << rightBase;

    // Aggiungi un arco alla base del poligono per fare la base curva
    QPainterPath path;
    path.moveTo(leftBase);
    path.lineTo(tip);
    path.lineTo(rightBase);
    path.arcTo(- innerRadius, - innerRadius, 2 * innerRadius, 2 * innerRadius, - C_angle_deg, C_angle_deg * 2);
    //path.closeSubpath();

    painter.setBrush(m_color);
    painter.setPen(Qt::NoPen);
    painter.drawPath(path);




    painter.restore();

}

}
