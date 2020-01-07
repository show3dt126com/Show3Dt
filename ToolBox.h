#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QObject>
#include <QLineEdit>
#include "Defines.h"

// 下面的几个地图、地理方面的转换函数，是从网络上抄来学习的。

// 火星坐标系 (GCJ-02) 与百度坐标系 (BD-09) 的转换算法: 将 GCJ-02 坐标转换成 BD-09 坐标
void gcj02_to_bd09(double gg_lat, double gg_lon, double &bd_lat, double &bd_lon);
// 火星坐标系 (GCJ-02) 与百度坐标系 (BD-09) 的转换算法: 将 BD-09 坐标转换成GCJ-02 坐标
void bd09_to_gcj02(double bd_lat, double bd_lon, double &gg_lat, double &gg_lon);

// 场景像素坐标与地图坐标之间的转换函数
QPointF qgs2map(const QPointF & qgs, quint8 zoomLevel);
// 地图坐标与场景像素坐标之间的转换函数
QPointF map2qgs(const QPointF & map, quint8 zoomLevel);

// 地图坐标与地理坐标之间的转换函数
QPointF map211(const QPointF & map);
//地理坐标与地图坐标之间的转换函数
QPointF ll2map(const QPointF & geo);

// 计算2点之间的距离，单位米
double distBetween2LB(double l0, double b0, double l1, double b1);

// 根据中心点和大小范围，计算经纬度范围
void CLBWH2LBLB(double l, double b, double w, double h,
              double &l0, double &b0, double &l1, double &b1);

// 水经注地图批量下载配置文件的辅助构建
void makeBatchDownloadConfigFileForShuiJingZhuMap();
void makeBatchDownloadConfigFileForShuiJingZhuMapGrid();

// 两点距离
double distBetween2P(QPointF p1, QPointF p2);
// 点到直线的距离: b是直线的角度
double distPoint2Line(QPointF p1, double b, QPointF p3);

// 两线交点， p11+p12是线1， p21+p22是线2
int intersection(QPointF p11, QPointF p12, QPointF p21, QPointF p22, QPointF & j);

// 两条直线的交点：过中心点的a角的射线，过p3(cx,cz)点b角的直线，局限于半径r的圆内
// 角度从正北计算，顺时针为正
// 返回：交且在圆内，返回1，圆外返回0，不交返回-1
int join2lines(double a, double b, QPointF p3, double r, QPointF & j);

// 圆与直线的交点：圆的半径，，过p3(cx,cz)点b角的直线，
// 返回：交且在圆内，返回1，圆外返回0，不交返回-1
int joinLineCricle(double r, double b, QPointF p3, QPointF &j);

// 设置带小数点的字符串
void setEditValue(QLineEdit * e, double v);
// QLineEdit 中的数据，根据辅助速度按键是否按下的情况，修改为新的的数
void zoomInOut(QLineEdit * e, double step, double max, double min);


class ToolBox
{
public:
    ToolBox();
};

#endif // TOOLBOX_H
