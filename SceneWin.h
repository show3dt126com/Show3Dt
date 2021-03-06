#ifndef SCENEWIN_H
#define SCENEWIN_H

#include <QWidget>
#include <QPushButton>
#include <Scene.h>
#include <QFrame>
#include <QScrollBar>
#include <QCheckBox>
#include <QLineEdit>
#include <ShapeButton.h>
#include <QTextEdit>

#include "ScrollBarV.h"
#include "BBS.h"
#include "RotatePanel.h"

class SceneWin : public QWidget, public BBSMessageProc
{
    Q_OBJECT
public:
    explicit SceneWin(QWidget *parent = nullptr);
    Scene * scene;

    RotatePanel * rotatePanel;

    ShapeButton * moveLeft, * moveRight, * moveUp, * moveDown;

    //
    QPushButton * toDefaultButton;
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
    bool keepDist;

    // 相机俯仰角调整
    QPushButton * cameraMidBut;
    QPushButton * someTestBut;

    // 视窗区域移动，前后或者上下
    ScrollBarV * vScrollBar;
    // 水平或者左右
    ScrollBarV * hScrollBar;

    // 深度，俯视图用，侧视图3D模式用
    ScrollBarV * dScrollBar;

    // 相机上下或者前后移动
    ScrollBarV * forwardScrollBar;

    // 纵切，侧视图用，侧视图3D模式用
    ScrollBarV * cutRadiusScrollBar;
    ScrollBarV * cutAngleScrollBar;

    // 抬头角度，-45..45
    ScrollBarV * headUpScrollBar;
    // 深度放大，1..10
    ScrollBarV * zoomDepthScrollBar;
    // 视窗放大  1..100
    ScrollBarV * zoomViewScrollBar;
    // 旋转视窗 0..360
    ScrollBarV * turnScrollBar;

    // 显示一些重要信息的文字框
    QTextEdit * textIinfo;

    void resizeEvent(QResizeEvent *event) override;

    // 根据具体情况更新控件的提示信息
    void updateControlToolTip();

    // 刷新声场区域的范围显示
    void updateFieldRange();

    // 根据滚动条控件的变化，刷新相机参数
    void calculateCameraPos();
    // 根据滚动条计算切空参数
    void calculateCutFileld();

    // 根据相机参数变化刷新滚动条等显示
    void updateFromCameraPos();
    // 根据切空参数变化，刷新滚动条等显示
    void updateFromCutField();
    // 根据视点参数刷新按钮
    void updateViewTypeModeCheck();

    // 更新文字信息的显示，
    void updateTextInfo();

    // bbsUser 当接收到 G.BBS 信号时回调下面的函数
    void procBBSMessage(BBSMessage bbsMsg) override;
    BBSBase bbsUser;

signals:

public slots:

    void onViewAreaVScroll(int v);
    void onViewAreaHScroll(int v);

    void onViewDepthScroll(int v);

    void onCutRadiusScroll(int v);
    void onCutAngleScroll(int v);

    void onCameraTurnScroll(int v);
    void onCameraForwardScroll(int v);

    void onHeadUpScroll(int v);
    void onZoomDepthScroll(int v);
    void onZoomViewScroll(int v);

    void onToDefault();
    void onCameraPitchMid();
    void onDimModeTogggle(bool checked);
    void onViewTypeTogle(bool checked);
    void onKeepDistToggle(bool checked);

    void onArrowMove();
    void onSomeTest();
};

#endif // SCENEWIN_H
