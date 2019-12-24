#ifndef SCENE_H
#define SCENE_H

#include "stdio.h"

#include <QOpenGLWidget>
#include <QObject>
#include "Defines.h"
#include "Cube.h"
#include "BBS.h"


// QOpenGLWidget 窗口，负责3D显示，响应鼠标操作键盘操作

// 管理按键按下状态的辅助对象
struct CameraMoveKey
{
    ECameraMovement move;
    int key;
    GLboolean pressed;
};

#define CameraMoveKeyMAX  100

// 专注于3D窗口中可以操作相机的鼠标事件、键盘按键的管理
class CameraMoveKeyMan
{
public:
    CameraMoveKeyMan();
    int num;
    static CameraMoveKey moveKeys[CameraMoveKeyMAX];
    void procKeyPressed(int key);
    void procKeyReleased(int key);
    GLboolean isDoingMove(ECameraMovement move);
};


// 3D窗口，显示当前视图的所有模型，
// 响应在3D窗口中的鼠标动作、键盘动作，反射成相机位置的变化
// 响应鼠标点击动作，拾取鼠标位置的数据
class Scene : public QOpenGLWidget, public BBSMessageProc
{
    Q_OBJECT
public:
    Scene(QWidget* parent = 0);
    ~ Scene();

    int init();

protected:

    //键盘按下事件
    void keyPressEvent(QKeyEvent* event) override;
    //键盘释放事件
    void keyReleaseEvent(QKeyEvent* event) override;

    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;
    //滚轮事件
    void wheelEvent(QWheelEvent* event) override;
    //鼠标按下事件
    void mousePressEvent(QMouseEvent* event) override;
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent* event) override;

    // 当鼠标移动或点击时拾取数据
    void pickPointerData(QMouseEvent* event);

    //
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    // bbsUser 当接收到 G.BBS 信号时回调下面的函数
    void procBBSMessage(BBSMessage bbsMsg) override;
    BBSBase bbsUser;

    void drawRuler();

private:
    //摄像机键盘处理函数，
    void processInput(GLfloat dt);
    //opengl 更新函数
    void updateGL();

    // 3D窗口线条
    int addScrLine(VertexPC vs, VertexPC ve);
    int drawScrLines();

public:
    // 窗口区域，大小变化时更新
    QRect viewRect;
    // 鼠标位置
    GLint mposx, mposy;
    QVector3D mpos0, mpos1;

public:
    // 鼠标移动拾取世界坐标点位和对应声场数据的模式
    EPointerPickMode pickMode;
    bool showPickData;

private:
    // 内部状态存储
    bool glInited;

    //获取键盘按键，实现多键触控
    CameraMoveKeyMan cameraMoveKeyMan;

    // 处理鼠标按下移动对场景的变化
    GLboolean isFirstMouse;
    GLboolean isLeftMousePress;
    GLint lastX;
    GLint lastY;

    // 根据时间差处理按键按下的场景变化率
    QTime timeForKeyPressProc;
    double deltaTime;
    double lastFrame;//上一帧

    // 一些视窗线条
    int scrLineVPCNum;
    VertexPC scrLineVPC[1000];
    // 文本对象的矩阵
    QMatrix4x4 textProjVert, textProjHorz;


    // 坐标系
    int coordLineNum;
    VertexPC coordLines[100];

};

#endif // SCENE_H
