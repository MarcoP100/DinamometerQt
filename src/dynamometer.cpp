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

#include <QElapsedTimer>
#include <QDebug>

namespace MyProject {
Dynamometer::Dynamometer(QWidget *parent) :
    QOpenGLWidget(parent),
    m_maxValue(50),
    m_showNeedle(false),
    m_diameter(0),
    m_cacheGaugeDirty(true),
    m_cacheNeedleDirty(false),
    m_updateGauge(true),
    m_updateNeedle(false),
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

    setAttribute(Qt::WA_AlwaysStackOnTop);
    setAttribute(Qt::WA_OpaquePaintEvent,true);
    setAutoFillBackground(false);
    setStyleSheet("background:transparent;");

    //setFormat(QSurfaceFormat::defaultFormat());

    qDebug() << "Dynamometer creato";
    } // Initialize m_cacheDirty to true


void Dynamometer::setMaxValue(float maxValue) {
    m_maxValue = maxValue;
}

void Dynamometer::setShowNeedle(bool show) {
    m_showNeedle = show;
    m_updateNeedle = m_showNeedle;
    m_cacheNeedleDirty = m_showNeedle;

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

void Dynamometer::setAngleNeedle(float angle){
    if (m_needle.getAngle() != angle) {
        m_needle.setAngle(angle);
        m_updateNeedle = true;

       update();
    }
}

void Dynamometer::applyUpdates() {
    m_cacheGaugeDirty = true;
    m_updateGauge = true;
    update();

}



void Dynamometer::initializeGL() {
    qDebug() << "initializeGL called";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0); // Imposta il colore di sfondo trasparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
}



void Dynamometer::paintGL() {

    qDebug() << "paintGL called";
    // Inizia il timer all'inizio del metodo paintEvent
    QElapsedTimer timer;
    timer.start();

    
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
   

    if (m_cacheGaugeDirty) {
        generateGaugeCache();
        m_cacheGaugeDirty = false;
        qDebug() << "Dynamometer aggiornato";
    }

    //if (m_updateGauge) {
        // Disegna la cache della ghiera centrata nel widget
        m_updateGauge = false;
        painter.drawPixmap(0, 0, m_gaugeCache);
    //}



   // Disegna la lancetta se necessario
    if (m_cacheNeedleDirty) {
        generateNeedleCache();
        //m_needle.draw(painter, m_outerRingRadius, m_innerRingRadius, QPointF (m_gaugeCache.width()/2, m_gaugeCache.height()/2));
        m_cacheNeedleDirty = false;
    }

    if (m_updateNeedle) {
        painter.save();
        // Trasla al centro della ghiera
        painter.translate(m_gaugeCache.width() / 2, m_gaugeCache.height() / 2);
        // Applica la rotazione in base all'angolo corrente della lancetta
        painter.rotate(m_needle.getAngle());
        // Disegna la cache della lancetta
        painter.drawPixmap(-m_gaugeCache.width() / 2, -m_gaugeCache.height() / 2, m_needleCache);
        painter.restore();
    }



    // Log del tempo trascorso alla fine del metodo paintEvent
    qint64 elapsed = timer.nsecsElapsed() / 1000;
    qDebug() << "paintEvent duration:" << elapsed << "microseconds";
}


void Dynamometer::resizeGL(int w, int h) {
    qDebug() << "resizeGL called";
    //glViewport(0, 0, w, h);
   
}

void Dynamometer::generateGaugeCache() {
    m_gaugeCache = QPixmap(size());
    m_gaugeCache.fill(Qt::transparent); // Rende la cache trasparente

    QPainter painter(&m_gaugeCache);
    painter.setRenderHint(QPainter::Antialiasing, true);

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

void Dynamometer::generateNeedleCache() {
    m_needleCache = QPixmap(m_gaugeCache.size());
    m_needleCache.fill(Qt::transparent); // Rende la cache trasparente

    QPainter painter(&m_needleCache);
    painter.setRenderHint(QPainter::Antialiasing, true);
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
    const int sizeFont = 16;
    QFont font("Noto Sans", sizeFont);
    if (fontId == -1) {
        QMessageBox::warning(nullptr, "Warning", "Could not load font!");

    } else {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        font.setFamily(fontFamily); 
        
    }

    font.setWeight(QFont::Black);
    font.setPointSize(sizeFont);
    painter.setFont(font);
    
    painter.setPen(Qt::white);
    painter.translate(x, y);

    QFontMetrics metrics(font);
    int fontSize = metrics.height();
    //QMessageBox::information(nullptr, "Font Size", "Font height: " + QString::number(fontSize));
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
        QPointF textPosition(textRect.center().x() - (textWidth / 2.0), textRect.center().y() + (sizeFont/2));
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

    // anello esterno 
    m_outerRingRadius = (m_largeTackPosition - m_largeTack.getLength() - m_largeTack.getWidth());
    const float outterRingRadiusHR = m_outerRingRadius * highResFactor;
    ringPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    const float ringWidth1 = 1; // Spessore del primo anello
    ringPainter.setPen(QPen(Qt::black, ringWidth1  * highResFactor));
    ringPainter.setBrush(Qt::NoBrush);
    ringPainter.drawEllipse(center, outterRingRadiusHR, outterRingRadiusHR);


    // anello interno 
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






//*************************
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
    m_color(color),
    m_angle(0)
    {}

void Needle::setTipAngle(float angle) {
    m_tipAngle = angle;
}

void Needle::setColor(QColor color) {
    m_color = color;
}

void Needle::setAngle(float angle) {
    m_angle = angle;
}

int Needle::getAngle() const {
    return m_angle;
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
    /*QPainterPath path;
    path.moveTo(leftBase);
    path.lineTo(tip);
    path.lineTo(rightBase);
    path.arcTo(- innerRadius, - innerRadius, 2 * innerRadius, 2 * innerRadius, - C_angle_deg, C_angle_deg * 2);
    //path.closeSubpath();

    painter.setBrush(m_color);
    painter.setPen(Qt::NoPen);
    painter.drawPath(path);*/

    // Calcolare il punto centrale (baricentro) della lancetta
    QPointF centroid((leftBase.x() + tip.x() + rightBase.x()) / 3, 
                     (leftBase.y() + tip.y() + rightBase.y()) / 3);


    // Funzione per calcolare la proiezione di un punto P su una linea definita da A e B
    auto projectPointOnLine = [](QPointF P, QPointF A, QPointF B) -> QPointF {
        QPointF AP = P - A;
        QPointF AB = B - A;
        double ab2 = AB.x() * AB.x() + AB.y() * AB.y();
        double ap_ab = AP.x() * AB.x() + AP.y() * AB.y();
        double t = ap_ab / ab2;
        return A + t * AB;
    };

    // Disegnare i tre triangoli con gradienti distinti
    auto drawTriangleWithGradient = [&](QPointF p1, QPointF p2, QPointF p3, bool arc_p3_p1) {
        // Calcolare il punto medio tra leftBase e rightBase
        QPointF projection = projectPointOnLine(p2, p1, p3); // Proiezione del centroide sulla linea p1-p3

        QLinearGradient gradient(p2, projection);
        gradient.setColorAt(0.0, QColor(255, 255, 0, 240)); // Colore al p2, più luminoso e meno trasparente
        gradient.setColorAt(1.0, QColor(255, 255, 0, 100)); // Colore al midpoint, più scuro e più trasparente

        
        QPainterPath path;
        path.moveTo(p1);
        path.lineTo(p2);
        path.lineTo(p3);
        if (!arc_p3_p1) {
            path.closeSubpath();
        } else {
            path.arcTo(- innerRadius, - innerRadius, 2 * innerRadius, 2 * innerRadius, - C_angle_deg, C_angle_deg * 2);
        }

        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawPath(path);

    };

    drawTriangleWithGradient(leftBase, centroid, tip, false);
    drawTriangleWithGradient(tip, centroid,rightBase, false);
    drawTriangleWithGradient(leftBase, centroid, rightBase, true);

    // Disegna linee dagli angoli della lancetta al punto centrale
    painter.setPen(QPen(Qt::black, 0.3));
    painter.drawLine(leftBase, centroid);
    painter.drawLine(tip, centroid);
    painter.drawLine(rightBase, centroid);
    painter.drawLine(leftBase, tip);

    painter.restore();

}

}
