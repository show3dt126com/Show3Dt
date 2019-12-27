#ifndef GLOBAL_H
#define GLOBAL_H

#include <QOpenGLFunctions_4_0_Core>
#include <QTextCodec>
#include <QObject>
#include <QList>
#include <QSettings>

#include "PythonIf.h"
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
#include "SceneWin.h"

#include "LookDownControl.h"
#include "LookIso3DCamera.h"
#include "LookIso3DControl.h"
#include "LookIso3DValue.h"
#include "LookSideControl.h"

class MainWindow;

class Global :public QObject
{
    Q_OBJECT
public:
    Global();
    virtual ~Global();

    int init();
    int initGL();
    int initSence();
    int initWinBox();
    void setField(Field field);
    void setViewType(EViewType viewType);

    // 保存读取缺省参数，或者最近参数
    void loadConfig();
    void saveConfig();

    // 从 shaderAll 中解析指定名称的代码，返回
    QString parseShaderCode(QString name);
    // 构建指定名称的Shader
    void makeShader(Shader & shader, QString name);

    // 程序开始路径，含最后的"/"
    QString mainPath;
    QTextCodec *codec;

    // 地图和纹理数据文件的路径
    // F:\\Software\\Gis\\GEBCO\\GEBCO_2014_1D_GRID_1.nc
    QString pathFileOfGEBCO;
    // F:\\DemFiles
    QString pathToDemFile;

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

    // CutField 的 Field 是 CubeModelMan 的所有Cube 的
    CubeModel * cutFiledUseCubeModel;

    // 原始数据+颜色映射表对，d=当前的工程打开了一些Cube，需要列表显示，选择某一个显示
    CubeModelMan cubeModelMan;
    // 各种需要显示的模型，包含天空、周边环境等
    ModelMan modelMan;

    // 管理地图
    BSIMap bsiMap;

    // 消息转发器
    BBS bbs;

    // 相机
    Camera camera;

    // 场景窗口
    Scene * scene;
    SceneWin * sceneWin;

    // 当前的场区以及相关设置
    // 当前的场区
    Field field;
    // 根据当前场区的参数计算的固定参数
    FieldRange fieldRange;
    // 根据当前场区参数计算的缺省视图参数，2个方向
    ViewPot defaultViewPotH;
    ViewPot defaultViewPotV;

    // 当前的视图参数
    ViewPot viewPot;
    // 最近的视图参数，切换时恢复到 viewPot
    ViewPot lastViewPotH;
    ViewPot lastViewPotV;

    // 参数设置对话框界面
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
    Shader shaderDem;
    Shader shaderSky;

    //
    Shader shaderCube;
    Shader shaderSpirit;

    //
    Shader shaderLine;
    Shader shaderScrnLine;

    //
    Shader shaderText;
    Texter texter;

    // 半球天空
    bool showSky;
    // 周边环境
    bool showDem;

    // 海底
    bool showBathy;
    // 直线
    bool showLine;

    double comjd1 ;      //计算后的经度
    double comwd1 ;      //计算后的纬度

    double comjd2 ;      //计算后的经度
    double comwd2 ;      //计算后的纬度
};

extern QOpenGLFunctions_4_0_Core * C;
extern Global * GD;

extern Test T;

#define G (*GD)

#endif // GLOBAL_H
