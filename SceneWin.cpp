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

SceneWin::SceneWin(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->setRowStretch(0, 100);
    layout->setColumnStretch(0, 100);
    layout->setColumnMinimumWidth(0, 300);

    setLayout(layout);

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
    vScrollBar = new QScrollBar(Qt::Vertical, this);
    vScrollBar->setToolTip("视区往相机前方/上方移动");
    vScrollBar->setMaximumWidth(maxSz);
    vScrollBar->setMinimum(0);
    vScrollBar->setMaximum(1000);
    vScrollBar->setPageStep(50);

    dScrollBar = new QScrollBar(Qt::Vertical, this);
    dScrollBar->setMaximumWidth(maxSz);
    dScrollBar->setToolTip("深度切面上下移动");
    dScrollBar->setMinimum(0);
    dScrollBar->setMaximum(1000);
    dScrollBar->setPageStep(50);

    zoomDepthScrollBar = new QScrollBar(Qt::Vertical, this);
    zoomDepthScrollBar->setMaximumWidth(maxSz);
    zoomDepthScrollBar->setToolTip("深度放大系数\n100%..1000%");
    zoomDepthScrollBar->setMinimum(100);
    zoomDepthScrollBar->setMaximum(1000);
    zoomDepthScrollBar->setPageStep(50);

    turnScrollBar = new QScrollBarV(Qt::Vertical, this);
    turnScrollBar->fmt = "旋转%d°";
    turnScrollBar->setMaximumWidth(maxSz);
    turnScrollBar->setToolTip("旋转视窗\n0(北)..359");
    turnScrollBar->setMinimum(0);
    turnScrollBar->setMaximum(359);
    turnScrollBar->setStatusTip("159");

    hScrollBar =new QScrollBar(Qt::Horizontal, this);
    hScrollBar->setToolTip("视区往相机侧方/水平方向移动");
    hScrollBar->setMaximumHeight(maxSz);
    hScrollBar->setMinimum(0);
    hScrollBar->setMaximum(1000);
    hScrollBar->setPageStep(50);

    zoomViewScrollBar = new QScrollBar(Qt::Horizontal, this);
    zoomViewScrollBar->setMaximumHeight(maxSz);
    zoomViewScrollBar->setToolTip("视窗放大系数\n1000(全部)..1");
    zoomViewScrollBar->setMinimum(1);
    zoomViewScrollBar->setMaximum(1000);

    rScrollBar = new QScrollBar(Qt::Horizontal, this);
    rScrollBar->setMaximumHeight(maxSz);
    rScrollBar->setToolTip("纵切面前后移动");
    rScrollBar->setMinimum(0);
    rScrollBar->setMaximum(1000);
    rScrollBar->setPageStep(50);

    headUpScrollBar = new QScrollBarV(Qt::Horizontal, this);
    headUpScrollBar->fmt = "俯仰%d°";
    headUpScrollBar->setMaximumHeight(maxSz);
    headUpScrollBar->setToolTip("相机抬头角度\n 范围：-45..45");
    headUpScrollBar->setMinimum(-45);
    headUpScrollBar->setMaximum(45);
    headUpScrollBar->setPageStep(5);

    scene = new Scene(this);

    info1 = new QLineEdit(this);
    info1->setMinimumWidth(200);
    info1->setText("坐标数据");
    info2 = new QLineEdit(this);
    info2->setText("相机参数");

    // layout
    QHBoxLayout * layoutH1 = new QHBoxLayout();
    QHBoxLayout * layoutH2 = new QHBoxLayout();
    QHBoxLayout * layoutHButtons = new QHBoxLayout();
    QVBoxLayout * layoutV1 = new QVBoxLayout();
    QVBoxLayout * layoutV2 = new QVBoxLayout();
    QVBoxLayout * layoutVInfo = new QVBoxLayout();
    QHBoxLayout * layoutB = new QHBoxLayout();
    QSplitter * spliterH = new QSplitter(Qt::Orientation::Vertical);
    QSplitter * spliterV = new QSplitter(Qt::Orientation::Horizontal);
    QFrame * frameH1 = new QFrame();
    QFrame * frameH2 = new QFrame();
    QFrame * frameV1 = new QFrame();
    QFrame * frameV2 = new QFrame();

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
    frameV1->setStyleSheet(scrollStyle);
    frameV2->setStyleSheet(scrollStyle);

    int layoutMargin = 3;
    layoutH1->setMargin(layoutMargin);
    layoutH2->setMargin(layoutMargin);
    layoutV1->setMargin(layoutMargin);
    layoutV2->setMargin(layoutMargin);

    int layoutSpace = 2;
    layoutH1->setSpacing(layoutSpace);
    layoutH2->setSpacing(layoutSpace);
    layoutV1->setSpacing(layoutSpace);
    layoutV2->setSpacing(layoutSpace);

    layoutB->setStretch(0, 50);
    layoutB->setStretch(1, 90);

    // do layout
    int scrollSpace = 2;
    layoutH1->addWidget(vScrollBar);
    layoutH1->addSpacing(scrollSpace);
    layoutH1->addWidget(dScrollBar);

    layoutH2->addWidget(zoomDepthScrollBar);
    layoutH2->addSpacing(scrollSpace);
    layoutH2->addWidget(turnScrollBar);

    frameH1->setLayout(layoutH1);
    frameH2->setLayout(layoutH2);

    spliterH->addWidget(frameH1);
    spliterH->addWidget(frameH2);
    spliterH->setStretchFactor(0, 70);
    spliterH->setStretchFactor(1, 30);

    layoutV1->addWidget(hScrollBar);
    layoutV1->addSpacing(scrollSpace);
    layoutV1->addWidget(zoomViewScrollBar);

    layoutV2->addWidget(headUpScrollBar);
    layoutV2->addSpacing(scrollSpace);
    layoutV2->addWidget(rScrollBar);

    frameV1->setLayout(layoutV1);
    frameV2->setLayout(layoutV2);

    spliterV->addWidget(frameV1);
    spliterV->addWidget(frameV2);
    spliterV->setStretchFactor(0, 70);
    spliterV->setStretchFactor(1, 30);

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
