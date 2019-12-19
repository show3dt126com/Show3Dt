#include "RotatePanel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
#include <QPixmap>
#include <QtMath>
#include <QDebug>
#include "Global.h"

RotatePanel::RotatePanel(int x, int y, QWidget *parent)
    : QWidget(parent), BBSMessageProc()
{
    m_title = "方向";
    setWindowFlags(Qt::FramelessWindowHint);//无窗体
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
    setGeometry(x, y, 120, 120);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    QPixmap pix(width(), height());
    QPainter painter(&pix);
    painter.fillRect(0,0,width(), height(), QBrush(QColor(255, 0,0)));
    painter.setBrush(QBrush(QColor(0,255,0)));
    painter.drawEllipse(0, 0, width(), height());
    QBitmap shapeB = pix.createHeuristicMask();
    setMask(shapeB);
    setToolTip("用箭头控制旋转");

    cutA = 90;
    camA = 45;

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    bbsUser.init(this);
    setNotFocusedColor();
}

void RotatePanel::procBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == &bbsUser)
        return;
    if (bbsMsg.varity == EBV_Camera)
        updateFromCameraPos();
    if (bbsMsg.varity == EBV_VCut)
        updateFromCutField();
}

void RotatePanel::updateFromCutField()
{
    cutA = G.viewPot.fieldCut.vCutAngle;
    repaint();
}

void RotatePanel::updateFromCameraPos()
{
    if (G.viewPot.viewType == EVT_Down)
        camA = G.viewPot.cameraPar.roll;
    else
        camA = G.viewPot.cameraPar.yaw;
    repaint();
}

void RotatePanel::calculateCutField()
{
    G.viewPot.fieldCut.vCutAngle = cutA;
    BBSMessage msg(EBS_RotatePanel, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void RotatePanel::calculateCameraPos()
{
    if (G.viewPot.viewType == EVT_Down)
        G.viewPot.cameraPar.roll = camA;
    else
        G.viewPot.cameraPar.yaw = camA;
    BBSMessage msg(EBS_RotatePanel, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
    qDebug() << "RotatePanel::calculateCameraPos()" << camA;
    qDebug() <<"";
}

void RotatePanel::mousePressEvent(QMouseEvent *event)
{
    bool isCut = event->modifiers() == Qt::AltModifier;
    //int xpos = event->x();
    //int ypos = event->y();
    qDebug() << "RotatePanel::mousePressEvent";
    if (event->button() == Qt::LeftButton)
    {
        if (isCut)
        {
            cutA = (cutA+10) % 360;
            calculateCutField();
        }
        else
        {
            camA = (camA+10) % 360;
            calculateCameraPos();
        }
        repaint();

    }
    else if (event->button() == Qt::RightButton)
    {
        if (isCut)
        {
            cutA = (cutA+350) % 360;
            calculateCutField();
        }
        else
        {
            camA = (camA+350) % 360;
            calculateCameraPos();
        }
        repaint();
        calculateCutField();
    }
}

void RotatePanel::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Minus)//Key_Left)
    {
        camA = (camA+10) % 360;
        repaint();
        calculateCameraPos();
    }
    else if(event->key() == Qt::Key_Equal)//Key_Right)
    {
        camA = (camA+350) % 360;
        repaint();
        calculateCameraPos();
    }
    else if(event->key() == Qt::Key_BracketLeft)//Key_Up)
    {
        cutA = (cutA+10) % 360;
        repaint();
        calculateCutField();
    }
    else if(event->key() == Qt::Key_BracketRight)//Key_Down)
    {
        cutA = (cutA+350) % 360;
        repaint();
        calculateCutField();
    }
}

void RotatePanel::paintEvent(QPaintEvent *event)
{
    QPainter painter (this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/2, height()/2);
    int side = qMin(width(), height());
    painter.scale(side/200.0, side/200.0);

    drawCrown(&painter);
    drawBackground(&painter);
    drawScaleNum(&painter);

    // 画刻度线  画单位  画数字显示
    drawScale(&painter);
    drawTitle(&painter);
    drawIndicator(&painter);
    drawCam(&painter);
}

void RotatePanel::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    setFocusedColor();
}

void RotatePanel::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    setNotFocusedColor();
}

void RotatePanel::setFocusedColor()
{
    backColor = Qt::black;
    fontColor = Qt::green;
    camColor = QColor(255,255,0);
    arrowColor = QColor(220,60,60);
    crownColor1 = Qt::white;
    crownColor2 = Qt::yellow;
    update();
}

void RotatePanel::setNotFocusedColor()
{
    backColor = Qt::gray;
    fontColor = Qt::green;
    camColor = QColor(105,105,0);
    arrowColor = QColor(120,60,60);
    crownColor1 = Qt::white;
    crownColor2 = Qt::gray;
    update();
}

void RotatePanel::drawCrown(QPainter *painter)
{
    painter->save();
    int radius = 100;
    QLinearGradient lg1(0,-radius, 0,radius);
    lg1.setColorAt(0.2, crownColor1);
    lg1.setColorAt(1, crownColor2);
    painter->setBrush(lg1);
    painter->drawEllipse(-99, -99, 198, 198);
    painter->restore();
}

void RotatePanel::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setBrush(backColor);
    int z = 3;
    painter->drawEllipse(-90-z, -90-z, 180+z+z, 180+z+z);
    painter->restore();
}

void RotatePanel::drawScaleNum(QPainter *painter)
{
    painter->save();
    painter->setPen(fontColor);
    QFont font("Times New Roman", 18, 500);
    painter->setFont(font);
    QFontMetricsF fm(font);
    int scW = fm.size(Qt::TextSingleLine, "E").width();
    int scH = fm.size(Qt::TextSingleLine, "E").height()-8;
    int r = 74;
    painter->drawText(-scW/2, -r+scH, "N");
    painter->drawText(r-scW+3, scH/2, "E");
    painter->drawText(-scW/2, r, "S");
    painter->drawText(-r,  scH/2, "W");
    painter->restore();
}

void RotatePanel::drawScale(QPainter *painter)
{
    painter->save();
    painter->rotate(0);
    int steps = 12;
    double angleStep = 360.0 / steps;
    painter->setPen(fontColor); //
    QPen pen = painter->pen();
    for (int i = 0; i <= steps; i++)
    {
        // 四个方向刻度显示加粗
        if (i % 3 == 0)
        {
            pen.setWidth(4);
            painter->setPen(pen);
            painter->drawLine(0, 78, 0, 92);
        }
        else
        {
            pen.setWidth(2);
            painter->setPen(pen);
            painter->drawLine(0, 87, 0, 92);
        }
        painter->rotate(angleStep);
    }
    painter->restore();
}

// QFontMetricsF这个类用于获取字体的像素信息，例如字体的高度，
// 一个给定字符串的像素宽度等等。
void RotatePanel::drawTitle(QPainter *painter)
{
    painter->save();
    painter->setPen(fontColor);
    //painter->setBrush(m_foreground);
    QFont font("黑体", 18, 500);
    painter->setFont(font);
    QFontMetricsF fm(font);
    double w = fm.size(Qt::TextSingleLine, m_title).width();
    painter->drawText(int(-w/2), -30, m_title);
    painter->restore();
}

void RotatePanel::drawIndicator(QPainter *painter)
{
    painter->save();
    QPolygon pts;
    // (-2,0)/(2,0)/(0,60) 第一个参数是 ，坐标的个数。后边的是坐标
    pts.setPoints(3, -12,0, 12,0, 0,90);

    //画指针 //顺时针旋转坐标系统
    painter->rotate(cutA);
    QRadialGradient haloGradient(0, 0, 60, 0, 0);  //辐射渐变
    haloGradient.setColorAt(0, arrowColor);
    haloGradient.setColorAt(1, QColor(160,160,160)); //灰
    painter->setPen(Qt::white); //定义线条文本颜色  设置线条的颜色
    painter->setBrush(haloGradient);//刷子定义形状如何填满 填充后的颜色
    painter->drawConvexPolygon(pts); //这是个重载函数，绘制多边形。

    //画中心点
    QColor niceBlue(150, 150, 200);
    QConicalGradient coneGradient(0, 0, -90.0);  //角度渐变
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);
    painter->setPen(Qt::NoPen);  //没有线，填满没有边界
    painter->setBrush(coneGradient);
    int r = 12;
    painter->drawEllipse(-r, -r, r+r, r+r);
    painter->restore();
}

void RotatePanel::drawCam(QPainter *painter)
{
    painter->save();
    painter->rotate(90 + camA);
    painter->setBrush(QBrush(camColor));
    int x = -92;
    int w = 26;
    int h = 40;
    painter->drawRect(x, -h/2, w, h);
    int lx0 = x+w;
    int lx1 = x+w-9;
    int dy = h*0.25;
    painter->drawLine(lx0, 0, lx1, 0);
    painter->drawLine(lx0, -dy, lx1, -dy);
    painter->drawLine(lx0,  dy, lx1,  dy);
    painter->restore();
}
