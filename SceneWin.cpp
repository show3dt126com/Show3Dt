#include "SceneWin.h"

#include <QGridLayout>

SceneWin::SceneWin(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(0);

    setLayout(layout);

    int maxsz = 25;
    conerButton = new QPushButton("*", this);
    conerButton->setMaximumWidth(maxsz);
    conerButton->setMaximumHeight(maxsz);
    vScrollBar = new QScrollBar(Qt::Vertical, this);
    vScrollBar->setMaximumWidth(maxsz);
    hScrollBar =new QScrollBar(Qt::Horizontal, this);
    hScrollBar->setMaximumHeight(maxsz);
    scene = new Scene(this);

    layout->addWidget(scene, 0, 0);
    layout->addWidget(vScrollBar, 0, 1);
    layout->addWidget(hScrollBar, 1, 0);
    layout->addWidget(conerButton, 1, 1);

    scene->show();
//    vScrollBar->show();
//    hScrollBar->show();
//    conerButton->show();

    connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onVScroll(int)));
    connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onHScroll(int)));
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

void SceneWin::onVScroll(int v)
{
    BBSMessage msg(EBS_Scene, EBV_Camera);
    bbsUser.sendBBSMessage(msg);
}

void SceneWin::onHScroll(int v)
{
    BBSMessage msg(EBS_Scene, EBV_Camera);
    bbsUser.sendBBSMessage(msg);

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
}
