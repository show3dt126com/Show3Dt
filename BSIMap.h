#ifndef BSMAP_H
#define BSMAP_H

/*
// 从水经注下载的大图，包括很多张单独的图，每个图有不同级别和区域
// 根据当前显示的区域和级别，自动选择合适的图
// 内存中仅打开一副地图
// 256*256 <-180,85.0511>   <180,-85.0511>

*/

#include "Defines.h"

// scale == 1, 每像素代表的米
#define SCALE1_MPP 156543.033928
// WebMercator的纬度书不含两极数据
#define WEB_MERCATOR_LAT_MAX    85.0511287798065467
// WebMercatot经度方面是完整的
#define MAX_MERCATOR_LON        180.0


// 一副地图的信息和文件名，
// Big Single Image Map
struct BSIMapTxt
{
    // 地图级别，参照WEB地图概念定义，1级256*256
    double scale;
    // meter per pixel; when scale==1空间分辨率：156543.033928米/像素
    double mpp;
    // 图形尺寸，像素
    int texW, texH;
    // 图像对应经纬度范围，左上角，右下角
    DVec3 texLB0, texLB1;
    // 图像对应 webMercator 坐标范围，texXY0左上角，texXY1右下角
    DVec3 texXY0, texXY1;
    // 图像文件，信息文件的名称，不含后缀，不含完整路径
    char file[64];
    // 计算任意点对应的图像坐标
    int lb2tex(DVec3 lb, double & texX, double & texY);
    // 计算图像坐标对应的经纬度
    int tex2lb(double texX, double texY, DVec3 & lb);

    //
    void testDemInfo();
};

// Big Single Image Map
class BSIMap
{
public:
    BSIMap();

    // 读取分级地图目录下面的信息文件，产生数据库
    void readAllPathBSIMapTxt();
    // 读取所有预先准备的地图数据库，合并到一起
    void readAllPathBSIMapDB();

    // 读取dem目录下面所有的信息文件
    void readPathAllBSIMapTxt(QString mapPath);
    // 根据读取的dem目录下面的信息，构数据库
    void makePathAllBSIMapTxtDB(QString txtDBFile);

    // 查找覆盖指定范围的最大分辨率图
    int findMaxCoverMap(double centL, double centB, double rr, QString & demFile);
    int findMaxCoverMap(double lon0, double lat0, double lon1, double lat1, QString & demFile);

    // 地图图像文件信息
    uint allTxtNum;
    BSIMapTxt * allBSIMTxt;

};

#endif // BSMAP_H
