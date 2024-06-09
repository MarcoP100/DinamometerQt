#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>

class BackgroundWidget : public QWidget {
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_background;
};

#endif // BACKGROUNDWIDGET_H

