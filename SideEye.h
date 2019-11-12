#ifndef SIDEEYE_H
#define SIDEEYE_H

#include <QObject>
#include <QWidget>

class SideEye : public QWidget
{
    Q_OBJECT
public:
    explicit SideEye(QWidget *parent = nullptr);

    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;
    //滚轮事件
    void wheelEvent(QWheelEvent* event) override;
    //鼠标按下事件
    void mousePressEvent(QMouseEvent* event) override;
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent* event) override;
    // 绘制
    void paintEvent(QPaintEvent *)  override;

signals:

public slots:
};

#endif // SIDEEYE_H
