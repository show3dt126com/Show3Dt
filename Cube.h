#ifndef CUBE_H
#define CUBE_H

/*
  长方体数据
*/


/*
 *
  声源等信息的数据文件格式：
  文件头："SR-I101"
  声源数据： N  SoundSource[N] 包含多个声源数据
  ...... 其它


  网格数据文件的格式：
  用于存储长方体形式的网格化数据，可用于各种网格化数据。
  文件头部：
    标识："SR-G101":8B
    纬度格数：nLat:4B
    经度格数：nLon:4B
    深度格数：nDept:4B
    数据集的信息: DataInfo
  网格参数，从小到大排列：
    纬度格的具体纬度值：lats:8B*nLat
    经度格的具体经度值：lons:8B*nLon
    深度格的具体深度值：depts:8B*nDept
  数据部分：按照纬度、经度、深度的顺序，保存网格点的数据
  每个文件保存一份数据集。
  SoundCube 对象可用于生成和读取长方体网格化数据。一个SoundCube对象可管多个。
*/

#include <QObject>
#include <stdio.h>
#include <QFile>

//#pragma pack(push, 1)

// 声源位置频率特性参数
class SoundSource
{
public:
    // 声源位置纬度
    double lat;
    // 声源位置经度
    double lon;
    // 声源位置深度
    double dept;
    // 声源频率
    int freq;
    // 声场特性参数
    double z[2];
    double cp[2];
    double rhos[2];
    double ap[2];

    // 海底特征参数
    double speed;
    double rho;
    double atten;

    // 声源位置声速
    int num;
    // 不同深度
    double * depths;
    // 对应声速
    double * speeds;
};

// 数据集的信息，一组长方体数据称为一个数据集
class DataInfo
{
public:
    // 数据集名称，字符串，
    char name[32];
    // 数据集类型，1：声压   2：温度   3：盐度   4：声速  ......
    int type;
};

// 定义数据场网格信息，可用于1个相同网格的数据集
class Cube
{
public:
    // 构造函数
    Cube();
    // 初始化函数
    int initCube();
    // 从文件中读取数据
    int readFile(QString pathFile);
    //
    int saveFile(QString pathFile);
    // 从数据文件中读取基本参数
    int readParam(QFile & f);
    // 保存参数到数据文件
    int saveParam(QFile & f);
    // 模拟数，先模拟参数
    int simuParam(int idx);
    int simuParam1();
    int simuParam2();
    int simuParam3();

    // 申请网格参数空间
    int initParamIfNeed();
    // 增加一份数据集
    int readData(QFile & f);
    // 保存数据到文件
    int saveData(QFile & f);
    // 产生模拟数，模拟从文件中读取一个 Cube数据
    int simuData(int idx);
    int simuData1();
    int simuData2();
    int simuData3();
    // 释放空间
    int free();

    // 文件头标记，识别和版本用
    char fileFlag[8];
    // 数据集的信息
    DataInfo dataInfo;
    // 网格的纬度格数
    int nLat;
    // 网格的经度格数
    int nLon;
    // 网格的深度格数
    int nDepth;
    // 无数据标记，比如-1，有数据的值大于 noneValue
    double noneValue;

    // 文件头的信息都定义在此变量前面
    int tailFlag;

    // 数据文件路径名
    QString pathFile;
    QString file;
    QString path;

    double lon0, lon1;
    double lat0, lat1;
    // (m)
    double depth;
    // (m)
    double widthWE, widthNS;

    // 网格纬度点，实际纬度数
    double * lats;
    // 网格经度点，实际经度数
    double * lons;
    // 网格深度点，实际深度数
    double * depths;
    // 数据 按照纬度、经度、深度顺序排列，经纬度从小到大，深度由上到下。
    // 理解：先按深度相同的分层数据；同层，先按经度不变的经线；经线上，纬度点由小到大。
    double * data;
};

//#pragma pack(pop)

#endif // CUBE_H

