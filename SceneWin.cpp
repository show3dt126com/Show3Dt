#include "SceneWin.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSpacerItem>
#include <QBitmap>

/*
 SceneWin 是 Scene 窗口外围包裹的操作窗口。
 外围的小操控是各种操作安装、滚动条、信息内容

┌────────────────────────────────────────────────────────┐
│ Scene Caption: some static information                 │
├─────────────────────────────────────────────────┬──┬┬──┤
│                                                 │  ││  │
│                                                 │  ││  │
│                                                 │  ││  │
│                                                 │  ││  │
│                Scene                            │  ││  │
│                                                 │  ││  │
│                                                 ├──┴┴──┤
│                                                 │  B   │
│                                                 ├──────┤
│                                                 │  B   │
│                                                 ├──────┤
│                                                 │  B   │
├──────────────┬────────────────┬─────┬─────┬─────┼──────┤
│              │                │     │     │     │      │
├──────────────┴────────────────┤  A  │  A  │  A  │  C   │
├──────────────┬────────────────┤     │     │     │      │
│              │                │     │     │     │      │
└──────────────┴────────────────┴─────┴─────┴─────┴──────┘


┌───────────────────────────────────────────────────────┐
│ Scene Caption: some static information                │
├─────────────────────────────────────────────────┬──┬──┤
│                                                 │  │  │
│                                                 │H1│H1│
│                                                 │-0│-1│
│                Scene                            │  │  │
│                                                 ├──┼──┤
│                                                 │  │  │
│                                                 │H2│H2│                                                 │-0│  │
│                                                 │-0│-1│
│                                                 │  │  │
├──────────────┬────────────────┬─────┬─────┬─────┼──┴──┤
│      V1-0    │      V2-0      │     │     │     │     │
├──────────────┼────────────────┤H3-0 │H3-1 │H3-2 │  C  │
│      V1-1    │      V2-1      │     │     │     │     │
└──────────────┴────────────────┴─────┴─────┴─────┴─────┘

*/

// 设置按钮的大小和图片属性
void setButtonMinMaxIcon(QPushButton * w, int minSz, int maxSz, const char * icon=nullptr)
{
    w->setMinimumSize(minSz, minSz);
    w->setMaximumSize(maxSz, maxSz);
    if (icon)
    {
        char style[300];
        sprintf(style, "QPushButton{border-image:url(:/%s.png);}"
                       "QPushButton:hover{border-image:url(:/%s-H.png);}"
                       "QPushButton:pressed{border-image:url(:/%s-P.png);}",
                icon, icon, icon);
        w->setStyleSheet(style);
    }
}

// 设置具备按下状态的按钮的大小和图片属性
void setCheckBoxMinMaxIcon(QPushButton * w, int minSz, int maxSz, const char * icon=nullptr)
{
    w->setMinimumSize(minSz, minSz);
    w->setMaximumSize(maxSz, maxSz);
    w->setCheckable(true);
    if (icon)
    {
        char style[300];
        sprintf(style, "QPushButton{border-image:url(:/%s-H.png);}"
                       "QPushButton::hover{border-image:url(:/%s-HH.png);}"
                       "QPushButton::checked:hover{border-image:url(:/%s-VH.png);}"
                       "QPushButton:checked{border-image:url(:/%s-V.png);}",
                icon, icon, icon,icon);
        w->setStyleSheet(style);
    }
}

/*
SceneWin::SceneWin(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->setRowStretch(0, 100);
    layout->setColumnStretch(0, 100);
    layout->setColumnStretch(1, 100);
    layout->setColumnMinimumWidth(1, 300);

    QHBoxLayout * layoutA = new QHBoxLayout();
    QVBoxLayout * layoutB = new QVBoxLayout();

    setLayout(layout);
    frameInScrollsH = new QFrame(this);
    frameInScrollsV = new QFrame(this);

    viewTypeToggle = new QPushButton(this);
    viewTypeToggle->setToolTip("切换俯视图/侧视图模式");
    dimModeToggle = new QPushButton(this);
    dimModeToggle->setToolTip("切换2D/3D模式");
    keepDistToggle = new QPushButton(this);
    dimModeToggle->setToolTip("保持相机与切面距离");

    conerButton = new QPushButton("*", this);
    conerButton->setStyleSheet(
        //"QPushButton{color: white;font: bold 10pt;border:none;"
        //"min-height: 24px;min-width: 60px;"
        //"background:url(:images/SimpleButtom.png) -60px 0px no-repeat;}");
        "border-image:url(:/Start.png);}");

    zoomInBut = new QPushButton("", this);
    zoomInBut->setToolTip("放大视窗范围");
    zoomOutBut = new QPushButton("", this);
    zoomOutBut->setToolTip("缩小视窗范围");

    turnClockBut = new QPushButton(this);
    turnClockBut->setToolTip("视区方向/相机朝向顺时针旋转");
    turnAClockBut = new QPushButton(this);
    turnAClockBut->setToolTip("视区方向/相机朝向逆时针旋转");

    forwardBut = new QPushButton(">>", this);
    forwardBut->setToolTip("相机往前推进");
    backwardBut = new QPushButton("<<", this);
    backwardBut->setToolTip("相机往后移动");

    cameraUpBut = new QPushButton("↑", this);
    cameraUpBut->setToolTip("增加相机抬头角度");
    cameraDownBut = new QPushButton("↓", this);
    cameraDownBut->setToolTip("减小相机抬头角度");
    cameraMidBut = new QPushButton("=", this);
    cameraMidBut->setToolTip("相机俯仰角归零");

    zoomInDepthBut = new QPushButton("×", this);
    zoomInDepthBut->setToolTip("放大深度放大系数");
    zoomOutDepthBut = new QPushButton("÷", this);
    zoomOutDepthBut->setToolTip("减小深度放大系数");

    frameInScrollsV->setStyleSheet("background-color: rgb(42, 42, 42);");
    frameInScrollsV->setToolTip("");
    frameInScrollsV->setMaximumWidth(5);
    frameInScrollsV->setMinimumWidth(5);
    frameInScrollsH->setStyleSheet("background-color: rgb(42, 42, 42);");
    frameInScrollsH->setMaximumHeight(5);
    frameInScrollsH->setMinimumHeight(5);

    int maxSz = 35;
    int minSz = 35;
    setButtonMinMaxIcon(conerButton, minSz, maxSz);
    setButtonMinMaxIcon(turnClockBut, minSz, maxSz, "Clock");
    setButtonMinMaxIcon(turnAClockBut, minSz, maxSz, "ClockA");
    setButtonMinMaxIcon(zoomInBut, minSz, maxSz, "ZoomIn");
    setButtonMinMaxIcon(zoomOutBut, minSz, maxSz, "ZoomOut");
    setButtonMinMaxIcon(forwardBut, minSz, maxSz);
    setButtonMinMaxIcon(backwardBut, minSz, maxSz);
    setButtonMinMaxIcon(cameraUpBut, minSz, maxSz);
    setButtonMinMaxIcon(cameraDownBut, minSz, maxSz);
    setButtonMinMaxIcon(cameraMidBut, minSz, maxSz);
    setButtonMinMaxIcon(zoomInDepthBut, minSz, maxSz);
    setButtonMinMaxIcon(zoomOutDepthBut, minSz, maxSz);

    setCheckBoxMinMaxIcon(viewTypeToggle, minSz, maxSz, "ZViewType");
    setCheckBoxMinMaxIcon(dimModeToggle, minSz, maxSz, "ZDimension");
    setCheckBoxMinMaxIcon(keepDistToggle, minSz, maxSz, "ZKeepDist");

    maxSz = 18;
    vScrollBar = new QScrollBar(Qt::Vertical, this);
    vScrollBar->setToolTip("视区往相机前方/上方移动");
    vScrollBar->setMaximumWidth(maxSz);

    hScrollBar =new QScrollBar(Qt::Horizontal, this);
    hScrollBar->setToolTip("视区往相机侧方/水平方向移动");
    hScrollBar->setMaximumHeight(maxSz);

    dScrollBar = new QScrollBar(Qt::Vertical, this);
    dScrollBar->setMaximumWidth(maxSz);
    dScrollBar->setToolTip("深度切面上下移动");

    rScrollBar = new QScrollBar(Qt::Horizontal, this);
    rScrollBar->setMaximumHeight(maxSz);
    rScrollBar->setToolTip("纵切面前后移动");

    headUpScrollBar = new QScrollBar(Qt::Vertical, this);
    headUpScrollBar->setMaximumWidth(maxSz);
    headUpScrollBar->setToolTip("相机抬头角度，-45..45");
    headUpScrollBar->setMinimum(-45);
    headUpScrollBar->setMaximum(45);
    headUpScrollBar->setPageStep(5);

    zoomDepthScrollBar = new QScrollBar(Qt::Vertical, this);
    zoomDepthScrollBar->setMaximumWidth(maxSz);
    zoomDepthScrollBar->setToolTip("深度放大系数，100%..1000%");
    zoomDepthScrollBar->setMinimum(100);
    zoomDepthScrollBar->setMaximum(1000);
    zoomDepthScrollBar->setPageStep(50);

    zoomViewScrollBar = new QScrollBar(Qt::Horizontal, this);
    zoomViewScrollBar->setMaximumHeight(maxSz);
    zoomViewScrollBar->setToolTip("视窗放大系数 1000(全部)..1");
    zoomViewScrollBar->setMinimum(1);
    zoomViewScrollBar->setMaximum(1000);

    turnScrollBar = new QScrollBar(Qt::Horizontal, this);
    turnScrollBar->setMaximumHeight(maxSz);
    turnScrollBar->setToolTip("旋转视窗，0(北)..360");
    turnScrollBar->setMinimum(0);
    turnScrollBar->setMaximum(359);

    scene = new Scene(this);

    info1 = new QLineEdit(this);
    //info1->setMaximumWidth(400);
    info1->setText("坐标数据");
    info2 = new QLineEdit(this);
    info2->setText("相机参数");

    layoutA->addSpacing(20);
    layoutA->addWidget(viewTypeToggle, 0, Qt::AlignVCenter);
    layoutA->addWidget(dimModeToggle, 0, Qt::AlignVCenter);
    layoutA->addWidget(keepDistToggle, 0, Qt::AlignVCenter);

    layoutA->addSpacing(20);
    layoutA->addWidget(cameraUpBut, 0, Qt::AlignVCenter);
    layoutA->addWidget(cameraDownBut, 0, Qt::AlignVCenter);
    layoutA->addWidget(cameraMidBut, 0, Qt::AlignVCenter);

    layoutA->addSpacing(20);
    layoutA->addWidget(zoomInDepthBut, 0, Qt::AlignVCenter);
    layoutA->addWidget(zoomOutDepthBut, 0, Qt::AlignVCenter);
    layoutA->addSpacing(20);
    layoutA->addWidget(forwardBut, 0, Qt::AlignVCenter);
    layoutA->addWidget(backwardBut, 0, Qt::AlignVCenter);
    layoutA->addSpacing(20);
    layoutA->addWidget(turnClockBut, 0, Qt::AlignVCenter);

    layoutB->addSpacing(20);
    layoutB->addWidget(zoomInBut, 0, Qt::AlignHCenter);
    layoutB->addWidget(zoomOutBut, 0, Qt::AlignHCenter);
    layoutB->addSpacing(20);
    layoutB->addWidget(turnAClockBut, 0, Qt::AlignHCenter);

    layout->addWidget(scene, 0, 0, 2, 3);
    layout->addWidget(info1, 2, 0, 1, 1);
    layout->addWidget(hScrollBar, 2, 1, 1, 1);
    layout->addWidget(frameInScrollsH, 3, 0, 1, 2);
    layout->addWidget(info2, 4, 0, 1, 1);
    layout->addWidget(rScrollBar, 4, 1, 1, 1);
    layout->addLayout(layoutA, 2, 2, 3, 1);

    layout->addWidget(vScrollBar, 0, 3, 1, 1);
    layout->addWidget(frameInScrollsV, 0, 4, 1, 1);
    layout->addWidget(dScrollBar, 0, 5, 1, 1);
    layout->addLayout(layoutB, 1, 3, 1, 3);

    layout->addWidget(conerButton, 2, 3, 3, 3);

    scene->show();

    connect(viewTypeToggle, SIGNAL(toggled(bool)), this, SLOT(onViewTypeToggle(bool)));
    connect(dimModeToggle, SIGNAL(toggled(bool)), this, SLOT(onDimModeTogggle(bool)));
    connect(keepDistToggle, SIGNAL(toggled(bool)), this, SLOT(onKeepDistTogggle(bool)));

    connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaVScroll(int)));
    connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaHScroll(int)));
    connect(dScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewDepthScroll(int)));
    connect(&bbsUser, SIGNAL(bbsMessage(BBSMessage)), this, SLOT(onBBSMessage(BBSMessage)));

    connect(zoomInBut, SIGNAL(pressed()), this, SLOT(onViewZoomIn()));
    connect(zoomOutBut, SIGNAL(pressed()), this, SLOT(onViewZoomOut()));
    connect(turnClockBut, SIGNAL(pressed()), this, SLOT(onTurnClock()));
    connect(turnAClockBut, SIGNAL(pressed()), this, SLOT(onTurnAClock()));

    connect(forwardBut, SIGNAL(pressed()), this, SLOT(onForward()));
    connect(backwardBut, SIGNAL(pressed()), this, SLOT(onBackward()));

    connect(cameraUpBut, SIGNAL(pressed()), this, SLOT(onCameraUp()));
    connect(cameraDownBut, SIGNAL(pressed()), this, SLOT(onCameraDown()));
    connect(cameraMidBut, SIGNAL(pressed()), this, SLOT(onCameraMid()));

    connect(zoomInDepthBut, SIGNAL(pressed()), this, SLOT(onZoomInDepth()));
    connect(zoomOutDepthBut, SIGNAL(pressed()), this, SLOT(onZoomOutDepth()));

    setArea(10000, 10000);
    setValue(500, 500);
}
*/


SceneWin::SceneWin(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->setRowStretch(0, 100);
    layout->setColumnStretch(0, 100);
    layout->setColumnMinimumWidth(1, 300);
    setLayout(layout);

    return;

    frameInScrollsH = new QFrame(this);
    frameInScrollsV = new QFrame(this);

    viewTypeToggle = new QPushButton(this);
    viewTypeToggle->setToolTip("切换俯视图/侧视图模式");
    dimModeToggle = new QPushButton(this);
    dimModeToggle->setToolTip("切换2D/3D模式");
    keepDistToggle = new QPushButton(this);
    dimModeToggle->setToolTip("保持相机与切面距离");

    conerButton = new QPushButton("*", this);
    conerButton->setStyleSheet(
        //"QPushButton{color: white;font: bold 10pt;border:none;"
        //"min-height: 24px;min-width: 60px;"
        //"background:url(:images/SimpleButtom.png) -60px 0px no-repeat;}");
        "border-image:url(:/Start.png);}");

    frameInScrollsV->setStyleSheet("background-color: rgb(42, 42, 42);");
    frameInScrollsV->setToolTip("");
    frameInScrollsV->setMaximumWidth(5);
    frameInScrollsV->setMinimumWidth(5);
    frameInScrollsH->setStyleSheet("background-color: rgb(42, 42, 42);");
    frameInScrollsH->setMaximumHeight(5);
    frameInScrollsH->setMinimumHeight(5);

    int maxSz = 35;
    int minSz = 35;
    setButtonMinMaxIcon(conerButton, minSz, maxSz);
    setButtonMinMaxIcon(cameraMidBut, minSz, maxSz);

    setCheckBoxMinMaxIcon(viewTypeToggle, minSz, maxSz, "ZViewType");
    setCheckBoxMinMaxIcon(dimModeToggle, minSz, maxSz, "ZDimension");
    setCheckBoxMinMaxIcon(keepDistToggle, minSz, maxSz, "ZKeepDist");

    maxSz = 18;
    vScrollBar = new QScrollBar(Qt::Vertical, this);
    vScrollBar->setToolTip("视区往相机前方/上方移动");
    vScrollBar->setMaximumWidth(maxSz);

    hScrollBar =new QScrollBar(Qt::Horizontal, this);
    hScrollBar->setToolTip("视区往相机侧方/水平方向移动");
    hScrollBar->setMaximumHeight(maxSz);

    dScrollBar = new QScrollBar(Qt::Vertical, this);
    dScrollBar->setMaximumWidth(maxSz);
    dScrollBar->setToolTip("深度切面上下移动");

    rScrollBar = new QScrollBar(Qt::Horizontal, this);
    rScrollBar->setMaximumHeight(maxSz);
    rScrollBar->setToolTip("纵切面前后移动");

    headUpScrollBar = new QScrollBar(Qt::Vertical, this);
    headUpScrollBar->setMaximumWidth(maxSz);
    headUpScrollBar->setToolTip("相机抬头角度，-45..45");
    headUpScrollBar->setMinimum(-45);
    headUpScrollBar->setMaximum(45);
    headUpScrollBar->setPageStep(5);

    zoomDepthScrollBar = new QScrollBar(Qt::Vertical, this);
    zoomDepthScrollBar->setMaximumWidth(maxSz);
    zoomDepthScrollBar->setToolTip("深度放大系数，100%..1000%");
    zoomDepthScrollBar->setMinimum(100);
    zoomDepthScrollBar->setMaximum(1000);
    zoomDepthScrollBar->setPageStep(50);

    zoomViewScrollBar = new QScrollBar(Qt::Horizontal, this);
    zoomViewScrollBar->setMaximumHeight(maxSz);
    zoomViewScrollBar->setToolTip("视窗放大系数 1000(全部)..1");
    zoomViewScrollBar->setMinimum(1);
    zoomViewScrollBar->setMaximum(1000);

    turnScrollBar = new QScrollBar(Qt::Horizontal, this);
    turnScrollBar->setMaximumHeight(maxSz);
    turnScrollBar->setToolTip("旋转视窗，0(北)..360");
    turnScrollBar->setMinimum(0);
    turnScrollBar->setMaximum(359);

    scene = new Scene(this);

    info1 = new QLineEdit(this);
    //info1->setMaximumWidth(400);
    info1->setText("坐标数据");
    info2 = new QLineEdit(this);
    info2->setText("相机参数");

    /*
    ┌──────────────────────────────────────────────────┐
    │ Scene Caption: some static information           │
    ├────────────────────────────────────────────┬──┬──┤
    │                                            │H1│H1│
    │                                            │-0│-1│
    │              Scene                         │  │  │
    │                                            ├──┼──┤
    │                                            │H2│H2│                                                 │-0│  │
    │                                            │-0│-1│
    │                                            │  │  │
    ├────────────┬─────────────┬─────┬─────┬─────┼──┴──┤
    │    V1-0    │      V2-0   │     │     │     │     │
    ├────────────┼─────────────┤H3-0 │H3-1 │H3-2 │  C  │
    │    V1-1    │      V2-1   │     │     │     │     │
    └────────────┴─────────────┴─────┴─────┴─────┴─────┘
    */

    QHBoxLayout * layoutH1 = new QHBoxLayout();
    QHBoxLayout * layoutH2 = new QHBoxLayout();
    QHBoxLayout * layoutH3 = new QHBoxLayout();
    QVBoxLayout * layoutV1 = new QVBoxLayout();
    QVBoxLayout * layoutV2 = new QVBoxLayout();
    QHBoxLayout * layoutB = new QHBoxLayout();
    QSplitter * spliterH = new QSplitter(Qt::Orientation::Horizontal);
    QSplitter * spliterV = new QSplitter(Qt::Orientation::Vertical);

    layoutH1->addWidget(vScrollBar);
    layoutH1->addSpacing(5);
    layoutH1->addWidget(dScrollBar);

    layoutH2->addWidget(zoomDepthScrollBar);
    layoutH2->addSpacing(5);
    layoutH2->addWidget(turnScrollBar);

    QFrame * frameH1 = new QFrame();
    frameH1->setLayout(layoutH1);
    QFrame * frameH2 = new QFrame();
    frameH2->setLayout(layoutH2);

    spliterV->addWidget(frameH1);
    spliterV->addWidget(frameH2);
    spliterV->setStretchFactor(0, 70);
    spliterV->setStretchFactor(1, 30);

    QFrame * frameV1 = new QFrame();
    frameV1->setLayout(layoutV1);
    QFrame * frameV2 = new QFrame();
    frameV2->setLayout(layoutV2);

    spliterH->addWidget(frameV1);
    spliterH->addWidget(frameV2);
    spliterH->setStretchFactor(0, 70);
    spliterH->setStretchFactor(1, 30);

    layoutH3->addWidget(cameraMidBut);
    layoutH3->addWidget(viewTypeToggle);
    layoutH3->addWidget(dimModeToggle);
    layoutH3->addWidget(conerButton);

    layoutV1->addWidget(hScrollBar);
    layoutV1->addSpacing(5);
    layoutV1->addWidget(zoomViewScrollBar);

    layoutV2->addWidget(headUpScrollBar);
    layoutV2->addSpacing(5);
    layoutV2->addWidget(zoomDepthScrollBar);

    layoutB->addWidget(spliterV);
    layoutB->addLayout(layoutH3);

    layout->addWidget(scene, 0, 0);
    layout->addWidget(spliterH, 0,1);
    layout->addLayout(layoutB, 1,0,1,2);


    scene->show();

    connect(viewTypeToggle, SIGNAL(toggled(bool)), this, SLOT(onViewTypeToggle(bool)));
    connect(dimModeToggle, SIGNAL(toggled(bool)), this, SLOT(onDimModeTogggle(bool)));
    connect(keepDistToggle, SIGNAL(toggled(bool)), this, SLOT(onKeepDistTogggle(bool)));

    connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaVScroll(int)));
    connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaHScroll(int)));
    connect(dScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewDepthScroll(int)));
    connect(&bbsUser, SIGNAL(bbsMessage(BBSMessage)), this, SLOT(onBBSMessage(BBSMessage)));

    setArea(10000, 10000);
    setValue(500, 500);
}

void SceneWin::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    adjustPageStep();
}

void SceneWin::adjustPageStep()
{
    hScrollBar->setPageStep(width()/2);
    vScrollBar->setPageStep(height()/2);
}

void SceneWin::onDimModeTogggle(bool checked)
{

}

void SceneWin::onViewTypeToggle(bool checked)
{

}

void SceneWin::onKeepDistToggle(bool checked)
{

}

void SceneWin::onViewAreaVScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewAreaHScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewDepthScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewZoomIn()
{

}

void SceneWin::onViewZoomOut()
{

}

void SceneWin::onTurnClock()
{

}

void SceneWin::onTurnAClock()
{

}

void SceneWin::onForward()
{

}

void SceneWin::onBackward()
{

}

void SceneWin::onCameraUp()
{

}

void SceneWin::onCameraDown()
{

}

void SceneWin::onCameraMid()
{

}

void SceneWin::onZoomInDepth()
{

}

void SceneWin::onZoomOutDepth()
{

}


void SceneWin::setArea(int w, int h)
{
    hScrollBar->setMinimum(0);
    hScrollBar->setMaximum(w);
    vScrollBar->setMinimum(0);
    vScrollBar->setMaximum(h);
}

void SceneWin::setValue(int x, int y)
{
    hScrollBar->setValue(x);
    vScrollBar->setValue(y);
}

int SceneWin::onBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == &bbsUser)
        return 0;
    return 1;
}
