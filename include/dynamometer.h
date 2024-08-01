#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>
#include <QPixmap>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>

namespace MyProject {

class Dynamometer;
class Tack;
class Needle;
class Notch;

// classe tacca rossa
class Notch {

private:
    float m_notchLength;
    int m_notchTransparency;
    float m_angle;

public:
    Notch(float length, int transparency, float angle);
    void draw(QPainter &painter, float outerRadius, float innerRadius, QPointF center);

    // Metodi di impostazione
    void setLength_deg(float length);
    void setTransparency(int transparency);
    void setAngle(float angle);

    int getLength_deg() const;
    int getAngle() const;
};


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
    void setAngle(float angle); // Metodo per impostare l'angolo
    void draw(QPainter &painter, float outerRadius, float innerRadius, QPointF center);

    int getAngle() const;

protected:
   float m_angle; // Angolo della lancetta
};


// dinamometro
class Dynamometer : public QOpenGLWidget, protected QOpenGLFunctions {
    //Q_OBJECT

public:
    Dynamometer(QWidget *parent = nullptr);
    void setMaxValue(float maxValue);
    void setShowNeedle(bool show);
    void setShowNotch(bool show);
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
    void setAngleNeedle(float angle);
    void setAngleNotch(float angle);
    void setNotchLenght(float lenght_kn);
    void setExtraPullZone(bool enable, float lenght_kN);
    void setHighPullZone(bool enable, float lenght_kN);
    void applyUpdates();
    ~Dynamometer();

protected:
   // void paintEvent(QPaintEvent *event) override;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void drawTacks(QPainter &painter);
    void generateGaugeCache();
    void generateNeedleCache();
    void generateNotchCache();
    void drawGradientBackground(QPainter &painter);
    void drawChromeRing(QPainter &painter);
    void drawNumbers(QPainter &painter);
    void drawInternalRings(QPainter &painter);
    void drawRadialGradient(QPainter &painter, const QPointF &center, float innerRadius, float outerRadius);
    void drawInnerRing(QPainter &painter, const QPointF &center, float innerRadius, int highResFactor);
    void drawExtraPullZone(QPainter &painter, float lenght_deg, float endAngle_deg, float outerRadius, float innerRadius, QPointF center);
    void drawHighPullZone(QPainter &painter, float startAngle_deg, float stopAngle_deg, float outerRadius, float innerRadius, QPointF center);


    float m_maxValue;
    
    float m_diameter;         // diametro della ghiera. E' escluso l'eventuale anello esterno
    QPoint m_center;        // Centro della ghiera
    
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
    Notch m_notch;
    bool m_showNeedle;
    bool m_showNotch;

    QPixmap m_gaugeCache;
    QPixmap m_needleCache;
    QPixmap m_notchCache;
    bool m_cacheGaugeDirty;
    bool m_cacheNeedleDirty;
    bool m_cacheNotchDirty;

    bool m_enableExtraPullZone;
    float m_ExtraPullLenght_deg;

    bool m_enableHighPullZone;
    float m_highPullLenght_deg;

};


} // namespace MyProject

#endif // DYNAMOMETER_H
