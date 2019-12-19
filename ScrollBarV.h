#ifndef SCROLLBARV_H
#define SCROLLBARV_H

#include <QObject>
#include <QScrollBar>

class ScrollBarV : public QScrollBar
{
public:
    ScrollBarV(Qt::Orientation, QString tag, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void setValue(int value);
    void setSliderPos(int pos);
    QString fmt;
    QString tag;
};

#endif // SCROLLBARV_H
