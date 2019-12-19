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

SceneWin::SceneWin(QWidget *parent)
    : QWidget(parent), BBSMessageProc()
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->setRowStretch(0, 100);
    layout->setColumnStretch(0, 100);
    layout->setColumnMinimumWidth(0, 300);

    setLayout(layout);

    keepDist = true;

    bbsUser.init(this);

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
    conerButton->setToolTip("回到对应视图的缺省状态");

    cameraMidBut = new QPushButton("=", this);
    cameraMidBut->setToolTip("相机俯仰角归零");

    someTestBut = new QPushButton("Test", this);
    someTestBut->setToolTip("Do Some Test");

    int maxSz = 35;
    int minSz = 35;
    setButtonMinMaxIcon(conerButton, minSz, maxSz);
    setButtonMinMaxIcon(cameraMidBut, minSz, maxSz);

    setCheckBoxMinMaxIcon(viewTypeToggle, minSz, maxSz, "ZViewType");
    setCheckBoxMinMaxIcon(dimModeToggle, minSz, maxSz, "ZDimension");
    setCheckBoxMinMaxIcon(keepDistToggle, minSz, maxSz, "ZKeepDist");

    maxSz = 18;
    vScrollBar = new ScrollBarV(Qt::Vertical, "vScrollBar", this);
    vScrollBar->setMaximumWidth(maxSz);
    vScrollBar->setMinimum(0);
    vScrollBar->setMaximum(1000);
    vScrollBar->setPageStep(50);

    dScrollBar = new ScrollBarV(Qt::Vertical, "dScrollBar", this);
    dScrollBar->setMaximumWidth(maxSz);
    dScrollBar->setMinimum(0);
    dScrollBar->setMaximum(1000);
    dScrollBar->setPageStep(50);

    zoomDepthScrollBar = new ScrollBarV(Qt::Vertical, "zoomDepth", this);
    zoomDepthScrollBar->setMaximumWidth(maxSz);
    zoomDepthScrollBar->fmt = "深度放大%d%%";
    zoomDepthScrollBar->setToolTip("深度放大系数\n100%..1000%");
    zoomDepthScrollBar->setMinimum(100);
    zoomDepthScrollBar->setMaximum(1000);
    zoomDepthScrollBar->setPageStep(100);

    turnScrollBar = new ScrollBarV(Qt::Vertical, "turn", this);
    turnScrollBar->fmt = "旋转%d°";
    turnScrollBar->setMaximumWidth(maxSz);
    turnScrollBar->setToolTip("旋转视窗\n0(北)..359");
    turnScrollBar->setMinimum(0);
    turnScrollBar->setMaximum(359);
    turnScrollBar->setPageStep(10);

    forwardScrollBar = new ScrollBarV(Qt::Vertical, "forward", this);
    forwardScrollBar->setMaximumWidth(maxSz);
    forwardScrollBar->fmt = "相机位置%d";

    hScrollBar =new ScrollBarV(Qt::Horizontal, "hScrollBar", this);
    hScrollBar->setMaximumHeight(maxSz);
    hScrollBar->setMinimum(0);
    hScrollBar->setMaximum(1000);
    hScrollBar->setPageStep(50);

    zoomViewScrollBar = new ScrollBarV(Qt::Horizontal, "zoomView", this);
    zoomViewScrollBar->setMaximumHeight(maxSz);
    zoomViewScrollBar->setToolTip("视窗放大系数\n1..100(全部)");
    zoomViewScrollBar->setMinimum(1);
    zoomViewScrollBar->setMaximum(100);
    zoomViewScrollBar->fmt = "视区占%d%%";

    cutRadiusScrollBar = new ScrollBarV(Qt::Horizontal, "cutRadius", this);
    cutRadiusScrollBar->fmt = "纵切面位置%d";
    cutRadiusScrollBar->setMaximumHeight(maxSz);
    cutRadiusScrollBar->setToolTip("纵切面前后移动");
    cutRadiusScrollBar->setMinimum(-1000);
    cutRadiusScrollBar->setMaximum(1000);
    cutRadiusScrollBar->setPageStep(100);

    cutAngleScrollBar = new ScrollBarV(Qt::Vertical, "cutAngle", this);
    cutAngleScrollBar->fmt = "纵切角%d°";
    cutAngleScrollBar->setMaximumWidth(maxSz);
    cutAngleScrollBar->setToolTip("纵切角度旋转");
    cutAngleScrollBar->setMinimum(0);
    cutAngleScrollBar->setMaximum(359);
    cutAngleScrollBar->setPageStep(10);

    headUpScrollBar = new ScrollBarV(Qt::Horizontal, "headUp", this);
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
    QHBoxLayout * layoutH3 = new QHBoxLayout();
    QHBoxLayout * layoutHButtons = new QHBoxLayout();
    QVBoxLayout * layoutV1 = new QVBoxLayout();
    QVBoxLayout * layoutV2 = new QVBoxLayout();
    QVBoxLayout * layoutVInfo = new QVBoxLayout();
    QHBoxLayout * layoutB = new QHBoxLayout();
    QSplitter * spliterH = new QSplitter(Qt::Orientation::Vertical);
    QSplitter * spliterV = new QSplitter(Qt::Orientation::Horizontal);
    QFrame * frameH1 = new QFrame();
    QFrame * frameH2 = new QFrame();
    QFrame * frameH3 = new QFrame();
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
    frameH3->setStyleSheet(scrollStyle);
    frameV1->setStyleSheet(scrollStyle);
    frameV2->setStyleSheet(scrollStyle);

    int layoutMargin = 3;
    layoutH1->setMargin(layoutMargin);
    layoutH2->setMargin(layoutMargin);
    layoutH3->setMargin(layoutMargin);
    layoutV1->setMargin(layoutMargin);
    layoutV2->setMargin(layoutMargin);

    int layoutSpace = 2;
    layoutH1->setSpacing(layoutSpace);
    layoutH2->setSpacing(layoutSpace);
    layoutH3->setSpacing(layoutSpace);
    layoutV1->setSpacing(layoutSpace);
    layoutV2->setSpacing(layoutSpace);

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

    layoutH3->addWidget(forwardScrollBar);
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

    frameV1->setLayout(layoutV1);
    frameV2->setLayout(layoutV2);

    spliterV->addWidget(frameV1);
    spliterV->addWidget(frameV2);
    spliterV->setStretchFactor(0, 50);
    spliterV->setStretchFactor(1, 30);
    spliterV->setStretchFactor(2, 30);

    layoutVInfo->addWidget(info1);
    layoutVInfo->addWidget(info2);

    layoutHButtons->setMargin(1);
    layoutHButtons->addWidget(cameraMidBut);
    layoutHButtons->addWidget(viewTypeToggle);
    layoutHButtons->addWidget(dimModeToggle);
    layoutHButtons->addWidget(someTestBut);
    layoutHButtons->addWidget(conerButton);

    layoutB->addLayout(layoutVInfo);
    layoutB->addWidget(spliterV);
    layoutB->addLayout(layoutHButtons);

    layout->addWidget(scene, 0, 0);
    layout->addWidget(spliterH, 0,1);
    layout->addLayout(layoutB, 1,0,1,2);

    moveBottom = new ShapeButton("ZButDown", 32, 32, this);
    moveRight = new ShapeButton("ZButRight", 32, 32, this);
    moveUp = new ShapeButton("ZButUp", 32, 32, this);
    moveLeft = new ShapeButton("ZButLeft", 32, 32, this);
    moveBottom->show();
    moveRight->show();
    moveUp->show();
    moveLeft->show();

    rotatePanel = new RotatePanel(100,100, this);

    scene->show();

    connect(moveLeft, SIGNAL(pressed()), this, SLOT(onArrowMove()));
    connect(moveRight, SIGNAL(pressed()), this, SLOT(onArrowMove()));
    connect(moveUp, SIGNAL(pressed()), this, SLOT(onArrowMove()));
    connect(moveBottom, SIGNAL(pressed()), this, SLOT(onArrowMove()));

    connect(conerButton, SIGNAL(pressed()), this, SLOT(onConor()));

    connect(viewTypeToggle, SIGNAL(toggled(bool)), this, SLOT(onViewTypeTogle(bool)));
    connect(dimModeToggle, SIGNAL(toggled(bool)), this, SLOT(onDimModeTogggle(bool)));
    connect(keepDistToggle, SIGNAL(toggled(bool)), this, SLOT(onKeepDistToggle(bool)));
    connect(cameraMidBut, SIGNAL(pressed()), this, SLOT(onCameraPitchMid()));

    connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaVScroll(int)));
    connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewAreaHScroll(int)));
    connect(dScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onViewDepthScroll(int)));

    connect(cutAngleScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCutAngleScroll(int)));
    connect(cutRadiusScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCutRadiusScroll(int)));
    connect(forwardScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCameraForwardScroll(int)));

    connect(headUpScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onHeadUpScroll(int)));
    connect(zoomDepthScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onZoomDepthScroll(int)));
    connect(zoomViewScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onZoomViewScroll(int)));
    connect(turnScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onCameraTurnScroll(int)));

    setArea(10000, 10000);
    setValue(500, 500);
}

void SceneWin::updateFieldRange()
{
    vScrollBar->setMinimum(-G.fieldRange.aboveField);
    vScrollBar->setMaximum(G.field.depth + G.fieldRange.belowField);
    vScrollBar->setPageStep((vScrollBar->maximum() - vScrollBar->minimum())/10);

    hScrollBar->setMinimum(-G.field.radius() -G.fieldRange.outRadius);
    hScrollBar->setMaximum(G.field.radius() +G.fieldRange.outRadius);
    hScrollBar->setPageStep((hScrollBar->maximum() - hScrollBar->minimum())/20);

    dScrollBar->setMinimum(0);
    dScrollBar->setMaximum(G.field.depth);
    dScrollBar->setPageStep((dScrollBar->maximum() - dScrollBar->minimum())/10);

    cutRadiusScrollBar->setMinimum(-G.field.radius());
    cutRadiusScrollBar->setMaximum(G.field.radius());
    cutRadiusScrollBar->setPageStep((cutRadiusScrollBar->maximum() - cutRadiusScrollBar->minimum())/20);
}

void SceneWin::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rotatePanel->move(2, scene->height()-rotatePanel->height()-2);
    moveBottom->move(scene->width()/2-moveBottom->width()/2,
            scene->height()-moveBottom->height()-2);
    moveUp->move(scene->width()/2-moveUp->width()/2, 2);
    moveLeft->move(2, scene->height()/2-moveLeft->height()/2);
    moveRight->move(scene->width()-moveRight->width(),
            scene->height()/2-moveRight->height()/2);
}

void SceneWin::updateControlToolTip()
{
    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        vScrollBar->setToolTip("视区往相机前后方移动");
        hScrollBar->setToolTip("视区往相机侧方向移动");
        dScrollBar->setToolTip("深度切面上下移动");
        forwardScrollBar->setToolTip("俯视图相机上下移动");
        vScrollBar->fmt = "视窗前后位置%d";
        hScrollBar->fmt = "视窗左右位置%d";
        dScrollBar->fmt = "深度切面%d";

        int camOff = G.field.depth*0.1;
        forwardScrollBar->setMinimum(-camOff);
        forwardScrollBar->setMaximum(camOff * 11);
        forwardScrollBar->setPageStep(camOff/10);
    }
    // EVT_Side
    else if (G.viewPot.viewType == EVT_Side)
    {
        vScrollBar->setToolTip("视区上下方向移动");
        hScrollBar->setToolTip("视区水平方向移动");

        dScrollBar->setToolTip("深度切面上下移动");
        forwardScrollBar->setToolTip("相机前后移动");
        vScrollBar->fmt = "视窗上下位置%d";
        hScrollBar->fmt = "视窗水平位置%d";
        dScrollBar->fmt = "深度切面%d";

        int camOff = G.fieldRange.radius*1.1;
        forwardScrollBar->setMinimum(-camOff);
        forwardScrollBar->setMaximum(camOff);
        forwardScrollBar->setPageStep(G.fieldRange.radius/100);
    }
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
    updateControlToolTip();
}

void SceneWin::onKeepDistToggle(bool checked)
{
    if (checked)
        keepDist = false;
    else
        keepDist = true;
}

void SceneWin::onArrowMove()
{
    QPushButton * button = qobject_cast<QPushButton*>(sender());
    ScrollBarV * scroll = nullptr;
    int delta = 0;

    if (button ==  moveLeft)
    {
        scroll = hScrollBar;
        delta = -1;
    }
    else if (button == moveRight)
    {
        scroll = hScrollBar;
        delta = 1;
    }
    else if (button == moveUp)
    {
        scroll = vScrollBar;
        delta = -1;
    }
    else if (button == moveBottom)
    {
        scroll = vScrollBar;
        delta = 1;
    }
    else
        return;

    int v = scroll->value() + delta*scroll->pageStep();
    if (v < scroll->minimum())
        v = scroll->minimum();
    if (v > scroll->maximum())
        v = scroll->maximum();
    scroll->setValue(v);
    qDebug() << "onArrowMove scroll=" << scroll->fmt << v;
}

void SceneWin::onConor()
{
    if (G.viewPot.viewType == EVT_Down)
    {
        G.viewPot.zoomDepth = 1.0;
        G.viewPot.cameraPar.x = 0;
        G.viewPot.cameraPar.y = G.field.depth * 0.1;
        G.viewPot.cameraPar.z = 0;
        G.viewPot.cameraPar.yaw = 0;
        G.viewPot.cameraPar.roll = 0;
        G.viewPot.cameraPar.pitch = 0;
    }
    else
    {

    }
}

void SceneWin::onSomeTest()
{
    updateFromCameraPos();
    updateFromCutField();

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

    int t = turnScrollBar->value();
    int camDist = forwardScrollBar->value();
    int head = headUpScrollBar->value();

    int zoomView = zoomViewScrollBar->value();
    double zoomD = zoomDepthScrollBar->value() / 100.0;

    CameraPar & gCameraPar = G.viewPot.cameraPar;
    FieldCut & gFieldCut = G.viewPot.fieldCut;

    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        gCameraPar.x = h*cos(t*D2R) + v*sin(t*D2R);
        gCameraPar.z = -h*sin(t*D2R) + v*cos(t*D2R);
        gCameraPar.y = - camDist;

        gCameraPar.roll = t;
        gCameraPar.pitch = -90 + head;
        gCameraPar.yaw = 0.0;

        gCameraPar.viewW = G.fieldRange.radius * zoomView / 100;
        G.viewPot.zoomDepth = 0.01 * zoomD;
    }
    // EVT_Side
    else
    {
        double t1 = 90 - t;
        gCameraPar.y = -v;
        gCameraPar.x = camDist*cos(t1*D2R) + h*sin(t1*D2R);
        gCameraPar.z = h*cos(t1*D2R) - camDist*sin(t1*D2R);

        gCameraPar.pitch = head;
        gCameraPar.yaw = t;
        gCameraPar.roll = 0;

        gCameraPar.viewW = G.fieldRange.radius * zoomView / 100;
        G.viewPot.zoomDepth = 0.01 * zoomD;
    }
    QString camS;
    camS.sprintf("XYZ=%4.1lf %4.1lf %4.1lf "
            "RPY=%4.1lf %4.1lf %4.1lf WD=%4.1lf %4.1lf",
            gCameraPar.x, gCameraPar.y, gCameraPar.z,
            gCameraPar.roll, gCameraPar.pitch, gCameraPar.yaw,
            gCameraPar.viewW, G.viewPot.zoomDepth);
    info1->setText(camS);
}

void SceneWin::updateFromCameraPos()
{
    CameraPar & gCamPar = G.viewPot.cameraPar;

    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        double t = gCamPar.roll;
        double v = gCamPar.x*sin(t*D2R) + gCamPar.z*cos(t*D2R);
        double h = gCamPar.x*cos(t*D2R) - gCamPar.z*sin(t*D2R);
        vScrollBar->setSliderPos(v);
        hScrollBar->setSliderPos(h);
        turnScrollBar->setSliderPos(gCamPar.roll);
        forwardScrollBar->setSliderPos(-gCamPar.y);
        headUpScrollBar->setSliderPos(gCamPar.pitch + 90);
    }
    // EVT_Side
    else
    {
        vScrollBar->setSliderPos(-gCamPar.y);
        turnScrollBar->setSliderPos(gCamPar.yaw);
        headUpScrollBar->setSliderPos(gCamPar.pitch);

        double t1 = 90 - gCamPar.yaw;
        double comDist = gCamPar.x*cos(t1*D2R) -gCamPar.z*sin(t1*D2R);
        double h = gCamPar.x*sin(t1*D2R) +gCamPar.z*cos(t1*D2R);
        hScrollBar->setSliderPos(h);
        forwardScrollBar->setSliderPos(comDist);
        gCamPar.roll = 0;
    }
    zoomDepthScrollBar->setSliderPos(G.viewPot.zoomDepth * 100);
    zoomViewScrollBar->setSliderPos(gCamPar.viewW * 100
            / G.fieldRange.radius);
}

void SceneWin::calculateCutFileld()
{
    int cutR = cutRadiusScrollBar->value();
    int cutA = cutAngleScrollBar->value();
    int d = dScrollBar->value();

    FieldCut & gCutField = G.viewPot.fieldCut;

    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        gCutField.cutDepth = d;
        gCutField.vCutAngle = cutA;
        gCutField.vCutPoint.y = 0.0;
        gCutField.vCutPoint.x = cutR*cos(cutA*D2R);
        gCutField.vCutPoint.z = cutR*sin(cutA*D2R);
    }
    // EVT_Side
    else
    {
        gCutField.cutDepth = d;
        gCutField.vCutAngle = cutA;
        gCutField.vCutPoint.y = 0.0;
        gCutField.vCutPoint.x = cutR*cos(cutA*D2R);
        gCutField.vCutPoint.z = cutR*sin(cutA*D2R);
    }
    QString cutS;
    cutS.sprintf("D=%4.1lf A=%4.1lf XYZ=%4.1lf %4.1lf %4.1lf",
            gCutField.cutDepth, gCutField.vCutAngle,
            gCutField.vCutPoint.x, gCutField.vCutPoint.y,
            gCutField.vCutPoint.z);
    info2->setText(cutS);
}

void SceneWin::updateFromCutField()
{
    FieldCut & gCutField = G.viewPot.fieldCut;

    if (G.viewPot.viewType == EVT_Down)
    {
        double cutA = gCutField.vCutAngle;
        double x = gCutField.vCutPoint.x;
        double z = gCutField.vCutPoint.z;
        cutRadiusScrollBar->setSliderPos(sqrt(x*x + z*z));
        cutAngleScrollBar->setSliderPos(cutA);
        dScrollBar->setSliderPos(gCutField.cutDepth);
    }
    // EVT_Side
    else
    {
        double cutA = gCutField.vCutAngle;
        double x = gCutField.vCutPoint.x;
        double z = gCutField.vCutPoint.z;
        cutRadiusScrollBar->setSliderPos(sqrt(x*x + z*z));
        cutAngleScrollBar->setSliderPos(cutA);
        dScrollBar->setSliderPos(gCutField.cutDepth);
    }
}

void SceneWin::onViewAreaVScroll(int v)
{
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
    qDebug() << "onViewAreaVScroll=";
}

void SceneWin::onViewAreaHScroll(int v)
{
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onZoomDepthScroll(int v)
{
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCameraTurnScroll(int v)
{
    qDebug() << "onCameraTurnScroll";
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCameraForwardScroll(int v)
{
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewDepthScroll(int v)
{
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onZoomViewScroll(int v)
{
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCutRadiusScroll(int v)
{
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCutAngleScroll(int v)
{
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onHeadUpScroll(int v)
{
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_HCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::procBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == &bbsUser)
        return;
    if (bbsMsg.varity == EBV_Field)
        updateFieldRange();
    if (bbsMsg.varity == EBV_Camera)
        updateFromCameraPos();
    if (bbsMsg.varity == EBV_VCut)
        updateFromCutField();
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
