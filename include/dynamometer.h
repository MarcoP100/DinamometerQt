#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>
#include <QPixmap>

namespace MyProject {
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
    void applyUpdates();
    ~Dynamometer();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawGauge(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawTacksAndNumbers(QPainter &painter);
    void generateGaugeCache();
    void drawGradientBackground(QPainter &painter);
    void drawChromeRing(QPainter &painter);

    int m_maxValue;
    bool m_showNeedle;
    int m_diameter;         // diametro della ghiera. E' escluso l'eventuale anello esterno
    QPoint m_center;        // Centro della ghiera
    bool m_cacheDirty;
    QPixmap m_gaugeCache;
    bool m_showChromeRing;  // abilitazione anello
    int m_chromeRingWidth;  // larghezza anello
    float m_startAngle;     // angolo inizio tacche
    float m_endAngle;       // anggggolo fine tacche
    int m_largeTacksCount;   // numero tacche grandi
    int m_smallTacksBetween; // Numero di tacche piccole tra due tacche grandi

};
} // namespace MyProject

#endif // DYNAMOMETER_H
