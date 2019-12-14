#ifndef SHAPEBUTTON_H
#define SHAPEBUTTON_H

#include <QObject>
#include <QPushButton>

class ShapeButton : public QPushButton
{
public:
    ShapeButton(QString shape, int x, int y, QWidget *parent = nullptr);

};

#endif // SHAPEBUTTON_H
