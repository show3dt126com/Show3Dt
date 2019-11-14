#ifndef EAGLE_H
#define EAGLE_H

/*
  鹰眼
*/

#include <QWidget>
#include <QTime>
#include <QObject>
#include <QPointF>
#include "BSIMap.h"

// 显示地图的窗口，主要作为鹰眼图用，叠加水声场区域示意图，摄像机方位方向示意图
// 本对象管理需要的纹理地图，从GOOGLE地图获取无偏移图像数据，采用大幅地图方式保存
// 因此也作为周边地理环境显示需要的纹理书，每次根据水声场位置选择合适的图像，
// 因此本对象也代为辅助选择合适的图像

class Eagle : public QWidget
{
    Q_OBJECT
public:
    Eagle(QWidget *parent = nullptr);
    // 根据全局对象 BSIMap bsiMap 以及保存的
    void init();
    void loadConfig();
    void saveConfig();

    void checkOpenFitDem();
    //两点确定一条直线方程
    void getLineFunc(double lon1, double lan1, double lon2, double lan2);
    //像素点转换为经纬度
    void getLB(double *, double *);
    // dem 图像文件的路径
    QString mapPath;
    // 当前打开的图像
    BSIMapTxt * loadedMapTxt;
    int loadedMap;
    QImage * image;

    bool keep;
    // 与地图级别对应，1...
    double scale;
    // 当前显示的中心点坐标
    double centL, centB;

    // 鼠标的移动位置
    double mouseL, mouseB;
    // 鼠标滚动处理
    QTime timeForScrollProc;
    bool isLeftMousePress;
    bool isFirstMouse;
    int lastX, lastY;
    double lastLat, lastLon;
    QPointF startPoint;
    bool isInited;

    double jd ;     //经度系数
    double wd ;     //纬度系数

    double coefficient;  //系数
    double conf;    //常数
protected:
    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;
    //滚轮事件
    void wheelEvent(QWheelEvent* event) override;
    //鼠标按下事件
    void mousePressEvent(QMouseEvent* event) override;
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent* event) override;
    //键盘事件
    void keyPressEvent(QKeyEvent *event) override;
    // 绘制
    void paintEvent(QPaintEvent *)  override;

signals:
   void drawMyline(double,double);
public slots:
    void onBBSMessage(int msg);
};

#endif // EAGLE_H
