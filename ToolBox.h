#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QObject>
#include <QLineEdit>
#include "Defines.h"

//
void gcj2bd(double gg_lat, double gg_lon, double &bd_lat, double &bd_lon);
void bd2gcj(double bd_lat, double bd_lon, double &gg_lat, double &gg_lon);

//
QPointF qgs2map(const QPointF & qgs, quint8 zoomLevel);
QPointF map2qgs(const QPointF & map, quint8 zoomLevel);

// 地图坐标与地理坐标之间的转换函数
QPointF map211(const QPointF & map);
//地理坐标与地图坐标之间的转换函数
QPointF ll2map(const QPointF & geo);

// 计算2点之间的距离，单位米
double distBetween2LB(double l0, double b0, double l1, double b1);

// 水经注地图批量下载配置文件的辅助构建
void makeBatchDownloadConfigFileForShuiJingZhuMap();

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

void setEditValue(QLineEdit * e, double v);
void zoomInOut(QLineEdit * e, double step, double max, double min);


class ToolBox
{
public:
    ToolBox();
};

#endif // TOOLBOX_H
