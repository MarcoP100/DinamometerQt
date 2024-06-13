#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>
#include <QSvgRenderer>
#include <QPixmap>

namespace MyProject {

class BackgroundWidget : public QWidget {
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget *parent = nullptr);
    void setUseSvg(bool useSvg); // Permette di scegliere tra SVG e PNG

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawLogo(QPainter &painter);
    void cacheLogo(); // Metodo per cache del logo

    QSvgRenderer *m_svgRenderer;
    QPixmap m_logoCache;
    bool m_cacheDirty;
    bool m_useSvg; // Flag per usare SVG o PNG

};

} // namespace MyProject

#endif // BACKGROUNDWIDGET_H

