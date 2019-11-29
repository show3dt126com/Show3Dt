#ifndef SCENEWIN_H
#define SCENEWIN_H

#include <QWidget>
#include <QPushButton>
#include <Scene.h>
#include <QFrame>
#include <QScrollBar>
#include <QCheckBox>
#include <QLineEdit>
#include "BBS.h"

class SceneWin : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWin(QWidget *parent = nullptr);
    Scene * scene;
    QPushButton * conerButton;

    QPushButton * viewTypeToggle;
    QPushButton * dimensionModeToggle;

    // 顺时针，逆时针旋转，俯视图、侧视图都可
    QPushButton * turnClockBut;
    QPushButton * turnAClockBut;

    // 相机位置移动
    QPushButton * forwardBut;
    QPushButton * backwardBut;

    // 相机俯仰角调整
    QPushButton * cameraUpBut;
    QPushButton * cameraDownBut;
    QPushButton * cameraMidBut;

    // 高度比例尺调整
    QPushButton * zoomInDepthBut;
    QPushButton * zoomOutDepthBut;

    // 视窗放大缩小
    QPushButton * zoomInBut;
    QPushButton * zoomOutBut;

    // 视窗区域移动
    QScrollBar * vScrollBar;
    QScrollBar * hScrollBar;

    // 深度，俯视图用，侧视图3D模式用
    QScrollBar * dScrollBar;
    // 纵切，侧视图用，侧视图3D模式用
    QScrollBar * rScrollBar;

    // 保留一些间隔
    QFrame * frameInScrollsH;
    QFrame * frameInScrollsV;

    // 显示一些重要信息的文字框
    QLineEdit * info1;
    QLineEdit * info2;

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

    void onForward();
    void onBackward();

    void onCameraUp();
    void onCameraDown();
    void onCameraMid();

    void onZoomInDepth();
    void onZoomOutDepth();

public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

#endif // SCENEWIN_H
