#include "SceneWin.h"
#include "Global.h"

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

SceneWin::SceneWin(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->setRowStretch(0, 100);
    layout->setColumnStretch(0, 100);
    layout->setColumnMinimumWidth(0, 300);

    setLayout(layout);

    keepDist = true;

    viewTypeToggle = new QPushButton(this);
    viewTypeToggle->setToolTip("切换俯视图/侧视图模式");
    dimModeToggle = new QPushButton(this);
    dimModeToggle->setToolTip("切换视区2D/3D模式");
    keepDistToggle = new QPushButton(this);
    keepDistToggle->setToolTip("保持相机与切面距离");

    conerButton = new QPushButton("*", this);
    conerButton->setStyleSheet(
        //"QPushButton{color: white;font: bold 10pt;border:none;"
        //"min-height: 24px;min-width: 60px;"
        //"background:url(:images/SimpleButtom.png) -60px 0px no-repeat;}");
        "border-image:url(:/Start.png);}");

    cameraMidBut = new QPushButton("=", this);
    cameraMidBut->setToolTip("相机俯仰角归零");

    int maxSz = 35;
    int minSz = 35;
    setButtonMinMaxIcon(conerButton, minSz, maxSz);
    setButtonMinMaxIcon(cameraMidBut, minSz, maxSz);

    setCheckBoxMinMaxIcon(viewTypeToggle, minSz, maxSz, "ZViewType");
    setCheckBoxMinMaxIcon(dimModeToggle, minSz, maxSz, "ZDimension");
    setCheckBoxMinMaxIcon(keepDistToggle, minSz, maxSz, "ZKeepDist");

    maxSz = 18;
    vScrollBar = new ScrollBarV(Qt::Vertical, this);
    vScrollBar->setMaximumWidth(maxSz);
    vScrollBar->setMinimum(0);
    vScrollBar->setMaximum(1000);
    vScrollBar->setPageStep(50);

    dScrollBar = new ScrollBarV(Qt::Vertical, this);
    dScrollBar->setMaximumWidth(maxSz);
    dScrollBar->setMinimum(0);
    dScrollBar->setMaximum(1000);
    dScrollBar->setPageStep(50);

    zoomDepthScrollBar = new ScrollBarV(Qt::Vertical, this);
    zoomDepthScrollBar->setMaximumWidth(maxSz);
    zoomDepthScrollBar->fmt = "深度放大%d%%";
    zoomDepthScrollBar->setToolTip("深度放大系数\n100%..5000%");
    zoomDepthScrollBar->setMinimum(100);
    zoomDepthScrollBar->setMaximum(5000);
    zoomDepthScrollBar->setPageStep(100);

    turnScrollBar = new ScrollBarV(Qt::Vertical, this);
    turnScrollBar->fmt = "旋转%d°";
    turnScrollBar->setMaximumWidth(maxSz);
    turnScrollBar->setToolTip("旋转视窗\n0(北)..359");
    turnScrollBar->setMinimum(0);
    turnScrollBar->setMaximum(359);
    turnScrollBar->setStatusTip("159");

    cameraVScrollBar = new ScrollBarV(Qt::Vertical, this);
    cameraVScrollBar->setMaximumWidth(maxSz);
    cameraVScrollBar->setMinimum(0);
    cameraVScrollBar->setMaximum(1000);
    cameraVScrollBar->setPageStep(50);

    hScrollBar =new ScrollBarV(Qt::Horizontal, this);
    hScrollBar->setMaximumHeight(maxSz);
    hScrollBar->setMinimum(0);
    hScrollBar->setMaximum(1000);
    hScrollBar->setPageStep(50);

    zoomViewScrollBar = new ScrollBarV(Qt::Horizontal, this);
    zoomViewScrollBar->setMaximumHeight(maxSz);
    zoomViewScrollBar->setToolTip("视窗放大系数\n1..100(全部)");
    zoomViewScrollBar->setMinimum(1);
    zoomViewScrollBar->setMaximum(100);
    zoomViewScrollBar->fmt = "视区占%d%%";

    cutRadiusScrollBar = new ScrollBarV(Qt::Horizontal, this);
    cutRadiusScrollBar->setMaximumHeight(maxSz);
    cutRadiusScrollBar->setToolTip("纵切面前后移动");
    cutRadiusScrollBar->setMinimum(0);
    cutRadiusScrollBar->setMaximum(1000);
    cutRadiusScrollBar->setPageStep(50);

    cutAngleScrollBar = new ScrollBarV(Qt::Vertical, this);
    cutAngleScrollBar->fmt = "纵切角%d°";
    cutAngleScrollBar->setMaximumWidth(maxSz);
    cutAngleScrollBar->setToolTip("纵切角度旋转");
    cutAngleScrollBar->setMinimum(0);
    cutAngleScrollBar->setMaximum(359);
    cutAngleScrollBar->setPageStep(10);

    headUpScrollBar = new ScrollBarV(Qt::Horizontal, this);
    headUpScrollBar->fmt = "俯仰%d°";
    headUpScrollBar->setMaximumHeight(maxSz);
    headUpScrollBar->setToolTip("相机抬头角度\n 范围：-45..45");
    headUpScrollBar->setMinimum(-45);
    headUpScrollBar->setMaximum(45);
    headUpScrollBar->setPageStep(5);

    cameraHScrollBar = new ScrollBarV(Qt::Horizontal, this);
    cameraHScrollBar->setToolTip("侧视图相机前后移动");
    cameraHScrollBar->setMaximumHeight(maxSz);
    cameraHScrollBar->setMinimum(0);
    cameraHScrollBar->setMaximum(1000);
    cameraHScrollBar->setPageStep(50);

    scene = new Scene(this);

    info1 = new QLineEdit(this);
    info1->setMinimumWidth(200);
    info1->setText("坐标数据");
    info2 = new QLineEdit(this);
    info2->setText("相机参数");

    // layout
    QHBoxLayout * layoutH1 = new QHBoxLayout();
    QHBoxLayout * layoutH2 = new QHBoxLayout();
    QHBoxLayout * layoutH3 = new QHBoxLayout();
    QHBoxLayout * layoutHButtons = new QHBoxLayout();
    QVBoxLayout * layoutV1 = new QVBoxLayout();
    QVBoxLayout * layoutV2 = new QVBoxLayout();
    QVBoxLayout * layoutV3 = new QVBoxLayout();
    QVBoxLayout * layoutVInfo = new QVBoxLayout();
    QHBoxLayout * layoutB = new QHBoxLayout();
    QSplitter * spliterH = new QSplitter(Qt::Orientation::Vertical);
    QSplitter * spliterV = new QSplitter(Qt::Orientation::Horizontal);
    QFrame * frameH1 = new QFrame();
    QFrame * frameH2 = new QFrame();
    QFrame * frameH3 = new QFrame();
    QFrame * frameV1 = new QFrame();
    QFrame * frameV2 = new QFrame();
    QFrame * frameV3 = new QFrame();

    // layout's style
    spliterH->setOpaqueResize(false);
    spliterV->setOpaqueResize(false);
    spliterH->setHandleWidth(5);
    spliterV->setHandleWidth(5);
    QString splitterStyle = "QSplitter::handle{background:rgb(60,0,140)}";
    spliterH->setStyleSheet(splitterStyle);
    spliterV->setStyleSheet(splitterStyle);

    QString scrollStyle = "QFrame{background-color:rgb(55,95,55);}";
    frameH1->setStyleSheet(scrollStyle);
    frameH2->setStyleSheet(scrollStyle);
    frameH3->setStyleSheet(scrollStyle);
    frameV1->setStyleSheet(scrollStyle);
    frameV2->setStyleSheet(scrollStyle);
    frameV3->setStyleSheet(scrollStyle);

    int layoutMargin = 3;
    layoutH1->setMargin(layoutMargin);
    layoutH2->setMargin(layoutMargin);
    layoutH3->setMargin(layoutMargin);
    layoutV1->setMargin(layoutMargin);
    layoutV2->setMargin(layoutMargin);
    layoutV3->setMargin(layoutMargin);

    int layoutSpace = 2;
    layoutH1->setSpacing(layoutSpace);
    layoutH2->setSpacing(layoutSpace);
    layoutH3->setSpacing(layoutSpace);
    layoutV1->setSpacing(layoutSpace);
    layoutV2->setSpacing(layoutSpace);
    layoutV3->setSpacing(layoutSpace);

    layoutB->setStretch(0, 50);
    layoutB->setStretch(1, 70);
    layoutB->setStretch(2, 70);

    // do layout
    int scrollSpace = 2;
    layoutH1->addWidget(vScrollBar);
    layoutH1->addSpacing(scrollSpace);
    layoutH1->addWidget(dScrollBar);

    layoutH2->addWidget(zoomDepthScrollBar);
    layoutH2->addSpacing(scrollSpace);
    layoutH2->addWidget(turnScrollBar);

    layoutH3->addWidget(cameraVScrollBar);
    layoutH3->addSpacing(scrollSpace);
    layoutH3->addWidget(cutAngleScrollBar);

    frameH1->setLayout(layoutH1);
    frameH2->setLayout(layoutH2);
    frameH3->setLayout(layoutH3);

    spliterH->addWidget(frameH1);
    spliterH->addWidget(frameH2);
    spliterH->addWidget(frameH3);
    spliterH->setStretchFactor(0, 40);
    spliterH->setStretchFactor(1, 30);
    spliterH->setStretchFactor(2, 30);

    layoutV1->addWidget(hScrollBar);
    layoutV1->addSpacing(scrollSpace);
    layoutV1->addWidget(cutRadiusScrollBar);

    layoutV2->addWidget(headUpScrollBar);
    layoutV2->addSpacing(scrollSpace);
    layoutV2->addWidget(zoomViewScrollBar);

    layoutV3->addWidget(cameraHScrollBar);
    layoutV3->addSpacing(scrollSpace);
    layoutV3->addStretch();

    frameV1->setLayout(layoutV1);
    frameV2->setLayout(layoutV2);
    frameV3->setLayout(layoutV3);

    spliterV->addWidget(frameV1);
    spliterV->addWidget(frameV2);
    spliterV->addWidget(frameV3);
    spliterV->setStretchFactor(0, 50);
    spliterV->setStretchFactor(1, 30);
    spliterV->setStretchFactor(2, 30);

    layoutVInfo->addWidget(info1);
    layoutVInfo->addWidget(info2);

    layoutHButtons->setMargin(1);
    layoutHButtons->addWidget(cameraMidBut);
    layoutHButtons->addWidget(viewTypeToggle);
    layoutHButtons->addWidget(dimModeToggle);
    layoutHButtons->addWidget(conerButton);

    layoutB->addLayout(layoutVInfo);
    layoutB->addWidget(spliterV);
    layoutB->addLayout(layoutHButtons);

    layout->addWidget(scene, 0, 0);
    layout->addWidget(spliterH, 0,1);
    layout->addLayout(layoutB, 1,0,1,2);

    updateScrollToolTip();

    scene->show();

    connect(viewTypeToggle, SIGNAL(toggled(bool)), this, SLOT(onViewTypeTogle(bool)));
    connect(dimModeToggle, SIGNAL(toggled(bool)), this, SLOT(onDimModeTogggle(bool)));
    connect(keepDistToggle, SIGNAL(toggled(bool)), this, SLOT(onKeepDistToggle(bool)));
    connect(cameraMidBut, SIGNAL(pressed()), this, SLOT(onCameraPitchMid()));

    connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaVScroll(int)));
    connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaHScroll(int)));
    connect(dScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewDepthScroll(int)));

    connect(cutRadiusScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCutRadiusScroll(int)));
    connect(cameraVScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCameraVScroll(int)));
    connect(cameraHScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCameraHScroll(int)));

    connect(headUpScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onHeadUpScroll(int)));
    connect(zoomDepthScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onZoomDepthScroll(int)));
    connect(zoomViewScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onZoomViewScroll(int)));
    connect(turnScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCameraTurnScroll(int)));

    connect(&bbsUser, SIGNAL(bbsMessage(BBSMessage)), this, SLOT(onBBSMessage(BBSMessage)));

    setArea(10000, 10000);
    setValue(500, 500);
}

void SceneWin::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    adjustPageStep();
}

void SceneWin::updateScrollToolTip()
{
    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        vScrollBar->setToolTip("视区往相机前后方移动");
        hScrollBar->setToolTip("视区往相机侧方向移动");
        dScrollBar->setToolTip("深度切面上下移动");
        cameraVScrollBar->setToolTip("俯视图相机上下移动");
    }
    // EVT_Side
    else if (G.viewPot.viewType == EVT_Side)
    {
        vScrollBar->setToolTip("视区上下方向移动");
        hScrollBar->setToolTip("视区水平方向移动");

        dScrollBar->setToolTip("深度切面上下移动");
        cameraVScrollBar->setToolTip("本操作在侧视图无效");
    }
}

void SceneWin::updateFieldRange()
{
    double w = G.cutField.field.width();
    double h = G.cutField.field.height();
    double depth = G.cutField.field.depth;
}

void SceneWin::adjustPageStep()
{
    hScrollBar->setPageStep(width()/2);
    vScrollBar->setPageStep(height()/2);
}

void SceneWin::onDimModeTogggle(bool checked)
{
    if (checked)
        G.viewPot.dimMode = EDM_3D;
    else
        G.viewPot.dimMode = EDM_2D;
    BBSMessage msg(EBS_SceneWin, EBV_DimMode);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewTypeTogle(bool checked)
{
    if (checked)
        G.viewPot.viewType = EVT_Down;
    else
        G.viewPot.viewType = EVT_Side;
    BBSMessage msg(EBS_SceneWin, EBV_ViewType);
    bbsUser.sendBBSMessage(msg);
    updateScrollToolTip();
}

void SceneWin::onKeepDistToggle(bool checked)
{
    if (checked)
        keepDist = false;
    else
        keepDist = true;
}

void SceneWin::onCameraPitchMid()
{
    headUpScrollBar->setValue(
        (headUpScrollBar->minimum()+headUpScrollBar->maximum())/2);
}

void SceneWin::calculateCameraPos()
{
    int v = vScrollBar->value();
    int h = hScrollBar->value();
    int r = cutRadiusScrollBar->value();
    int turn = turnScrollBar->value();
    int camV = cameraVScrollBar->value();
    int camH = cameraHScrollBar->value();

    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        G.viewPot.cameraPar.x = v * sin(G.viewPot.cameraPar.roll*D2R);
        G.viewPot.cameraPar.z = v * cos(G.viewPot.cameraPar.roll*D2R);
        G.viewPot.cameraPar.y = camV;
    }
    // EVT_Side
    else
    {
        G.viewPot.cameraPar.y = v;
    }
}

void SceneWin::updateFromCameraPos()
{

}

void SceneWin::onViewAreaVScroll(int v)
{
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewAreaHScroll(int v)
{
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewDepthScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCameraVScroll(int v)
{

}

void SceneWin::onCameraHScroll(int v)
{

}

void SceneWin::onCutRadiusScroll(int v)
{

}

void SceneWin::onCutAngleScroll(int v)
{

}

void SceneWin::onHeadUpScroll(int v)
{

}
void SceneWin::onZoomDepthScroll(int v)
{

}

void SceneWin::onZoomViewScroll(int v)
{

}

void SceneWin::onCameraTurnScroll(int v)
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
