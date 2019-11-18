#ifndef SCENEWIN_H
#define SCENEWIN_H

#include <QWidget>
#include <QPushButton>
#include <Scene.h>
#include <QScrollBar>
#include "BBS.h"

class SceneWin : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWin(QWidget *parent = nullptr);
    Scene * scene;
    QPushButton * conerButton;

    // 顺时针，逆时针旋转，俯视图、侧视图都可
    QPushButton * turnClockBut;
    QPushButton * turnAClockBut;

    QPushButton * zoomInBut;
    QPushButton * zoomOutBut;

    QScrollBar * vScrollBar;
    QScrollBar * hScrollBar;
    // 深度，俯视图用，侧视图3D模式用
    QScrollBar * dScrollBar;

    void resizeEvent(QResizeEvent *event) override;
    void setArea(int w, int h);
    void setValue(int x, int y);
    void adjustPageStep();

    BBSBase bbsUser;

signals:

public slots:
    void onVScroll(int v);
    void onHScroll(int v);
    void onDScroll(int v);
    void onZoomIn();
    void onZoomOut();
    void onTurnClock();
    void onTurnAClock();

public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

#endif // SCENEWIN_H
