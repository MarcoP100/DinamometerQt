#include "backgroundwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>
#include <QSvgRenderer>
#include <QPixmap>

namespace MyProject {

BackgroundWidget::BackgroundWidget(QWidget *parent)
    : QWidget(parent),
    m_svgRenderer(new QSvgRenderer(QStringLiteral(":/images/logo.svg"), this)),
    m_logoCache(30,30),
    m_cacheDirty(true),
    m_useSvg(false) // Di default usa PNG
{}

BackgroundWidget::~BackgroundWidget() {
    // Pulizia
}

void BackgroundWidget::setUseSvg(bool useSvg) {
    m_useSvg = useSvg;
    m_cacheDirty = true;
    update(); // Forza il ridisegno
}

void BackgroundWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    drawBackground(painter);
    drawLogo(painter); // Disegna il logo
}

void BackgroundWidget::drawBackground(QPainter &painter) {
    QRect rect = this->rect();  // Ottiene i limiti del widget corrente
    // Esempio di gradiente lineare
    QLinearGradient linearGradient(0, 0, 0, rect.height());
    linearGradient.setColorAt(0.0, QColor(0, 0, 0));
    linearGradient.setColorAt(1.0, QColor(0, 0, 100));
    painter.fillRect(rect, linearGradient);

}

void BackgroundWidget::drawLogo(QPainter &painter) {

    //svg
    if (m_cacheDirty) {
        cacheLogo();
    }
    int x = width() - m_logoCache.width() - 10; // Posizione X (10 pixel dal bordo destro)
    int y = height() - m_logoCache.height() - 10; // Posizione Y (10 pixel dal bordo inferiore)
    painter.drawPixmap(x, y, m_logoCache);

 }

void BackgroundWidget::cacheLogo() {
    // disegno svg
    int logoWidth = 30; // Larghezza del logo
    int logoHeight = 30; // Altezza del logo
    m_logoCache = QPixmap(logoWidth, logoHeight);
    m_logoCache.fill(Qt::transparent);

    QPainter cachePainter(&m_logoCache);

    if (m_useSvg){
        // Render del SVG
        m_svgRenderer->render(&cachePainter, QRect(0, 0, logoWidth, logoHeight));

    }else {
        // Carica il PNG e disegna nel QPixmap
                QPixmap logoPng(":/images/logo.png");
                cachePainter.drawPixmap(0, 0, logoWidth, logoHeight, logoPng);
           }

    m_cacheDirty = false;
}
} // namespace MyProject
