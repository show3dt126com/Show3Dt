#ifndef GLOBAL_H
#define GLOBAL_H

#include <QOpenGLFunctions_4_0_Core>
#include <QTextCodec>
#include <QObject>
#include <QList>
#include <QSettings>

#include "Defines.h"
#include "Scene.h"
#include "Shader.h"
#include "Texter.h"
#include "BBS.h"
#include "Sky.h"
#include "Dem.h"
#include "Cube.h"
#include "BaseModel.h"
#include "Scheme.h"
#include "BSIMap.h"
//#include "MainWindow.h"
#include "Camera.h"
#include "WinBox.h"
#include "Test.h"
#include "EagleEye.h"
#include "SideEye.h"

#include "LookDownControl.h"
#include "LookIso3DCamera.h"
#include "LookIso3DControl.h"
#include "LookIso3DValue.h"
#include "LookSideControl.h"

class MainWindow;

class Global
{
public:
    Global();

    int init();
    int initGL();
    int initSence();
    int initWinBox();

    // 从 shaderAll 中解析指定名称的代码，返回
    QString parseShaderCode(QString name);
    // 构建指定名称的Shader
    void makeShader(Shader & shader, QString name);

    // 程序开始路径，含最后的"/"
    QString mainPath;
    QTextCodec *codec;

    // 方便访问主窗口
    MainWindow * mw;
    WinBox * winBox;
    WinBox * eyeBox;

    // 初始化配置文件
    QSettings * iniFile;

    // shaders 源码，合并在一个文件中
    QString shaderAll;

    // 所有预定义的颜色 映射表
    ColorMap colorMaps[ColorMap_MAX];
    // 当前正在使用的，全局缺省的颜色映射表
    ColorMap colorMap;

    // 程序启动时创建一个新的空的，可以打开已有的
    Scheme scheme;

    // 当前打开声场范围, lat0 < lat1, lon0 < lon1, depth >0
    Field openedField;

    // 数据场区域，要切去的部分的参数
    CutField cutField;

    // 原始数据+颜色映射表对，
    CubeMan cubeMan;
    // 各种需要显示的模型
    ModelMan modelMan;

    // 管理地图
    BSIMap bsiMap;

    // 消息转发器
    BBS bbs;

    // 相机
    Camera camera;

    // 场景窗口
    Scene * scene;

    // 当前显示的视图类型
    EViewType viewType;

    LookDownControl * lookDownCtrl;
    LookIso3DCamera * lookIso3DCmr;
    LookIso3DControl * lookIso3DCtrl;
    LookIso3DValue  * lookIso3DVal;
    LookSideControl*  lookSideCtrl;

    // 俯视全局图
    Eagle * eagleEye;
    // 侧视全局图
    SideEye * sideEye;

    // 周边环境
    Dem dem;
    Sky sky;

    Shader shaderCube;
    Shader shaderDem;
    Shader shaderSky;
    Shader shaderSpirit;

    //
    Shader shaderLine;
    Shader shaderScrnLine;
    Shader shaderText;
    Texter texter;

    // 海底
    bool showBathy;
    // 半球天空
    bool showSky;
    // 周边环境
    bool showDem;
    // 直线
    bool showLine;

};

extern QOpenGLFunctions_4_0_Core * C;
extern Global * GD;

extern Test T;

#define G (*GD)

#endif // GLOBAL_H
