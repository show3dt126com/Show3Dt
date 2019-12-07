#ifndef SCROLLBARV_H
#define SCROLLBARV_H

#include <QObject>
#include <QScrollBar>

class ScrollBarV : public QScrollBar
{
public:
    ScrollBarV(Qt::Orientation, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

    QString fmt;
};

#endif // SCROLLBARV_H
