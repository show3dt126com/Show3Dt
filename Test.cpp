#include "Test.h"
#include <QDebug>
#include "Global.h"
#include "ToolBox.h"
#include "WebMercator.h"

Test T;

Test::Test()
{

}

void Test::init()
{
    bbs1.init();
    //bbs2.init();
    //bbs3.init();
}

int Test::test(int p)
{
    //testBBS(p);
    //testSchemeSaveLoad();
    //testWebMercator();
    //testFindMap();
    testMakeBatchDownloadShuiJingZhuFile();
    return 1;
}

void Test::testWebMercator()
{
    DVec3 lonLat;
    lonLat.x = 110;
    lonLat.y = 25;
    lonLat.z = 0;

    DVec3 m = WebMercator::lonLat2WebMercatorD(lonLat);
    DVec3 ll = WebMercator::webMercator2lonLatD(m);
    qDebug() << "Test ll2map" << m.x << m.y << m.z << ll.x << ll.y << ll.z;

    DVec3 twM0, twM1, twLL0, twLL1;
    twM0.x = 12972241.510078705847263;
    twM0.y = 2399498.498509500175714;
    twM1.x = 13662089.402329696342349;
    twM1.y = 3084405.306345136370510;

    twLL0 = WebMercator::webMercator2lonLatD(twM0);
    twLL1 = WebMercator::webMercator2lonLatD(twM1);
    qDebug() << "Test TaiWan" << twLL0.x << twLL0.y << twLL1.x << twLL1.y;
    // 116.532 21.0639 122.729 26.6869
    QString taiWanMap = G.mapMan.findFitMap(116.6, 21.1, 122.6, 26.6);
    qDebug() << "taiWanMap=" << taiWanMap;

}

void Test::testFindMap()
{

}

void Test::testMakeBatchDownloadShuiJingZhuFile()
{
    makeBatchDownloadConfigFileForShuiJingZhuMapGrid();
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

int Test::testSceneWin()
{
    Field field;
    field.lat0 = 30;
    field.lat1 = 32;
    field.lon0 = 115;
    field.lon1 = 117;
    field.depth = 15000;
    G.setField(field);

    G.scheme.schemeInfo = "台湾海峡周围150km海声场春季温度特性";
    G.viewPot = G.defaultViewPotV;


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
    scheme.viewPots.append(vp);
    scheme.viewPots.append(vp);

    scheme.saveToFile("./test/1.sch");

    Scheme scheme1;
    scheme1.loadFromFile("./test/1.sch");
    qDebug() << scheme1.cubeFiles[1];
}


