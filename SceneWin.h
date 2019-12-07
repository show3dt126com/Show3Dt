#ifndef SCENEWIN_H
#define SCENEWIN_H

#include <QWidget>
#include <QPushButton>
#include <Scene.h>
#include <QFrame>
#include <QScrollBar>
#include <QCheckBox>
#include <QLineEdit>

#include "ScrollBarV.h"
#include "BBS.h"

class SceneWin : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWin(QWidget *parent = nullptr);
    Scene * scene;

    //
    QPushButton * conerButton;
    // 切换俯视图、侧视图模式
    QPushButton * viewTypeToggle;
    // 切换 2D、3D模式
    QPushButton * dimModeToggle;

    // 顺时针，逆时针旋转，俯视图、侧视图都可

    // 相机位置移动，俯视图情况下上下移动，侧视图情况下前后移动
    // 如果不考虑相机俯仰角的话，是不需要移动相机的
    // 因此设置一个相机跟随选项，俯视图下保持相机据当前显示深度不变
    // 侧视图下，保持相机与当前纵切面距离不变

    // 相机跟随
    QPushButton * keepDistToggle;

    // 相机俯仰角调整
    QPushButton * cameraMidBut;

    // 视窗区域移动
    ScrollBarV * vScrollBar;
    ScrollBarV * hScrollBar;

    // 深度，俯视图用，侧视图3D模式用
    ScrollBarV * dScrollBar;
    // 纵切，侧视图用，侧视图3D模式用
    ScrollBarV * rScrollBar;

    // 抬头角度，-45..45
    ScrollBarV * headUpScrollBar;
    // 深度放大，100..1000
    ScrollBarV * zoomDepthScrollBar;
    // 视窗放大  1000..1
    ScrollBarV * zoomViewScrollBar;
    // 旋转视窗 0..360
    ScrollBarV * turnScrollBar;

    // 显示一些重要信息的文字框
    QLineEdit * info1;
    QLineEdit * info2;

    void resizeEvent(QResizeEvent *event) override;
    void setArea(int w, int h);
    void setValue(int x, int y);
    void adjustPageStep();

    // 刷新声场区域的范围显示
    void updateFieldRange();

    BBSBase bbsUser;

signals:

public slots:
    void onViewAreaVScroll(int v);
    void onViewAreaHScroll(int v);
    void onViewDepthScroll(int v);

    void onViewZoomIn();
    void onViewZoomOut();

    void onTurnClock();
    void onTurnAClock();

    void onForward();
    void onBackward();

    void onCameraUp();
    void onCameraDown();
    void onCameraMid();

    void onZoomInDepth();
    void onZoomOutDepth();

    void onDimModeTogggle(bool checked);
    void onViewTypeToggle(bool checked);
    void onKeepDistToggle(bool checked);

public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

#endif // SCENEWIN_H
