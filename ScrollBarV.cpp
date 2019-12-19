#include "ScrollBarV.h"
#include <QPainter>
#include <QDebug>
#include "Global.h"

ScrollBarV::ScrollBarV(Qt::Orientation o, QString tag, QWidget *parent)
    : QScrollBar (o, parent)
{
    fmt = "";
    this->tag = tag;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void ScrollBarV::paintEvent(QPaintEvent * pe)
{
    QScrollBar::paintEvent(pe);
    if (fmt == "")
        return;

    int v = value();
    int mid = minimum()/2 + maximum()/2;
    QString vs;
    vs.sprintf(fmt.toUtf8().data(), v);
    QPainter pntr(this);
    QFontMetrics fm = pntr.fontMetrics();
    int vsw = fm.width(vs);
    // vsh == 12  width()==17
    int vsh = fm.height();
    int x = 25;
    int y = vsh;
    // 原点：左上角 X:往下， Y:往左
    if (orientation() == Qt::Vertical)
    {
        pntr.rotate(90);
        if (v < mid)
            x = height() - 25 - vsw;
        y = -width()+vsh;
        //pntr.drawLine(0,0, height()/2, -width());
    }
    else
    {
        if (v < mid)
            x = width() - 25 - vsw;
        //pntr.drawLine(0,0, width(), height()/2);
    }
    // y = baseline
    pntr.drawText(x, y, vs);
}

void ScrollBarV::focusInEvent(QFocusEvent *event)
{
    if (this->orientation() == Qt::Horizontal)
        setStyleSheet(G.activeScrollBarStyleH);
    else
        setStyleSheet(G.activeScrollBarStyleV);
}

void ScrollBarV::focusOutEvent(QFocusEvent *event)
{
    setStyleSheet("");
}

void ScrollBarV::setValue(int value)
{
    //setTracking(false);
    QScrollBar::setValue(value);
    //setSliderPosition(value);
    //qDebug() << "setValue()" << tag << value;
    //setTracking(true);
}

void ScrollBarV::setSliderPos(int pos)
{
    setSliderPosition(pos);
}
