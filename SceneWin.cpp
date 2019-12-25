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
┌─────────────────────────────────────────────────────┐
│ Scene Caption: some static information              │
├───────────────────────────────────────────────┬──┬──┤
│                                               │  │  │
│                                               │H1│H1│
│                                               │-0│-1│
│                Scene                          │  │  │
│                                               ├──┼──┤
│                                               │  │  │
│                                               │H2│H2│                                                 │-0│  │
│                                               │-0│-1│
│                                               │  │  │
├──────────────┬──────────────┬─────┬─────┬─────┼──┴──┤
│      V1-0    │      V2-0    │     │     │     │     │
├──────────────┼──────────────┤H3-0 │H3-1 │H3-2 │  C  │
│      V1-1    │      V2-1    │     │     │     │     │
└──────────────┴──────────────┴─────┴─────┴─────┴─────┘
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

    scene = new Scene(this);
    scene->show();

    viewTypeToggle = new QPushButton(this);
    viewTypeToggle->setToolTip("切换俯视图/侧视图模式");
    dimModeToggle = new QPushButton(this);
    dimModeToggle->setToolTip("切换视区2D/3D模式");
    keepDistToggle = new QPushButton(this);
    keepDistToggle->setToolTip("保持相机与切面距离");

    toDefaultButton = new QPushButton("", this);
    toDefaultButton->setStyleSheet(
        //"QPushButton{color:white;font:bold 10pt;border:none;"
        //"min-height:24px;min-width:60px;"
        //"background:url(:images/SimpleButtom.png) -60px 0px no-repeat;}");
        "QPushButton{border-image:url(:/Start.png);}");
    toDefaultButton->setToolTip("回到对应视图的缺省状态");

    cameraMidBut = new QPushButton("=", this);
    cameraMidBut->setToolTip("相机俯仰角归零");

    someTestBut = new QPushButton("Test", this);
    someTestBut->setToolTip("Do Some Test");
    someTestBut->setMaximumWidth(30);

    int maxSz = 35;
    int minSz = 35;
    setButtonMinMaxIcon(toDefaultButton, minSz, maxSz);
    setButtonMinMaxIcon(cameraMidBut, minSz, maxSz);

    setCheckBoxMinMaxIcon(viewTypeToggle, minSz, maxSz, "ZViewType");
    setCheckBoxMinMaxIcon(dimModeToggle, minSz, maxSz, "ZDimension");
    setCheckBoxMinMaxIcon(keepDistToggle, minSz, maxSz, "ZKeepDist");

    maxSz = 18;
    vScrollBar = new ScrollBarV(Qt::Vertical, "vScrollBar", this);
    vScrollBar->setMaximumWidth(maxSz);
    vScrollBar->setMinimum(-300);
    vScrollBar->setMaximum(300);
    vScrollBar->setPageStep(50);

    dScrollBar = new ScrollBarV(Qt::Vertical, "dScrollBar", this);
    dScrollBar->setMaximumWidth(maxSz);
    dScrollBar->setMinimum(0);
    dScrollBar->setMaximum(100);
    dScrollBar->setPageStep(50);
    // 下面的设置可以与坐标系一致，但是 pageUp pageDown 操作又 反向的bug，所以放弃
    //dScrollBar->setInvertedAppearance(true);

    zoomDepthScrollBar = new ScrollBarV(Qt::Vertical, "zoomDepth", this);
    zoomDepthScrollBar->setMaximumWidth(maxSz);
    zoomDepthScrollBar->fmt = "深度放大%d倍";
    zoomDepthScrollBar->setToolTip("深度放大系数\n1倍..10倍");
    zoomDepthScrollBar->setMinimum(1);
    zoomDepthScrollBar->setMaximum(10);
    zoomDepthScrollBar->setPageStep(2);

    turnScrollBar = new ScrollBarV(Qt::Vertical, "turn", this);
    turnScrollBar->fmt = "旋转%d°";
    turnScrollBar->setMaximumWidth(maxSz);
    turnScrollBar->setToolTip("旋转视窗\n0(北)..359°");
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

    vScrollBar->fmt = "视窗上下位置%d(百米)";
    hScrollBar->fmt = "视窗水平位置%d(千米)";
    dScrollBar->fmt = "深度切面%d(百米)";

    zoomViewScrollBar = new ScrollBarV(Qt::Horizontal, "zoomView", this);
    zoomViewScrollBar->setMaximumHeight(maxSz);
    zoomViewScrollBar->setToolTip("视窗放大系数\n1..100(倍)");
    zoomViewScrollBar->setMinimum(1);
    zoomViewScrollBar->setMaximum(100);
    zoomViewScrollBar->fmt = "视区占%d%%";

    cutRadiusScrollBar = new ScrollBarV(Qt::Horizontal, "cutRadius", this);
    cutRadiusScrollBar->fmt = "纵切面位置%d(千米)";
    cutRadiusScrollBar->setMaximumHeight(maxSz);
    cutRadiusScrollBar->setToolTip("纵切面前后移动");
    cutRadiusScrollBar->setMinimum(-300);
    cutRadiusScrollBar->setMaximum(300);
    cutRadiusScrollBar->setPageStep(50);

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
    headUpScrollBar->setToolTip("相机抬头角度\n 范围：-45..45°");
    headUpScrollBar->setMinimum(-45);
    headUpScrollBar->setMaximum(45);
    headUpScrollBar->setPageStep(5);

    textIinfo = new QTextEdit(this);
    textIinfo->setMinimumWidth(200);
    textIinfo->setMaximumHeight(47);
    QTextCursor textCursor = textIinfo->textCursor();
    QTextBlockFormat textBlockFormat;
    textBlockFormat.setLineHeight(15, QTextBlockFormat::FixedHeight);//设置固定行高
    textCursor.setBlockFormat(textBlockFormat);
    textIinfo->setTextCursor(textCursor);
    textIinfo->setText("坐标数据\r相机参数");

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
    QString splitterStyle = "QSplitter::handle{background:rgb(120,120,140)}";
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

    layoutVInfo->addWidget(textIinfo);

    layoutHButtons->setMargin(1);
    layoutHButtons->addWidget(cameraMidBut);
    layoutHButtons->addWidget(viewTypeToggle);
    layoutHButtons->addWidget(dimModeToggle);
    layoutHButtons->addWidget(someTestBut);
    layoutHButtons->addWidget(keepDistToggle);
    layoutHButtons->addWidget(toDefaultButton);

    layoutB->addLayout(layoutVInfo);
    layoutB->addWidget(spliterV);
    layoutB->addLayout(layoutHButtons);

    layout->addWidget(scene, 0, 0);
    layout->addWidget(spliterH, 0,1);
    layout->addLayout(layoutB, 1,0,1,2);

    moveUp = new ShapeButton("ZButUp", 32, 32, this);
    moveDown = new ShapeButton("ZButDown", 32, 32, this);
    moveLeft = new ShapeButton("ZButLeft", 32, 32, this);
    moveRight = new ShapeButton("ZButRight", 32, 32, this);

    rotatePanel = new RotatePanel(100,100, this);

    updateControlToolTip();

    moveUp->show();
    moveDown->show();
    moveLeft->show();
    moveRight->show();
    connect(moveLeft, SIGNAL(pressed()), this, SLOT(onArrowMove()));
    connect(moveRight, SIGNAL(pressed()), this, SLOT(onArrowMove()));
    connect(moveUp, SIGNAL(pressed()), this, SLOT(onArrowMove()));
    connect(moveDown, SIGNAL(pressed()), this, SLOT(onArrowMove()));

    connect(toDefaultButton, SIGNAL(pressed()), this, SLOT(onToDefault()));

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

    connect(someTestBut, SIGNAL(pressed()), this, SLOT(onSomeTest()));
}

void SceneWin::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rotatePanel->move(2, scene->height()-rotatePanel->height()-2);

    // 可以考虑修改为一种配置，设置4个jiant 的位置
    int arrowPos = 1;
    // 箭头放置在4边的中间
    if (arrowPos == 0)
    {
        moveUp->move(scene->width()/2-moveUp->width()/2, 2);
        moveDown->move(scene->width()/2-moveDown->width()/2,
                scene->height()-moveDown->height()-2);
        moveLeft->move(2, scene->height()/2-moveLeft->height()/2);
        moveRight->move(scene->width()-moveRight->width(),
                scene->height()/2-moveRight->height()/2);
    }
    // 箭头放置在一起
    else if (arrowPos == 1)
    {
        int s = moveLeft->width();
        int c = 5;
        int d = s/4;
        moveUp->move(c+s-d, c);
        moveDown->move(c+s-d, c+s+s-d-d);
        moveLeft->move(c, c+s-d);
        moveRight->move(c+s+s-d-d, c+s-d);
    }
}

void SceneWin::procBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == &bbsUser)
        return;
    if (bbsMsg.varity == EBV_Field)
    {
        updateFieldRange();
        updateFromCameraPos();
        updateFromCutField();
        updateViewTypeModeCheck();
    }
    if (bbsMsg.varity == EBV_ViewType)
    {
        updateFromCameraPos();
        updateFromCutField();
        updateViewTypeModeCheck();
    }
    if (bbsMsg.varity == EBV_Camera)
        updateFromCameraPos();
    if (bbsMsg.varity == EBV_VCut
            || bbsMsg.varity == EBV_HCut)
        updateFromCutField();
    if (bbsMsg.varity == EBV_DimMode)
        updateViewTypeModeCheck();
}

void SceneWin::updateFieldRange()
{
    // 前后移动
    if (G.viewPot.viewType == EVT_Down)
    {
        vScrollBar->setMaximum(G.fieldRange.radius +G.fieldRange.outRadius);
        vScrollBar->setMinimum(-G.fieldRange.radius -G.fieldRange.outRadius);
        vScrollBar->setPageStep((vScrollBar->maximum() - vScrollBar->minimum())/20);
    }
    // 上下移动，
    else
    {
        vScrollBar->setMinimum(-G.fieldRange.aboveField);
        vScrollBar->setMaximum(G.fieldRange.depth +G.fieldRange.belowField);
        vScrollBar->setPageStep((vScrollBar->maximum() - vScrollBar->minimum())/20);
    }

    hScrollBar->setMinimum(-G.fieldRange.radius -G.fieldRange.outRadius);
    hScrollBar->setMaximum(G.fieldRange.radius +G.fieldRange.outRadius);
    hScrollBar->setPageStep((hScrollBar->maximum() - hScrollBar->minimum())/20);

    //
    dScrollBar->setMinimum(0);
    dScrollBar->setMaximum(G.fieldRange.depth);
    dScrollBar->setPageStep((dScrollBar->maximum() - dScrollBar->minimum())/10);

    cutRadiusScrollBar->setMinimum(-G.fieldRange.radius);
    cutRadiusScrollBar->setMaximum(G.fieldRange.radius);
    cutRadiusScrollBar->setPageStep((cutRadiusScrollBar->maximum() - cutRadiusScrollBar->minimum())/20);

    if (G.viewPot.viewType == EVT_Down)
    {
        forwardScrollBar->setMinimum(-G.fieldRange.aboveField);
        forwardScrollBar->setMaximum(G.fieldRange.depth + G.fieldRange.belowField);
        forwardScrollBar->setPageStep(
                (forwardScrollBar->maximum()-forwardScrollBar->minimum())/20);
    }
    else
    {
        int camOff = G.fieldRange.radius+G.fieldRange.outRadius;
        forwardScrollBar->setMinimum(-camOff);
        forwardScrollBar->setMaximum(camOff);
        forwardScrollBar->setPageStep(
                (forwardScrollBar->maximum()-forwardScrollBar->minimum())/20);
    }
    updateTextInfo();
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
        vScrollBar->fmt = "视窗前后位置%d(千米)";
        hScrollBar->fmt = "视窗左右位置%d(千米)";
        dScrollBar->fmt = "深度切面%d(百米)";

        moveUp->setToolTip("视区往前移动");
        moveDown->setToolTip("视区往后移动");
        moveLeft->setToolTip("视区往左移动");
        moveRight->setToolTip("视区往右移动");
    }
    // EVT_Side
    else if (G.viewPot.viewType == EVT_Side)
    {
        vScrollBar->setToolTip("视区上下方向移动");
        hScrollBar->setToolTip("视区水平方向移动");

        dScrollBar->setToolTip("深度切面上下移动");
        forwardScrollBar->setToolTip("相机前后移动");
        vScrollBar->fmt = "视窗上下位置%d(百米)";
        hScrollBar->fmt = "视窗水平位置%d(千米)";
        dScrollBar->fmt = "深度切面%d(百米)";

        moveUp->setToolTip("视区往上移动");
        moveDown->setToolTip("视区往下移动");
        moveLeft->setToolTip("视区往左移动");
        moveRight->setToolTip("视区往右移动");
    }
    vScrollBar->repaint();
    hScrollBar->repaint();
    dScrollBar->repaint();
}

void SceneWin::calculateCameraPos()
{
    int v = vScrollBar->value();
    int h = hScrollBar->value();

    int t = turnScrollBar->value();
    int camDist = forwardScrollBar->value();
    int head = headUpScrollBar->value();

    CameraPar & gCameraPar = G.viewPot.cameraPar;
    gCameraPar.viewPercent = zoomViewScrollBar->value();
    G.viewPot.zoomDepth = zoomDepthScrollBar->value();

    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        gCameraPar.x = h*cos(t*D2R) + v*sin(t*D2R);
        gCameraPar.z = -h*sin(t*D2R) + v*cos(t*D2R);
        gCameraPar.y = - camDist;

        gCameraPar.roll = t;
        gCameraPar.pitch = -90 + head;
        gCameraPar.yaw = 0.0;
    }
    // EVT_Side
    else
    {
        double t1 = 90 - t;
        gCameraPar.y = - v;
        gCameraPar.x = camDist*cos(t1*D2R) + h*sin(t1*D2R);
        gCameraPar.z = h*cos(t1*D2R) - camDist*sin(t1*D2R);

        gCameraPar.pitch = head;
        gCameraPar.yaw = t;
        gCameraPar.roll = 0;
    }
    updateTextInfo();
}

void SceneWin::updateFromCameraPos()
{
    CameraPar & gCamPar = G.viewPot.cameraPar;
    qDebug() << "SceneWin::updateFromCameraPos";

    // EVT_Down
    if (G.viewPot.viewType == EVT_Down)
    {
        qDebug() << "Roll=" << gCamPar.roll;
        double t = gCamPar.roll;
        double v = gCamPar.x*sin(t*D2R) + gCamPar.z*cos(t*D2R);
        double h = gCamPar.x*cos(t*D2R) - gCamPar.z*sin(t*D2R);
        vScrollBar->setValue(v);
        hScrollBar->setValue(h);
        turnScrollBar->setValue(gCamPar.roll);
        forwardScrollBar->setValue(-gCamPar.y);
        headUpScrollBar->setValue(gCamPar.pitch + 90);
    }
    // EVT_Side
    else
    {
        qDebug() << "Yaw=" << gCamPar.yaw;
        vScrollBar->setValue(-gCamPar.y);
        turnScrollBar->setValue(gCamPar.yaw);
        headUpScrollBar->setValue(gCamPar.pitch);

        double t1 = 90 - gCamPar.yaw;
        double comDist = gCamPar.x*cos(t1*D2R) -gCamPar.z*sin(t1*D2R);
        double h = gCamPar.x*sin(t1*D2R) +gCamPar.z*cos(t1*D2R);
        hScrollBar->setValue(h);
        forwardScrollBar->setValue(comDist);
        gCamPar.roll = 0;
    }
    zoomDepthScrollBar->setValue(G.viewPot.zoomDepth);
    zoomViewScrollBar->setValue(gCamPar.viewPercent);
    updateTextInfo();
}

void SceneWin::calculateCutFileld()
{
    FieldCut & gCutField = G.viewPot.fieldCut;
    gCutField.vCutRadius = cutRadiusScrollBar->value();
    gCutField.vCutAngle = cutAngleScrollBar->value();
    gCutField.cutDepth = dScrollBar->value();
    updateTextInfo();
}

void SceneWin::updateFromCutField()
{
    FieldCut & gCutField = G.viewPot.fieldCut;
    cutRadiusScrollBar->setValue(gCutField.vCutRadius);
    cutAngleScrollBar->setValue(gCutField.vCutAngle);
    dScrollBar->setValue(gCutField.cutDepth);
    updateTextInfo();
}

void SceneWin::updateViewTypeModeCheck()
{
    if (G.viewPot.viewType == EVT_Down)
    {
        viewTypeToggle->setChecked(true);
    }
    else
    {
        viewTypeToggle->setChecked(false);
    }

    if (G.viewPot.dimMode == EDM_2D)
    {
        dimModeToggle->setChecked(false);
    }
    else
    {
        dimModeToggle->setChecked(true);
    }
}

void SceneWin::updateTextInfo()
{
    CameraPar & gCameraPar = G.viewPot.cameraPar;
    QString camS;
    camS.sprintf("Cam:%c XYZ=%3.1lf %3.1lf %3.1lf "
            "Roll=%2.1lf Pitch=%2.1lf Yaw=%2.1lf V%=%1.0lf D*=%1.0lf\r\n",
            G.viewPot.viewType == EVT_Down ? 'V':'H',
            gCameraPar.x, gCameraPar.y, gCameraPar.z,
            gCameraPar.roll, gCameraPar.pitch, gCameraPar.yaw,
            gCameraPar.viewPercent, G.viewPot.zoomDepth);

    FieldCut & gCutField = G.viewPot.fieldCut;
    QString cutS;
    cutS.sprintf("Cut: D=%4.1lf A=%4.1lf R=%4.1lf",
            gCutField.cutDepth, gCutField.vCutAngle,
            gCutField.vCutRadius);

    textIinfo->setText(camS + cutS);

    // 窗口标题区域，显示主要的静态参数，包括场区范围、大小、中心点、备注等
    QString windowTitle;
    windowTitle.sprintf("Center Lat=%8.4lf Lon=%8.4lf "
            "W=%4.1lf(km) H=%4.1lf(km) Depth=%4.0lf(百米) Note=%s",
            G.fieldRange.centerP.lat, G.fieldRange.centerP.lat,
            G.fieldRange.width, G.fieldRange.height,
            G.fieldRange.depth,
            G.scheme.schemeInfo.toUtf8().data());
    setWindowTitle(windowTitle);

//    vScrollBar->repaint();
//    hScrollBar->repaint();
//    dScrollBar->repaint();
//    cutAngleScrollBar->repaint();
//    cutRadiusScrollBar->repaint();
//    forwardScrollBar->repaint();
//    headUpScrollBar->repaint();
//    zoomDepthScrollBar->repaint();
//    zoomViewScrollBar->repaint();
//    turnScrollBar->repaint();
}

void SceneWin::onDimModeTogggle(bool checked)
{
    qDebug() << "onDimModeTogggle" << checked;
    if (checked)
        G.viewPot.dimMode = EDM_3D;
    else
        G.viewPot.dimMode = EDM_2D;
    BBSMessage msg(EBS_SceneWin, EBV_DimMode);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewTypeTogle(bool checked)
{
    qDebug() << "onViewTypeTogle" << checked;
    // 设置好全局变量
    if (checked)
        G.setViewType(EVT_Down);
    else
        G.setViewType(EVT_Side);

    // 通知其它客户端
    BBSMessage msg(EBS_SceneWin, EBV_ViewType);
    bbsUser.sendBBSMessage(msg);
    // 更新本窗口显示
    updateControlToolTip();
    updateFieldRange();
    updateFromCameraPos();
    updateFromCutField();
}

void SceneWin::onKeepDistToggle(bool checked)
{
    if (checked)
        keepDist = false;
    else
        keepDist = true;
}

void SceneWin::onToDefault()
{
    if (G.viewPot.viewType == EVT_Down)
        G.viewPot = G.defaultViewPotV;
    else
        G.viewPot = G.defaultViewPotH;
    // 通知其它客户端
    BBSMessage msg(EBS_SceneWin, EBV_ViewType);
    bbsUser.sendBBSMessage(msg);
    // 刷新本窗口显示
    updateControlToolTip();
    updateFieldRange();
    updateFromCameraPos();
    updateFromCutField();
}

void SceneWin::onCameraPitchMid()
{
    headUpScrollBar->setValue(
        (headUpScrollBar->minimum()+headUpScrollBar->maximum())/2);
    updateTextInfo();
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
        delta = 1;
    }
    else if (button == moveDown)
    {
        scroll = vScrollBar;
        delta = -1;
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

void SceneWin::onViewAreaVScroll(int v)
{
    if (! vScrollBar->hasFocus())
        return;
    qDebug() << "onViewAreaVScroll=" << v;
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewAreaHScroll(int v)
{
    if (! hScrollBar->hasFocus())
        return;
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onZoomDepthScroll(int v)
{
    if (! zoomDepthScrollBar->hasFocus())
        return;
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCameraTurnScroll(int v)
{
    if (! turnScrollBar->hasFocus())
        return;
    qDebug() << "onCameraTurnScroll";
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCameraForwardScroll(int v)
{
    if (! forwardScrollBar->hasFocus())
        return;
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onViewDepthScroll(int v)
{
    if (! dScrollBar->hasFocus())
        return;
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onZoomViewScroll(int v)
{
    if (! zoomViewScrollBar->hasFocus())
        return;
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCutRadiusScroll(int v)
{
    if (! cutRadiusScrollBar->hasFocus())
        return;
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onCutAngleScroll(int v)
{
    if (!cutAngleScrollBar->hasFocus())
        return;
    calculateCutFileld();
    BBSMessage msg(EBS_SceneWin, EBV_VCut);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onHeadUpScroll(int v)
{
    if (! headUpScrollBar->hasFocus())
        return;
    calculateCameraPos();
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onSomeTest()
{
    qDebug() << "onSomeTest";
    //viewTypeToggle->setChecked(false);
    vScrollBar->setValue(vScrollBar->value()+10);
    return;

    updateFromCameraPos();
    updateFromCutField();
    updateViewTypeModeCheck();
}
