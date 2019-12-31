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
    else if (a.level > b.level)
        return false;
    else if (a.texLB0.lon < b.texLB0.lon-1E-10)
        return true;
    else if (a.texLB0.lon > b.texLB0.lon+1E-10)
        return false;
    else if (a.texLB0.lat < b.texLB0.lat-1E-10)
        return true;
    return false;
}

int MapMan::init()
{
    QString indexListFile = G.pathToDemFile + "\\WorldIndexList.txt";
    QFile indexLF(indexListFile);
    if(!indexLF.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() <<"Can't open the WorldIndexList.txt!"<<endl;
        qDebug() << indexListFile;
        return -1;
    }

    int c = 0;
    while(!indexLF.atEnd())
    {
        QString indexFileName = QString(indexLF.readLine());
        if (indexFileName.length() < 5)
            break;
        indexFileName.replace("\r", "").replace("\n", "");
        QString indexFile = G.pathToDemFile +"\\" + indexFileName;
        QFile indexF(indexFile);
        if(!indexF.open(QIODevice::ReadOnly | QIODevice::Text))
            break;
        qDebug() << "indexFile=" << indexFile;

        while(!indexF.atEnd())
        {
            QString texInfoS = QString(indexF.readLine());
            // 解析一个 txt 文件的信息
            // World-L05-000-000|-180.0000000000000,-84.0000000000000|180.0000000000000,84.0000000000000|5
            DemInfo demInfo;
            QStringList list = texInfoS.split(QRegExp("[|,\n\r]"), QString::SkipEmptyParts);
            if (list.count() != 6)
                continue;
            //qDebug() << "MapMan::init " << list;
            c ++;
            demInfo.level = list[5].toInt();
            demInfo.texLB0.lon = list[1].toDouble();
            demInfo.texLB0.lat = list[2].toDouble();
            demInfo.texLB1.lon = list[3].toDouble();
            demInfo.texLB1.lat = list[4].toDouble();

            demList.append(demInfo);
        }
        indexF.close();
    }
    qDebug() << "Dem File Num = " << c;
    indexLF.close();

    // 对文件按时间排序
    std::sort(demList.begin(), demList.end(), compareDemInfo);

    FILE * fp = fopen("D:\\ShuiJingZhu-GoogleMap\\Sort.txt", "wt");
    for (int i=0; i<demList.count(); i++)
    {
        fprintf(fp, "%d %d %10.7lf %10.7lf %10.7lf %10.7lf %10.7lf %10.7lf\n",
                i, demList[i].level,
                demList[i].texLB0.lon, demList[i].texLB0.lat,
                demList[i].texLB1.lon, demList[i].texLB1.lat,
                demList[i].texLB1.lon-demList[i].texLB0.lon,
                demList[i].texLB1.lat-demList[i].texLB0.lat);
        }
    fclose(fp);
}

QString MapMan::findFitMap(double l0, double b0, double l1, double b1)
{

}
