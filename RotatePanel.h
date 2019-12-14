#ifndef ROTATEPANEL_H
#define ROTATEPANEL_H

#include <QObject>
#include <QWidget>

#include "ShapeButton.h"

class RotatePanel : public QWidget
{
    Q_OBJECT
public:
    explicit RotatePanel(int x, int y, QWidget *parent = nullptr);

    int w, h;
    int camA;
    int cutA;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:

public slots:

};

#endif // ROTATEPANEL_H
