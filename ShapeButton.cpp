#include "ShapeButton.h"
#include <QPixmap>
#include <QBitmap>
#include <QDebug>

ShapeButton::ShapeButton(QString shape, int w, int h, QWidget *parent)
    : QPushButton (parent)
{
    QString shapeF = ":/" + shape + "1.png";
    QPixmap pix(shapeF);
    QBitmap shapeB = pix.createHeuristicMask();
    //QBitmap shapeB = pix.createMaskFromColor(QColor(255,0,0), Qt::MaskInColor);
    //qDebug() << "shapeB=" << shapeF << pix.width() << pix.height();
    setMask(shapeB);
    //
    int pw = pix.width();
    int ph = pix.height();

    setMinimumSize(w, h);
    setMaximumSize(w, h);
    char style[300];
    char * icon = shape.toLatin1().data();
    sprintf(style, "QPushButton{border-image:url(:/%s1.png);}"
                   "QPushButton:hover{border-image:url(:/%s2.png);}"
                   "QPushButton:pressed{border-image:url(:/%s3.png);}",
            icon, icon, icon);
    setStyleSheet(style);
}
