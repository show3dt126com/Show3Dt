#ifndef ROTATEPANEL_H
#define ROTATEPANEL_H

#include <QObject>
#include <QWidget>

#include "ShapeButton.h"
#include "BBS.h"

class RotatePanel : public QWidget
{
    Q_OBJECT
public:
    explicit RotatePanel(int x, int y, QWidget *parent = nullptr);

    void updateFromCutField();
    void updateFromCameraPos();
    void calculateCutFidld();
    void calculateCameraPos();

    int camA;
    int cutA;
    BBSBase bbsUser;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void drawCrown(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawIndicator(QPainter *painter);
    void drawCam(QPainter *painter);


public:
    QColor m_background;
    QColor m_foreground;

    QString m_title;

signals:

public slots:
    int onBBSMessage(BBSMessage bbsMsg);

};

#endif // ROTATEPANEL_H
