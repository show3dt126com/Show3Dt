#ifndef MAPMAN_H
#define MAPMAN_H

/*
  从水经注下载的地图，采用单幅大地图不拼接的模式，每个单幅地图
  满足当前地图显示和纹理的需求。
  本对象主要建立地图索引，根据显示位置和大小参数，选择最合适的地图
*/
#include <QString>
#include <QList>
#include "Defines.h"

class MapMan
{
public:
    MapMan();
    // 根据 Global 中的目录参数初始化，建立索引
    int init();
    // 输入的参数是实际
    QString findFitMap(double l0, double b0, double l1, double b1);

    // 所有的 纹理文件的信息
    QList <DemInfo> demList;
};

#endif // MAPMAN_H
