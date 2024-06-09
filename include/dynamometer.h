// File: include/dynamometer.h

#ifndef DYNAMOMETER_H
#define DYNAMOMETER_H

#include <QWidget>

class Dynamometer : public QWidget {
    Q_OBJECT

public:
    explicit Dynamometer(QWidget *parent = nullptr);
    void setValue(int value);
    void setPosition(int x, int y);
    QPoint getPosition() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_value;
    int m_x;
    int m_y;
    QPixmap m_gauge;

    void drawGauge(QPainter &painter);
};

#endif // DYNAMOMETER_H
