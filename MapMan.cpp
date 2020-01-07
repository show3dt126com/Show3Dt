#include "MapMan.h"
#include <QFile>
#include <QDebug>
#include "Global.h"
#include <QtAlgorithms>
#include "WebMercator.h"


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


/*
  下载水经注地图时，考虑的是不拼图方式。奇数编号的图片都是用于覆盖接缝处的。
  可以：修改为拼图方式：作为纹理时，选择合适的相邻4张图片进行覆盖。
  作为地图时，根据当前缩放级别，选择相邻的4张图片进行显示。
*/

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
            QStringList listName = list[0].split(QRegExp("[-]"), QString::SkipEmptyParts);
            if (listName.count() != 4)
                continue;
            int x = listName[2].toInt();
            int y = listName[3].toInt();

            // 如果采用拼图方式，重叠的图片不需要考虑
            if (x%2!=0 || y%2!=0)
                continue;

            //qDebug() << "MapMan::init " << list;
            c ++;
            demInfo.demFile = list[0];
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

/*
  决定合适图片的策略：最多4张相邻图片拼接。文字大小要合适，即显示的分辨率与屏幕分辨率吻合。
  图片分辨率为：4096*3844  屏幕分辨率为：1920*1080。
  如果正好落在一张图片，如果落在2张图片中间，则需要选择上一级的图片，造成分辨率显示不一致，
  这种情况需要拼接。根据范围经纬度，计算 Mercator坐标范围，根据 Mercator 范围选择合适的级别。
*/

QString MapMan::findFitMap(double l0, double b0, double l1, double b1)
{
    DVec3 lonLat0(l0, b0, 0.0);
    DVec3 xy0 = WebMercator::lonLat2WebMercatorD(lonLat0);
    DVec3 lonLat1(l1, b1, 0.0);
    DVec3 xy1 = WebMercator::lonLat2WebMercatorD(lonLat1);

    for (int i=demList.count()-1; i>=0; i--)
    {
        DemInfo & di = demList[i];
        if (di.texLB0.lon < l0 && di.texLB0.lat < b0
                && di.texLB1.lon > l1 && di.texLB1.lat > b1)
        {
            printf("findFitMap= %d %d %s %10.7lf %10.7lf %10.7lf %10.7lf %10.7lf %10.7lf\n",
                   i, di.level, di.demFile.toLatin1().data(),
                   di.texLB0.lon, di.texLB0.lat, di.texLB1.lon, di.texLB1.lat,
                   di.texLB1.lon-di.texLB0.lon, di.texLB1.lat-di.texLB0.lat);
            return di.demFile;
        }
    }
    return "";
}
