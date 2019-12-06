#ifndef QSCROLLBARV_H
#define QSCROLLBARV_H

#include <QObject>
#include <QScrollBar>

class QScrollBarV : public QScrollBar
{
public:
    QScrollBarV(Qt::Orientation orientation, QWidget *parent =nullptr);

    // 绘制当前值
    void paintEvent(QPaintEvent *) override;
    //
    QString fmt;
};

#endif // QSCROLLBARV_H
