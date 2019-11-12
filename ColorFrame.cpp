#include "ColorFrame.h"
#include <QPainter>
#include <QPen>
#include <QColor>

ColorFrame::ColorFrame(QWidget* parent)
{
    showAs = CFA_SPACE;
    pColorMap = nullptr;
}

void ColorFrame::paintEvent(QPaintEvent *event)
{
    if (pColorMap == nullptr)
        return;
    if (showAs == CFA_SPACE)
        drawAsColorFrame();
    else if (showAs == CFA_ZEBRA)
        drawAsColorZebra();
    else if (showAs == CFA_SPLIT)
        drawAsColorSplit();
}

void ColorFrame::drawAsColorFrame()
{
    QPainter painter(this);
    ColorMap & colorMap = * pColorMap;
    if (colorMap.keyNum < 2)
        return;

    float w = this->width();
    float h = this->height();
    double dv = colorMap.vMax - colorMap.vMin;
    if (dv < 0.001)
        dv = 0.001;
    float vCo = w / dv;
    ColorKey & key0 = colorMap.colorKeys[0];
    for (int i=1; i<colorMap.keyNum; i++)
    {
        ColorKey & key1 = colorMap.colorKeys[i-1];
        ColorKey & key2 = colorMap.colorKeys[i];
        float x1 = int(vCo * (key1.v - key0.v));
        float x2 = int(vCo * (key2.v - key0.v));
        float len = x2 - x1;
        QColor c1 = key1.color;
        QColor c2 = key2.color;
        for (int x=x1; x<x2; x++)
        {
            float r = c1.red() + (x-x1)/len*(c2.red()-c1.red());
            float g = c1.green() + (x-x1)/len*(c2.green()-c1.green());
            float b = c1.blue() + (x-x1)/len*(c2.blue()-c1.blue());
            painter.setPen(QColor(r, g, b));
            painter.drawLine(x,0,x,h);
        }
    }
}

void ColorFrame::drawAsColorZebra()
{
    ColorMap & colorMap = * pColorMap;
    int colorZebraNum = colorMap.colorZebraNum;
    if (colorZebraNum == 0)
        return;

    QPainter painter(this);
    double w = this->width();
    double h = this->height();
    painter.fillRect(0, 0, w, h, QColor(255,255,255));
    for (int z=0; z<colorZebraNum; z++)
    {
        QVector3D & v = colorMap.colorZebra[z];
        int r = v.x()*255;
        int g = v.y()*255;
        int b = v.z()*255;
        QColor c(r, g, b);
        double x0 = z*w/colorZebraNum;
        double x1 = (z+1)*w/colorZebraNum;
        painter.fillRect(x0+1, 0, x1-x0-2, h, c);
    }
}

void ColorFrame::drawAsColorSplit()
{
    ColorMap & colorMap = * pColorMap;
    int colorZebraNum = colorMap.colorZebraNum;
    if (colorZebraNum == 0)
        return;

    QPainter painter(this);
    double w = this->width();
    double h = this->height();
    painter.fillRect(0, 0, w, h, QColor(255,255,255));
    for (int z=0; z<colorZebraNum; z++)
    {
        QVector3D & v = colorMap.colorZebra[z];
        QColor c(v.x()*255, v.y()*255, v.z()*255);
        double x0 = z*w/colorZebraNum;
        double x1 = (z+1)*w/colorZebraNum;
        painter.fillRect(x0+1, 0, x1-x0-2, h, c);
    }
}
