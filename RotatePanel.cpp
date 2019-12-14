#include "RotatePanel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
#include <QPixmap>

RotatePanel::RotatePanel(int x, int y, QWidget *parent)
    : QWidget(parent)
{
    w = 120;
    h = 120;
    setGeometry(x, y, w, h);

    QPixmap pix(w, h);
    QPainter painter(&pix);
    painter.fillRect(0,0,w,h, QBrush(QColor(255, 0,0)));
    painter.setBrush(QBrush(QColor(0,255,0)));
    painter.drawEllipse(0, 0, w, h);
    QBitmap shapeB = pix.createHeuristicMask();
    setMask(shapeB);

    camA = 45;
}

void RotatePanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBrush(QBrush(QColor(240,240,240)));
    p.drawEllipse(0,0, w-1, h-1);

    p.translate(w/2, h/2);
    p.rotate(90 + camA);
    p.setBrush(QBrush(QColor(255,255,0)));
    p.drawRect(-w/2+5, -15, 20, 30);

    p.rotate(-45);
    p.setPen(QPen(QBrush(QColor(255,0,0)), 4));
    p.drawLine(-(w/2-5), 0, w/2-5, 0);
}

void RotatePanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int xpos = event->pos().x();
        int ypos = event->pos().y();
        camA += 10;
        repaint();
    }
    else if (event->button() == Qt::RightButton)
    {
        int xpos = event->pos().x();
        int ypos = event->pos().y();
        camA -= 10;
        repaint();
    }
}
