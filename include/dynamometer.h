#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>
#include <QPixmap>

namespace MyProject {

class Dynamometer;
class Tack;
class Needle;

// classe tacca
class Tack {

private:
    int m_tackLength;
    int m_shadowTransparency;
    float m_shadowOffset;
    int m_tackWidth;        // larghezza tacca

public:
    Tack(int length, int shadowTransparency, float shadowOffset, int width);
    void draw(QPainter &painter, float angle, float position);

    // Metodi di impostazione
    void setLength(int length);
    void setShadowTransparency(int transparency);
    void setShadowOffset(float offset);
    void setWidth(float width);
    int getLength() const;
    int getWidth() const;

};

// classe lancetta
class Needle {

private:
    float m_tipAngle;
    QColor m_color;

public:
    Needle(float angle, QColor color);

    void setTipAngle(float angle);
    void setColor(QColor color);

    void draw(QPainter &painter, float outerRadius, float innerRadius, QPointF center);
};


// dinamometro
class Dynamometer : public QWidget {
    Q_OBJECT

public:
    explicit Dynamometer(QWidget *parent = nullptr);
    void setMaxValue(float maxValue);
    void setShowNeedle(bool show);
    void setDiameter(float diameter);
    void setPositionCenter(float x, float y);
    void setShowChromeRing(bool show);
    void setChromeRingWidth(float width);
    void setLargeTacksCount(int count);
    void setSmallTacksBetweenCount(int count);
    void setStartAngle(float angle);
    void setEndAngle(float angle);
    void setlargeTack(float length, float shadowTransparency, float shadowOffset, float width);
    void setsmallTack(float length, float shadowTransparency, float shadowOffset, float width);
    void setNumberRadius(float position);
    void setInnerRing(float radius, float width);
    void setNeedle(float angle, QColor color);
    void applyUpdates();
    ~Dynamometer();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawTacks(QPainter &painter);
    void generateGaugeCache();
    void drawGradientBackground(QPainter &painter);
    void drawChromeRing(QPainter &painter);
    void drawNumbers(QPainter &painter);
    void drawInternalRings(QPainter &painter);
    void drawRadialGradient(QPainter &painter, const QPointF &center, float innerRadius, float outerRadius);
    void drawInnerRing(QPainter &painter, const QPointF &center, float innerRadius, int highResFactor);



    float m_maxValue;
    bool m_showNeedle;
    float m_diameter;         // diametro della ghiera. E' escluso l'eventuale anello esterno
    QPoint m_center;        // Centro della ghiera
    bool m_cacheDirty;
    QPixmap m_gaugeCache;
    bool m_showChromeRing;  // abilitazione anello
    float m_chromeRingWidth;  // larghezza anello
    float m_startAngle;     // angolo inizio tacche
    float m_endAngle;       // angolo fine tacche
    int m_largeTacksCount;   // numero tacche grandi
    int m_smallTacksBetween; // Numero di tacche piccole tra due tacche grandi
    Tack m_largeTack;
    Tack m_smallTack;
    float m_numberRadius;       // raggio a cui posizionare i numeri
    float m_largeTackPosition;
    float m_innerRingRadius;
    float m_innerRingWidth;
    float m_outerRingRadius;

    Needle m_needle;

};


} // namespace MyProject

#endif // DYNAMOMETER_H
