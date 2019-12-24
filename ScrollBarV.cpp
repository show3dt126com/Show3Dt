#include "ScrollBarV.h"
#include <QPainter>
#include <QDebug>
#include "Global.h"


const QString ActiveScrollBarStyleV =
    "QScrollBar::vertical{background:rgb(153,70,87);"
        "border:1px solid grey;margin:24px 0px 24px 0px;}"
    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{"
        "background:rgb(192,232,182);}"
    "QScrollBar::handle:vertical{background:rgb(73,70,197);"
        "border:1px solid grey;border-radius:2px;min-height:30px;}"
    "QScrollBar::add-line:vertical{height:22px;subcontrol-origin:margin;}"
    "QScrollBar::sub-line:vertical{height:22px;subcontrol-origin:margin;}";

const QString ActiveScrollBarStyleH =
    "QScrollBar::horizontal{background:rgb(153,70,87);"
        "border:1px solid grey;margin:0px 24px 0px 24px;}"
    "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{"
        "background:rgb(192,232,182);}"
    "QScrollBar::handle:horizontal{background:rgb(73,70,197);"
        "border:1px solid grey;border-radius:2px;min-width:30px;}"
    "QScrollBar::add-line:horizontal{width:22px;subcontrol-origin:margin;"
        "subcontrol-position:right;}"
    "QScrollBar::sub-line:horizontal{width:22px;subcontrol-origin:margin;"
        "subcontrol-position:left;}";
    //"QScrollBar:left-arrow:horizontal,QScrollBar::right-arrow:horizontal{"
    //    "width:6px;height:6px;background:rgb(173,230,37);}";


const QString InactiveScrollBarStyleV =
    "QScrollBar::vertical{background:rgb(213,210,207);"
        "border:1px solid grey;margin:24px 0px 24px 0px;}"
    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{"
        "background:rgb(229,229,209);}"
    "QScrollBar::handle:vertical{background:rgb(223,220,217);"
        "border:1px solid grey;border-radius:2px;min-height:30px;}"
    "QScrollBar::add-line:vertical{height:22px;subcontrol-origin:margin;}"
    "QScrollBar::sub-line:vertical{height:22px;subcontrol-origin:margin;}";

const QString InactiveScrollBarStyleH =
    "QScrollBar::horizontal{background:rgb(213,210,207);"
        "border:1px solid grey;margin:0px 24px 0px 24px;}"
    "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{"
        "background:rgb(229,229,209);}"
    "QScrollBar::handle:horizontal{background:rgb(223,220,217);"
        "border:1px solid grey;border-radius:2px;min-width:30px;}"
    "QScrollBar::add-line:horizontal{width:22px;subcontrol-origin:margin;"
        "subcontrol-position:right;}"
    "QScrollBar::sub-line:horizontal{width:22px;subcontrol-origin:margin;"
        "subcontrol-position:left;}";


ScrollBarV::ScrollBarV(Qt::Orientation o, QString tag, QWidget *parent)
    : QScrollBar (o, parent)
{
    fmt = "";
    this->tag = tag;
    if (o == Qt::Horizontal)
        setStyleSheet(InactiveScrollBarStyleH);
    else
        setStyleSheet(InactiveScrollBarStyleV);
    setTracking(false);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void ScrollBarV::paintEvent(QPaintEvent * pe)
{
    QScrollBar::paintEvent(pe);
    if (fmt == "")
        return;
    qDebug() << "ScrollBarV::paintEvent" << tag;

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
        if (((v < mid) && (!invertedAppearance())) ||
                ((v > mid) && invertedAppearance()))
            x = height() - 25 - vsw;
        y = -width()+vsh;
    }
    else
    {
        if (((v < mid) && (!invertedAppearance())) ||
                ((v > mid) && invertedAppearance()))
            x = width() - 25 - vsw;
    }
    // y = baseline
    qDebug() << "pntr.drawText" << tag << fmt << v << vs;
    pntr.drawText(x, y, vs);
}

void ScrollBarV::focusInEvent(QFocusEvent *event)
{
    if (this->orientation() == Qt::Horizontal)
        setStyleSheet(ActiveScrollBarStyleH);
    else
        setStyleSheet(ActiveScrollBarStyleV);
}

void ScrollBarV::focusOutEvent(QFocusEvent *event)
{
    if (this->orientation() == Qt::Horizontal)
        setStyleSheet(InactiveScrollBarStyleH);
    else
        setStyleSheet(InactiveScrollBarStyleV);
}

void ScrollBarV::setValue(int value)
{
    qDebug() << "setValue(int)" << tag << value;
    QScrollBar::setValue(value);
    repaint();
}

void ScrollBarV::setValue(double pos)
{
    qDebug() << "setValue(double)" << tag << pos;
    QScrollBar::setValue(int(pos));
    repaint();
}
