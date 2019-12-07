#include "ToolBox.h"
#include <QtMath>
#include <QPoint>
#include <QApplication>
#include "WebMercator.h"

ToolBox::ToolBox()
{

}

// GCJ-02 和 BD-09
const double x_pi = 3.14159265358979324 * 3000.0 / 180.0;

void gcj2bd(double gg_lat, double gg_lon, double &bd_lat, double &bd_lon)
{
    double x = gg_lon, y = gg_lat;
    double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
    double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
    bd_lon = z * cos(theta) + 0.0065;
    bd_lat = z * sin(theta) + 0.006;
}

void bd2gcj(double bd_lat, double bd_lon, double &gg_lat, double &gg_lon)
{
    double x = bd_lon - 0.0065, y = bd_lat - 0.006;
    double z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_pi);
    double theta = atan2(y, x) - 0.000003 * cos(x * x_pi);
    gg_lon = z * cos(theta);
    gg_lat = z * sin(theta);
}


#define MapLenght(level)   256*pow(2,(level))
#define WMCT256_P	20037508.342789
//#define PI	3.141592653589793
#define WMCT256_RES(x) ((WMCT256_P * 2)/(256 * (2<<((x)-1))))

// 场景像素坐标与地图坐标之间的转换函数
QPointF qgs2map(const QPointF & qgs, quint8 zoomLevel)
{
    double resolution = WMCT256_RES(zoomLevel);
    qreal x = (qreal)(WMCT256_P + (qgs.x()*resolution));
    qreal y = (qreal)(WMCT256_P - (qgs.y()*resolution));
    return QPointF(x, y);
}

// 地图坐标与地理坐标之间的转换函数
QPointF map211(const QPointF & map)
{
    qreal x = (qreal)(map.x() / WMCT256_P * 180);
    qreal y = (qreal)(map.y() / WMCT256_P * 180);
    y = (180 / PI)*(2 * atan(exp(y*PI / 180) - PI / 2));
    return QPointF(x, y);
}

// 地图坐标与场景像素坐标之间的转换函数
QPointF map2qgs(const QPointF & map, quint8 zoomLevel)
{
    double resolution = WMCT256_RES(zoomLevel);
    qreal x = (qreal)((WMCT256_P + map.x()) / resolution);
    qreal y = (qreal)((WMCT256_P - map.y()) / resolution);
    return QPointF(x, y);
}

//地理坐标与地图坐标之间的转换函数
QPointF ll2map(const QPointF & geo)
{
    qreal x = (qreal)(geo.x() *WMCT256_P /180);
    qreal y = (qreal)(log(tan(((90 + geo.y())*PI/360))) / (PI/180));
    y = y*WMCT256_P / 180;
    return QPointF(x, y);
}

double distBetween2LB(double l0, double b0, double l1, double b1)
{
    double distance = acos(sin(b0*D2R)*sin(b1*D2R)
           + cos(b0*D2R)*cos(b1*D2R)*cos((l0-l1)*D2R)) * EARTH_R;
    return distance;
}

// makeBatchDownloadConfigFileForShuiJingZhuMap
// 在标准 Web地图2分块基础上，每个分割处增加一个块，但0度和360的位置数据
// 能保证在任何位置，任何级别，都能有有一块地图覆盖
// L05:1
// L06:3*3 重叠
// L07:7*7
// L08:15*15
// L09:31*31
// L10:63*63
void makeBatchDownloadConfigFileForShuiJingZhuMap()
{
    double l0, b0, l1, b1;
    int s0 = 5, s1=12;
    FILE * fp = fopen("D:/ShuiJingZhuBatch.txt", "wt");

    DVec3 P;
    P.x = 0;
    //P.y = 85.0511287798065467;
    P.y = 84.0;
    P.z = 0;
    DVec3 M = WebMercator::lonLat2WebMercatorD(P);
    double MY = M.y;
    double dlon = 360.0;
    int t = 1;

    int f10i = 0;
    for (int s=5; s<=10; s++)
    {

        for (int x=0; x<t; x++)
        {

            DVec3 MXY;
            MXY.x = 0;
            MXY.y = -M.y + MY * x;
            DVec3 latMin = WebMercator::webMercator2lonLatD(MXY);
            MXY.x = 0;
            MXY.y = MXY.y + MY*2;
            DVec3 latMax = WebMercator::webMercator2lonLatD(MXY);

            for (int y=0; y<t; y++)
            {
                if (s == 10 && x%7 == 0 && y == 0)
                {
                    fclose(fp);
                    char fn[300];
                    sprintf(fn, "D:/ShuiJingZhuBatch-L10-%d.txt", f10i++);
                    fp = fopen(fn, "wt");
                }
                double lonMin = -180 + y*dlon/2.0;
                double lonMax = lonMin + dlon;
                fprintf(fp, "World-L%02d-%03d-%03d|%15.13lf,%15.13lf|%15.13lf,%15.13lf|%d\n",
                        s, x,y, lonMin, latMin.y, lonMax, latMax.y, s);
            }
            fprintf(fp, "\n");
        }
        MY = MY / 2.0;
        dlon = dlon / 2.0;
        t = 2*t+1;
    }
    fclose(fp);
}

double distBetween2P(QPointF p1, QPointF p2)
{
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return sqrt(dx*dx + dy*dy);
}

double distPoint2Line(QPointF p1, double b, QPointF p3)
{
    // Ax+y+C=0，点 P 的坐标为（Xp，Yp），则点 P 到直线 L 的距离为：
    // abs(A*Xp + Yp + C) / sqrt(A*A + 1)
    // tan(89.999) = 57295.78
    double A, C;
    if (abs(b-90) > 1E-4 && abs(b-270) > 1E-4)
    {
        A = -tan(b*D2R);
        C = -p3.y() -p3.x()*A;
        return abs(A*p1.x() + p1.y() + C)/ sqrt(A*A + 1);
    }
    else
        return abs(p1.y() - p3.y());
}

// p11+p12是线， p21+p22是线
int intersection(QPointF p11, QPointF p12, QPointF p21, QPointF p22, QPointF & j)
{
    double p1x = p11.x();
    double p1y = p11.y();
    double p2x = p21.x();
    double p2y = p21.y();
    double p3x = p12.x();
    double p3y = p12.y();
    double p4x = p22.x();
    double p4y = p22.y();

    double xm = p2x*p1y - p4x*p1y - p1x*p2y + p3x*p2y - p2x*p3y + p4x*p3y + p1x*p4y - p3x*p4y;
    double ym = p2y*p1x - p4y*p1x - p1y*p2x + p3y*p2x - p2y*p3x + p4y*p3x + p1y*p4x - p3y*p4x;

    if (abs(xm) < 1E-11 || abs(ym) < 1E-11)
        return -1;

    double xz = p2x*p3x*p1y - p3x*p4x*p1y - p1x*p4x*p2y + p3x*p4x*p2y
                - p1x*p2x*p3y + p1x*p4x*p3y + p1x*p2x*p4y - p2x*p3x*p4y;
    double yz = p2y*p3y*p1x - p3y*p4y*p1x - p1y*p4y*p2x + p3y*p4y*p2x
                - p1y*p2y*p3x + p1y*p4y*p3x + p1y*p2y*p4x - p2y*p3y*p4x;

    j.setX(xz / xm);
    j.setY(yz / ym);
    return 1;
}


int join2lines(double a, double b, QPointF p3, double r, QPointF & j)
{
    QPointF p1(0,0);
    QPointF p2(r*sin(a*D2R), -r*cos(a*D2R));
    QPointF p4(p3.x()+10000*sin(b*D2R), p3.y()-10000*cos(b*D2R));
    int s = intersection(p1, p2, p3, p4, j);
    if (s != 1)
        return s;

    double d = distBetween2P(p1, j);
    if (d > r)
        return 0;

    return s;
}

int joinLineCricle(double r, double b, QPointF p3, QPointF &j)
{


    return 1;
}

void zoomInOut(QLineEdit * e, double step, double max, double min)
{
    double co = 0.0;
    Qt::KeyboardModifiers modifiers = QApplication::queryKeyboardModifiers();
    if (modifiers == (Qt::ControlModifier | Qt::ShiftModifier))
        co = 0.25;
    else if (modifiers == Qt::ControlModifier)
        co = 0.05;
    else if (modifiers == Qt::ShiftModifier)
        co = 0.01;

    double v = e->text().toDouble();
    double newV = v;

    //step *= co;
    if (co > 0.0)
    {
        if (step > 0)
            co = 1.0 + co;
        else
            co = 1.0 - co;
        newV = v * co;
    }
    else
        newV = v + step;

    if (newV > max)
        newV = max;
    if (newV < min)
        newV = min;
    e->setText(QString::number(newV, 'f', 1));
}

void setEditValue(QLineEdit * e, double v)
{
    e->setText(QString::number(v, 'f', 1));
}
