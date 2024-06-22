#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>
#include <QPixmap>

namespace MyProject {

class Dynamometer;
class Tack;

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


// dinamometro
class Dynamometer : public QWidget {
    Q_OBJECT

public:
    explicit Dynamometer(QWidget *parent = nullptr);
    void setMaxValue(int maxValue);
    void setShowNeedle(bool show);
    void setDiameter(int diameter);
    void setPositionCenter(int x, int y);
    void setShowChromeRing(bool show);
    void setChromeRingWidth(int width);
    void setLargeTacksCount(int count);
    void setSmallTacksBetweenCount(int count);
    void setStartAngle(float angle);
    void setEndAngle(float angle);
    void setlargeTack(int length, int shadowTransparency, float shadowOffset, int width);
    void setsmallTack(int length, int shadowTransparency, float shadowOffset, int width);
    void setNumberRadius(int position);
    void setInnerRing(int radius, int width);
    void applyUpdates();
    ~Dynamometer();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawGauge(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawTacks(QPainter &painter);
    void generateGaugeCache();
    void drawGradientBackground(QPainter &painter);
    void drawChromeRing(QPainter &painter);
    void drawNumbers(QPainter &painter);
    void drawInternalRings(QPainter &painter);


    int m_maxValue;
    bool m_showNeedle;
    int m_diameter;         // diametro della ghiera. E' escluso l'eventuale anello esterno
    QPoint m_center;        // Centro della ghiera
    bool m_cacheDirty;
    QPixmap m_gaugeCache;
    bool m_showChromeRing;  // abilitazione anello
    int m_chromeRingWidth;  // larghezza anello
    float m_startAngle;     // angolo inizio tacche
    float m_endAngle;       // angolo fine tacche
    int m_largeTacksCount;   // numero tacche grandi
    int m_smallTacksBetween; // Numero di tacche piccole tra due tacche grandi
    Tack m_largeTack;
    Tack m_smallTack;
    int m_numberRadius;       // raggio a cui posizionare i numeri
    int m_largeTackPosition;
    int m_innerRingRadius;
    int m_innerRingWidth;
    int m_outerRingRadius;
};


} // namespace MyProject

#endif // DYNAMOMETER_H
