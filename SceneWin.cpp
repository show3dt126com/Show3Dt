#include "SceneWin.h"

#include <QGridLayout>
#include <QBitmap>

SceneWin::SceneWin(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(1);
    layout->setSpacing(1);
    layout->setRowStretch(0, 100);
    layout->setColumnStretch(0, 100);

    setLayout(layout);

    QIcon icon;
    icon.addFile("url(:/Clock.png)");
    QPalette p = palette();
    p.setBrush(QPalette::Button, QBrush(QPixmap("url(:/Clock.png)")));

    conerButton = new QPushButton("*", this);
    conerButton->setStyleSheet(
        //"QPushButton{color: white;font: bold 10pt;border:none;"
        //"min-height: 24px;min-width: 60px;"
        //"background:url(:images/SimpleButtom.png) -60px 0px no-repeat;}");
        "border-image:url(:/Start.png);}");

    zoomInBut = new QPushButton("", this);
    zoomOutBut = new QPushButton("", this);
    turnClockBut = new QPushButton("", this);
    turnAClockBut = new QPushButton(this);

    zoomOutBut->setStyleSheet(
                "QPushButton{border-image:url(:/ZoomOut.png);}"
                "QPushButton:hover{border-image:url(:/ZoomOut-H.png);}"
                "QPushButton:pressed{border-image:url(:/ZoomOut-P.png);}");
    zoomInBut->setStyleSheet(
                "QPushButton{border-image:url(:/ZoomIn.png);}"
                "QPushButton:hover{border-image:url(:/ZoomIn-H.png);}"
                "QPushButton:pressed{border-image:url(:/ZoomIn-P.png);}");
    turnAClockBut->setStyleSheet(
                "QPushButton{border-image:url(:/ClockA.png);}"
                "QPushButton:hover{border-image:url(:/ClockA-H.png);}"
                "QPushButton:pressed{border-image:url(:/ClockA-P.png);}");
    turnClockBut->setStyleSheet(
                "QPushButton{border-image:url(:/Clock.png);}"
                "QPushButton:hover{border-image:url(:/Clock-H.png);}"
                "QPushButton:pressed{border-image:url(:/Clock-P.png);}");

    int maxsz = 30;
    conerButton->setMinimumWidth(maxsz);
    conerButton->setMinimumHeight(maxsz);
    turnClockBut->setMinimumWidth(maxsz);
    turnClockBut->setMinimumHeight(maxsz);
    turnAClockBut->setMinimumWidth(maxsz);
    turnAClockBut->setMinimumHeight(maxsz);
    zoomInBut->setMinimumWidth(maxsz);
    zoomInBut->setMinimumHeight(maxsz);
    zoomOutBut->setMinimumWidth(maxsz);
    zoomOutBut->setMinimumHeight(maxsz);

    maxsz = 30;
    conerButton->setMaximumWidth(maxsz);
    conerButton->setMaximumHeight(maxsz);
    turnClockBut->setMaximumWidth(maxsz);
    turnClockBut->setMaximumHeight(maxsz);
    turnAClockBut->setMaximumWidth(maxsz);
    turnAClockBut->setMaximumHeight(maxsz);
    zoomInBut->setMaximumWidth(maxsz);
    zoomInBut->setMaximumHeight(maxsz);
    zoomOutBut->setMaximumWidth(maxsz);
    zoomOutBut->setMaximumHeight(maxsz);

    maxsz = 18;
    vScrollBar = new QScrollBar(Qt::Vertical, this);
    vScrollBar->setMaximumWidth(maxsz);
    hScrollBar =new QScrollBar(Qt::Horizontal, this);
    hScrollBar->setMaximumHeight(maxsz);
    dScrollBar = new QScrollBar(Qt::Vertical, this);
    dScrollBar->setMaximumWidth(maxsz);

    scene = new Scene(this);

    layout->addWidget(scene, 0, 0, 8, 1);
    layout->addWidget(hScrollBar, 8, 0, 1, 1);

    layout->addWidget(vScrollBar, 0, 1, 1, 1);
    layout->addWidget(dScrollBar, 0, 2, 1, 1);

    layout->addWidget(zoomInBut, 1, 1, 2, 2);
    layout->addWidget(zoomOutBut, 3, 1, 2, 2);

    layout->addWidget(turnAClockBut, 5, 1, 2, 2);
    layout->addWidget(turnClockBut, 7, 1, 2, 2);

    //layout->addWidget(conerButton, 3, 4, 1, 1);

    scene->show();

    connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onVScroll(int)));
    connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onHScroll(int)));
    connect(dScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onDScroll(int)));
    connect(&bbsUser, SIGNAL(bbsMessage(BBSMessage)), this, SLOT(onBBSMessage(BBSMessage)));

    connect(zoomInBut, SIGNAL(pressed()), this, SLOT(onZoomIn()));
    connect(zoomOutBut, SIGNAL(pressed()), this, SLOT(onZoomOut()));
    connect(turnClockBut, SIGNAL(pressed()), this, SLOT(onTurnClock()));
    connect(turnAClockBut, SIGNAL(pressed()), this, SLOT(onTurnAClock()));

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

void SceneWin::onVScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onHScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onDScroll(int v)
{
    BBSMessage msg(EBS_SceneWin, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onZoomIn()
{

}

void SceneWin::onZoomOut()
{

}

void SceneWin::onTurnClock()
{

}

void SceneWin::onTurnAClock()
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
