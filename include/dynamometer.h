#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>
#include <QPixmap>

class Dynamometer : public QWidget {
    Q_OBJECT

public:
    explicit Dynamometer(QWidget *parent = nullptr);
    void setValue(int value);
    void setMaxValue(int maxValue);
    void setTackCount(int count);
    void setShowNeedle(bool show);
    void setPosition(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_value;
    int m_maxValue;
    int m_tackCount;
    bool m_showNeedle;
    int m_x;
    int m_y;

    QPixmap m_gaugeCache;
    bool m_cacheDirty;

    void drawGauge(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawTacks(QPainter &painter);
    void generateGaugeCache();
    void drawGradientBackground(QPainter &painter);
};

#endif // DYNAMOMETER_H
