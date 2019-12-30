#include "MapMan.h"
#include <QFile>
#include <QDebug>
#include "Global.h"
#include <QtAlgorithms>


MapMan::MapMan()
{

}


//排列判断
bool compareDemInfo(const DemInfo &a, const DemInfo &b)
{
    if (a.level < b.level)
        return true;
    if (a.texLB0.lon < b.texLB0.lon)
        return true;
    return false;
}

int MapMan::init()
{
    QString indexListFile = G.pathToDemFile + "\\DemIndexList.txt";
    QFile indexLF(indexListFile);
    if(!indexLF.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() <<"Can't open the DemIndexList.txt!"<<endl;
        return -1;
    }

    while(!indexLF.atEnd())
    {
        QByteArray indexFileName = indexLF.readLine();
        QString indexFile = G.pathToDemFile +"\\" + QString(indexFileName);
        QFile indexF(indexFile);
        if(!indexF.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Can't open: " << indexFile;
            return -1;
        }

        while(!indexF.atEnd())
        {
            QString texInfoS = QString(indexF.readLine());
            // 解析一个 txt 文件的信息

            DemInfo demInfo;

//            QTextStream in(&file);
//            in.setCodec( QTextCodec::codecForName("UTF8"));
//            QString txtAll = in.readAll();
//            QStringList list = txtAll.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
//            //qDebug() << "loadTextureFile=" <<  txtAll << list;
//            //qDebug() << list[1] << list[2];
//            QRegExp rx("\\d+");
//            //QString wStr = list[1].mid(3);
//            rx.indexIn(list[1], 3);
//            //qDebug() << rx.cap(0) << rx.cap(1);
//            tex.texW = rx.cap(0).toInt();
//            rx.indexIn(list[2], 3);
//            tex.texH = rx.cap(0).toInt();

//            QStringList p = list[9].mid(6).split(",");
//            tex.texXY0.x = p[0].toDouble();
//            tex.texXY0.y = p[1].toDouble();
//            tex.texXY0.z = 0.0;
//            p = list[11].mid(6).split(",");
//            tex.texXY1.x = p[0].toDouble();
//            tex.texXY1.y = p[1].toDouble();
//            tex.texXY1.z = 0.0;

//            tex.texLB0 = WebMercator::webMercator2lonLat(tex.texXY0);
//            tex.texLB1 = WebMercator::webMercator2lonLat(tex.texXY1);


            demList.append(demInfo);
        }
        indexF.close();
    }
    indexLF.close();

    // 对文件按时间排序
    std::sort(demList.begin(), demList.end(), compareDemInfo);

}

QString MapMan::findFitMap(double l0, double b0, double l1, double b1)
{

}
