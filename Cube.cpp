#include "Cube.h"

#include <string.h>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QDebug>
#include "ToolBox.h"

typedef char * PChar;

Cube::Cube()
{
    initCube();
}

// 初始化函数
int Cube::initCube()
{
    strcpy(fileFlag, "SR-G101");
    nLat = 0;
    nLon = 0;
    nDepth = 0;

    lats = nullptr;
    lons = nullptr;
    depths = nullptr;
    data = nullptr;

    return 1;
}

// 申请网格参数空间
int Cube::initParamIfNeed()
{
    if (lats == nullptr)
    {
        lats = new double [nLat];
        lons = new double [nLon];
        depths = new double [nDepth];
        return 1;
    }
    return 0;
}

// 释放空间
int Cube::free()
{
    delete [] data;
    delete [] lons;
    delete [] lats;
    delete [] depths;

    initCube();
    return 1;
}

int Cube::simuParam(int idx)
{
    switch (idx)
    {
    case 1:
        simuParam1();
        break;
    case 2:
        simuParam2();
        break;
    case 3:
        simuParam3();
        break;
    }

    return 1;
}

int Cube::simuParam1()
{
    strcpy(fileFlag, "simu001");
    strcpy(dataInfo.name, "simu-Sound");
    dataInfo.type = 1;

    nLat = 299;
    nLon = 499;
    nDepth = 49;
    noneValue = NoneValue-1;
    initParamIfNeed();

    double minLat = 20.0;
    double stepLat = 0.01;
    for (int i=0; i<nLat; i++)
        lats[i] = minLat + i*stepLat;

    double minLon = 100.0;
    double stepLon = 0.01;
    for (int i=0; i<nLon; i++)
        lons[i] = minLon + i*stepLon;

    double minDept = 0.0;
    double stepDept = 10.0;
    for (int i=0; i<nDepth; i++)
        depths[i] = minDept + i*stepDept;

    return 1;
}

int Cube::simuParam2()
{

return 1;
}

int Cube::simuParam3()
{

    return 1;
}

int Cube::readFile(QString pathFile)
{
    this->pathFile = pathFile;

    QFileInfo fi(pathFile);
    file = fi.fileName();
    path = fi.absolutePath();

    QFile f(pathFile);
    if (!f.open(QIODevice::ReadOnly))
        return -1;

    int r1 = readParam(f);
    int r2 = readData(f);

    f.close();
    return 1;
}

int Cube::saveFile(QString file)
{
    QFile f(file);
    if (!f.open(QIODevice::WriteOnly))
        return -1;

    int r1 = saveParam(f);
    int r2 = saveData(f);

    f.close();
    return 1;
}

// 从数据文件中读取基本参数
int Cube::readParam(QFile & f)
{
    int headSize = PChar(&tailFlag)-PChar(this);
    qDebug() << "headSize=" << headSize;

    int r0 = f.read((char *)this, headSize);

    // 将来可以加验证标记信息，根据版本分别读取
    initParamIfNeed();
    int r1 = f.read((char *)lats, sizeof(double)*nLat);
    int r2 = f.read((char *)lons, sizeof(double)*nLon);
    int r3 = f.read((char *)depths, sizeof(double)*nDepth);

    lon0 = lons[0];
    lon1 = lons[nLon-1];
    lat0 = lats[0];
    lat1 = lats[nLat-1];
    depth = depths[nDepth-1];

    widthWE = distBetween2LB(lon0, lat0, lon1, lat0);
    widthNS = distBetween2LB(lon0, lat0, lon0, lat1);
    return 1;
}

// 保存参数到数据文件
int Cube::saveParam(QFile & f)
{
    size_t headSize = size_t(PChar(&tailFlag)-PChar(this));
    f.write((char *)this, headSize);
    f.write((char *)lats, sizeof(double)*nLat);
    f.write((char *)lons, sizeof(double)*nLon);
    f.write((char *)depths, sizeof(double)*nDepth);
    return 1;
}

int Cube::simuData(int idx)
{
    switch (idx)
    {
    case 1:
        simuData1();
        break;
    case 2:
        simuData2();
        break;
    case 3:
        simuData3();
        break;
    }

    return 1;
}

int Cube::simuData1()
{
    //
    if (data != nullptr)
        delete [] data;
    int dataSize = nLat * nLon * nDepth;
    data = new double [dataSize];

    int ijk = 0;
    for (int i=0; i<nLat; i++)
    {
        for (int j=0; j<nLon; j++)
        {
            for (int k=0; k<nDepth; k++)
            {
                double d = k*100 + j;
                data[ijk++] = d;
            }
        }
    }
    return 1;
}

int Cube::simuData2()
{

    return 1;
}

int Cube::simuData3()
{


    return 1;
}

// 增加一份数据集
int Cube::readData(QFile & f)
{

    //readParam(fp);
    int dataSize = nLat * nLon * nDepth;
    if (data != nullptr)
        delete [] data;
    data = new double [dataSize];
    f.read((char *)data, dataSize*sizeof(double));
    return 1;
}

// 保存数据到文件
int Cube::saveData(QFile & f)
{
    saveParam(f);
    int dataSize = nLat * nLon * nDepth;
    f.write((char *)data, dataSize*sizeof(double));
    return 1;
}


//QList <Cube *> cubes;
