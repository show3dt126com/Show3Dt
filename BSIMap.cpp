#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtMath>

#include "BSIMap.h"
#include "WebMercator.h"


static const char * allBSIMapRoot  = "D:/ShuiJingZhu-GoogleMap/";
static const char * allBSIMapPath [] =
{
    "GoogleMap-05",
    "GoogleMap-06",
    "GoogleMap-07",
    "GoogleMap-08",
    "GoogleMap-09",
    "GoogleMap-10"
};


int BSIMapTxt::lb2tex(DVec3 lb, double & texX, double & texY)
{
    texX = (lb.x - texLB0.x) * texW / (texLB1.x - texLB0.x);
    DVec3 xy =  WebMercator::lonLat2WebMercatorD(lb);
    texY = (texXY0.y - xy.y) * texH /  (texXY0.y - texXY1.y);
    return 1;
}

int BSIMapTxt::tex2lb(double texX, double texY, DVec3 & lb)
{
    DVec3 xy;
    xy.y = texXY0.y - texY * (texXY0.y - texXY1.y) / texH;
    lb =  WebMercator::webMercator2lonLatD(xy);
    lb.x = texLB0.x + texX * (texLB1.x - texLB0.x) / texW;
    return 1;
}

BSIMap::BSIMap()
{
    allBSIMTxt = nullptr;
}

void BSIMap::readAllPathBSIMapDB()
{
    int pn = sizeof(allBSIMapPath) / sizeof(char *);
    allTxtNum = 0;
    for (int i=0; i<pn; i++)
    {
        char buf[200];
        sprintf(buf, "%s%s.db", allBSIMapRoot, allBSIMapPath[i]);
        // read db
        FILE * fp = fopen(buf, "rb");
        if (fp != nullptr)
        {
            int num;
            fread(&num, 1, sizeof(int), fp);
            allTxtNum += num;
            fclose(fp);
        }
    }

    allBSIMTxt = new BSIMapTxt[allTxtNum];
    int all0 = 0;
    for (int i=0; i<pn; i++)
    {
        char buf[200];
        sprintf(buf, "%s%s.db", allBSIMapRoot, allBSIMapPath[i]);
        // read db
        FILE * fp = fopen(buf, "rb");
        if (fp != nullptr)
        {
            int num;
            fread(&num, 1, sizeof(int), fp);
            fread(allBSIMTxt+all0, sizeof(BSIMapTxt), num, fp);
            for (int j=0; j<num; j++)
            {
                BSIMapTxt & txt = allBSIMTxt[all0+j];
                char buf[256];
                sprintf(buf, "%s/%s", allBSIMapRoot, txt.file);
                strcpy(txt.file, buf);
                //qDebug() << "dem.file=" << i << dem.file;
            }
            all0 += num;
            fclose(fp);
        }
    }
}

// 读取分级地图目录下面的信息文件，产生数据库
void BSIMap::readAllPathBSIMapTxt()
{
    int pn = sizeof(allBSIMapPath) / sizeof(char *);
    for (int i=0; i<pn; i++)
    {
        char buf[200];
        readPathAllBSIMapTxt(allBSIMapPath[i]);
        sprintf(buf, "%s%s.db", allBSIMapRoot, allBSIMapPath[i]);
        makePathAllBSIMapTxtDB(buf);
    }
}

// 根据读取的dem目录下面的信息，构数据库
void BSIMap::makePathAllBSIMapTxtDB(QString txtDBFile)
{
    FILE * fp = fopen(txtDBFile.toLatin1().data(), "wb");
    fwrite(&allTxtNum, 1, sizeof(int), fp);
    fwrite(allBSIMTxt, sizeof(BSIMapTxt), allTxtNum, fp);
    fclose(fp);
}

// 读取 map 目录下面所有的信息文件
void BSIMap::readPathAllBSIMapTxt(QString mapPath)
{
    QDir dir(QString(allBSIMapRoot) + mapPath);
    QStringList filter;
    filter << "*.txt";
    QStringList files = dir.entryList(filter, QDir::Files|QDir::Readable, QDir::Name);
    int txtCount = files.count();
    qDebug() << "txtCount" << txtCount;

    if (allBSIMTxt != nullptr)
        delete [] allBSIMTxt;
    allBSIMTxt = new BSIMapTxt[txtCount];

    allTxtNum = 0;
    for (int i=0; i<txtCount; i++)
    {
        QString txtFile = files[i];
        qDebug()  << "Read Txt=" << txtFile;

        // 从水-经-注下载的卫星图，导出大图后得到一个整体的jpg文件和一个txt信息文件。
        // txt信息文件的内容如下：
        //【图片信息】
        //宽度：4056像素
        //高度：3232像素
        //波段数：3
        //位深度：24
        //比例尺(72DPI)：1:9244648
        //空间分辨率：2445.984905米/像素

        //【坐标信息】
        //输出坐标系：Web墨卡托坐标系
        //左上角坐标：8433755.952873207628727,7181411.681448882445693
        //右上角坐标：18354670.728062804788351,7181411.681448882445693
        //右下角坐标：18354670.728062804788351,-724011.531917189946398
        //左下角坐标：8433755.952873207628727,-724011.531917189946398

        QFile file(mapPath+txtFile);
        if (!file.open(QIODevice::ReadOnly| QIODevice::Text))
            continue;

        QTextStream in(&file);
        //in.setCodec( QTextCodec::codecForName("UTF8"));
        in.setCodec( QTextCodec::codecForName("GB2312"));
        //in.setCodec( QTextCodec::codecForName("GBK"));
        //in.setAutoDetectUnicode(true);
        QString txtAll = in.readAll();
        QStringList list = txtAll.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
        //qDebug() << "loadTextureFile=" <<  txtAll << list;
        //qDebug() << list[1] << list[2];

        BSIMapTxt & txt = allBSIMTxt[allTxtNum];
        QRegExp rx("\\d+");
        rx.indexIn(list[1], 3);
        txt.texW = rx.cap(0).toInt();
        rx.indexIn(list[2], 3);
        txt.texH = rx.cap(0).toInt();

        QRegExp rxd("\\d+(\\.\\d+)?");
        rxd.indexIn(list[6], 6);
        txt.mpp = rxd.cap(0).toInt();
        //qDebug() << "list[6]" << list[6] << dem.mpp;

        QStringList p = list[9].mid(6).split(",");
        qDebug() << "list[9]" << p;
        txt.texXY0.x = p[0].toDouble();
        txt.texXY0.y = p[1].toDouble();
        txt.texXY0.z = 0.0;
        p = list[11].mid(6).split(",");
        txt.texXY1.x = p[0].toDouble();
        txt.texXY1.y = p[1].toDouble();
        txt.texXY1.z = 0.0;

        txt.texLB0 = WebMercator::webMercator2lonLat(txt.texXY0);
        txt.texLB1 = WebMercator::webMercator2lonLat(txt.texXY1);

        double s = txt.texW / (txt.texLB1.x - txt.texLB0.x);
        txt.scale = 1;
        double minScale = 256.0 / MAX_MERCATOR_LON / 2.0;
        while (s >= minScale+0.0000001)
        {
            s /= 2.0;
            txt.scale += 1.0;
        }
        // GD.mainPath + QString("/dem/") +
        QString txtFileName =  txtFile.left(txtFile.length()-3);
        sprintf(txt.file, "%s%s", mapPath.toLatin1().data(), txtFileName.toLatin1().data());
        qDebug() << "  File=" << i << txt.scale << txt.file;

        file.close();
        allTxtNum ++;
    }
}

int BSIMap::findMaxCoverMap(double lon0, double lat0, double lon1, double lat1, QString & demFile)
{
    int findDem = -1;
    double findScale = 0;
    for (int i=0; i<allTxtNum; i++)
    {
        BSIMapTxt & dem = allBSIMTxt[i];
        // 必须完整覆盖，且找最高分辨率的
        if (dem.texLB0.x < lon0 && dem.texLB1.x > lon1
                && dem.texLB0.y > lat1 && dem.texLB1.y < lat0)
        {
            if (findDem == -1 || findScale < dem.scale)
                findScale = dem.scale;
            findDem = i;
        }
    }

    if (findDem != -1)
    {
        demFile = allBSIMTxt[findDem].file;
        qDebug() << "findMaxCoverDem=" << demFile;
    }
    else
    {
        qDebug() << "findMaxCoverDem Fail";
    }
    return findDem;
}

int BSIMap::findMaxCoverMap(double centL, double centB, double rr, QString & demFile)
{
    double dB = rr * M2D;
    double dL = rr * M2D / cos(centB*D2R);
    return findMaxCoverMap(centL-dL, centB-dB, centL+dL, centB+dB, demFile);
}
