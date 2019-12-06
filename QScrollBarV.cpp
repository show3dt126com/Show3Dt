#include "QScrollBarV.h"
#include <QPainter>

QScrollBarV::QScrollBarV(Qt::Orientation orientation, QWidget *parent)
    : QScrollBar(orientation, parent)
{
    fmt = "V=%d";
}

void QScrollBarV::paintEvent(QPaintEvent * e)
{
    QScrollBar::paintEvent(e);
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    //painter.translate(width() / 2, height() / 2);
    //painter.scale(side / 200.0, side / 200.0);
    QString vs;
    int v = value();
    vs.sprintf(fmt.toUtf8().data(), v);
    int x = 30;
    int mid = (maximum() + minimum()) / 2;
    QFontMetrics fm = painter.fontMetrics();
    int vsw = fm.width(vs);

    if (this->orientation() == Qt::Vertical)
    {
        if (v < mid)
            x = height() - vsw - 30;
        painter.rotate(90);
        painter.drawText(x, -3, vs);
    }
    else
    {
        if (v < mid)
            x = width() - vsw - 30;
        painter.drawText(x, 12, vs);
    }
}
