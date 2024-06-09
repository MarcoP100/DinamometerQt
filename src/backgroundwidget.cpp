#include "backgroundwidget.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>

BackgroundWidget::BackgroundWidget(QWidget *parent) :
    QWidget(parent),
    m_background(":/images/gradient_background.png") {}

void BackgroundWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap scaledBackground = m_background.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(rect(), scaledBackground);
}
