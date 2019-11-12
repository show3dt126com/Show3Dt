#include "Test.h"
#include <QDebug>


Test T;

Test::Test()
{

}

void Test::init()
{
    bbs1.init();
    bbs2.init();
    bbs3.init();
}

int Test::test(int p)
{
    testBBS(p);
    //testSchemeSaveLoad();
    return 1;
}

int Test::testBBS(int p)
{
    if (p == 1)
    {
        BBSMessage msg(EBS_Scene, EBV_Camera);
        T.bbs1.sendBBSMessage(msg);
    }

    else if (p == 2)
    {
        BBSMessage msg(EBS_Scene, EBV_Camera);
        T.bbs2.sendBBSMessage(msg);
    }
    return 1;
}

BBS1::BBS1(QObject *parent) : BBSBase(parent)
{}
int  BBS1::onBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == this)
        return -1;
    qDebug() << "BBS1" << bbsMsg.source << bbsMsg.varity;
    return 1;
}

BBS2::BBS2(QObject *parent) : BBSBase(parent)
{}
int  BBS2::onBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == this)
        return -1;
    qDebug() << "BBS2" << bbsMsg.source << bbsMsg.varity;
    return 1;
}

BBS3::BBS3(QObject *parent) : BBSBase(parent)
{}
int  BBS3::onBBSMessage(BBSMessage bbsMsg)
{
    if (bbsMsg.sender == this)
        return -1;
    qDebug() << "BBS3" << bbsMsg.source << bbsMsg.varity;
    return 1;
}


int Test::testSchemeSaveLoad()
{
    Scheme scheme;
    scheme.cubeFiles.append("111.cub");
    scheme.cubeFiles.append("222.cub");
    scheme.colorMap.keyNum = 4;
    scheme.soundSourceFiles.append("444.snd");
    scheme.soundSourceFiles.append("555.snd");
    ViewPot vp;
    vp.lat = 30;
    vp.lon = 100;
    vp.roll = 0;
    vp.pitch = 45;
    vp.yaw = 56;
    scheme.viewPots.append(vp);
    vp.lat = 31;
    vp.lon = 101;
    vp.pitch = 46;
    vp.yaw = 57;
    scheme.viewPots.append(vp);

    scheme.saveToFile("./test/1.sch");

    Scheme scheme1;
    scheme1.loadFromFile("./test/1.sch");
    qDebug() << scheme1.viewPots[1].yaw;
    qDebug() << scheme1.cubeFiles[1];
}


