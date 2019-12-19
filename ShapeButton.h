#ifndef SHAPEBUTTON_H
#define SHAPEBUTTON_H

#include <QObject>
#include <QPushButton>

class ShapeButton : public QPushButton
{
public:
    ShapeButton(QString shape, int w, int h, QWidget *parent = nullptr);
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    QString shape;
};

#endif // SHAPEBUTTON_H
