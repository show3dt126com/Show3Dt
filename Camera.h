#ifndef CAMERA_H
#define CAMERA_H

/*
属于一个三维场景，
负责计算矩阵，
可以设置位置等参数，
响应场景界面的键盘按键进行移动旋转等
相机移动限制：
   俯视图：固定高度，仅滚动角可变，中心轴在以场区平面对角线
   为边长的正方形范围移动
   侧视图：固定与场区中心点的距离，仅偏航角可变
   中心轴在以场区平面对角线为宽，深度为高的矩形区域移动
视图开关：
   2D/3D视图开关：视图开关是一个2D/3D切换开关。
   2D状态下，仅显示切面2D图。
   3D状态下同时由一组切空参数控制显示。
   可以按照声场强度分级切空，也可以几何形状切空，如条状或分层切空
   俯视图的深度、侧视图的切面也是切空区的切空参数。
*/

#include <QObject>
#include <QTime>
#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <qopengl.h>
#include <QDebug>
#include <QtMath>

#include "Defines.h"


// 移动速度
const double SPEED = 1.0;
//
const double SENSITIVITY = 0.1;
// 透视角
const double ZOOM = 45.0;

class Camera
{
public:
    Camera();

    //
    void setViewPort(double w, double h);

    //返回lookat函数
    QMatrix4x4 getViewMatrix();
    // 设置摄像机在世界坐标中的位置
    void setCameraPos(QVector3D xyz);
    void setCameraPos(double x, double y, double z);
    // 设置摄像机的姿态角: 俯仰 偏转 滚动
    void setCameraAttitude(double pitch, double yaw, double roll);
    //
    void setWorldUp(QVector3D worldUp);

    // Perspective Project Options
    void setPerspectiveOptions(double nearPlane, double farPlane, double _zoom);
    // get perspective project
    QMatrix4x4 getPerspectiveMatrix();

    // Ortho Options
    void setOrthoOptions(double w, double h, double orthoNear, double orthoFar);
    void setOrthoOptions(double w, double orthoNear, double orthoFar);

    // get ortho project
    QMatrix4x4 getOrthoMatrix();
    QMatrix4x4 getOrthoMatrix(double size);
    // get text project
    QMatrix4x4 getTextMatrix(double w, double h, GLboolean isHorz=GL_TRUE);
    //
    QMatrix4x4 getScrnLineMatrix(double w=0, double h=0);


    double getZoom();

    //相应鼠标操作函数
    void processMouseMovement(double xoffset, double yoffset, GLboolean constraintPitch = true);
    //鼠标滚轮事件
    void processMouseScroll(double yoffset);
    //键盘处理事件函数
    void processKeyboard(ECameraMovement direction, double deltaTime);

    // 鼠标和键盘都可以水平的旋转场景
    void procRotateYaw(double dYaw);
    //
    void procRotatePitch(double dPitch);

    // 摄像机的位置
    QVector3D position;
    // 摄像机不进行roll操作
    QVector3D worldUp;

    // 摄像机的姿态矢量，操作摄像机移动时的单位矩阵，结合移动速度可得到移动的位移
    QVector3D front;
    QVector3D up;
    QVector3D right;

    // Eular Angles 不考虑 roll操作
    // 俯仰角，度
    double pitch;
    // 偏转角，度
    double yaw;
    // 滚动角，度
    double roll;

    //Camera options
    double movementSpeed;
    double mouseSensitivity;

    // 视口的参数
    double viewportW;
    double viewportH;

    // 透视投影的参数 Project Options
    // 投影角度，张角
    double zoom;
    double frustumNear;
    double frustumFar;

    // 正射投影的参数 Ortho Options
    double orthoW, orthoH;
    double orthoNear, orthoFar;

    // 鼠标滚动处理
    QTime timeForScrollProc;

private:
    void updateCameraVectors();
};

#endif // CAMERA_H
