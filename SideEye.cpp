#include "SideEye.h"
#include <QPainter>

SideEye::SideEye(QWidget *parent) : QWidget(parent)
{

    setWindowTitle("侧视全局图");
}


// 鼠标移动事件
void SideEye::mouseMoveEvent(QMouseEvent* event)
{

}

//滚轮事件
void SideEye::wheelEvent(QWheelEvent* event)
{

}

//鼠标按下事件
void SideEye::mousePressEvent(QMouseEvent* event)
{

}

//鼠标释放事件
void SideEye::mouseReleaseEvent(QMouseEvent* event)
{

}

// 绘制
void SideEye::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.drawText(20,20, "侧视图全局图");
}
