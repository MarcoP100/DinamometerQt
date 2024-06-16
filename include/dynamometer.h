#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>
#include <QPixmap>

namespace MyProject {
class Dynamometer : public QWidget {
    Q_OBJECT

public:
    explicit Dynamometer(QWidget *parent = nullptr);
    void setValue(int value);
    void setMaxValue(int maxValue);
    void setTackCount(int count);
    void setShowNeedle(bool show); 
    void setDiameter(int diameter);
    void setPositionCenter(int x, int y);
    void setShowChromeRing(bool show);
    void setChromeRingWidth(int width);
    void setChromeRingColor(const QColor &color);
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

    int m_value;
    int m_maxValue;
    int m_tackCount;
    bool m_showNeedle;
    int m_diameter; // diametro della ghiera. E' escluso l'eventuale anello esterno
    QPoint m_center; // Centro della ghiera
    bool m_cacheDirty;
    QPixmap m_gaugeCache;

    bool m_showChromeRing;
    int m_chromeRingWidth;
    QColor m_chromeRingColor;

};
} // namespace MyProject

#endif // DYNAMOMETER_H
