
#include <QMainWindow>
#include <QObject>
#include "Global.h"
#include "BBS.h"
#include "Scene.h"
#include "Shader.h"
#include "ToolBox.h"


QOpenGLFunctions_4_0_Core * C = nullptr;
Global * GD = nullptr;

Global::Global()
{
    scene = nullptr;
    sceneWin = nullptr;
    winBox = nullptr;
    eyeBox = nullptr;
}

Global::~Global()
{
    //PythonIf::finalizePython();
}

int Global::init()
{
    codec = QTextCodec::codecForName("GB18030");

    iniFile = new QSettings(mainPath + "/config/DFShow3D.ini", QSettings::IniFormat);
    loadConfig();
    dem.initDemData();
    sky.initSkyData();
    mapMan.init();

    double l0, b0, l1, b1;
    CLBWH2LBLB(fieldRange.centerP.lon, fieldRange.centerP.lat,
               fieldRange.width, fieldRange.height,
               l0, b0, l1, b1);
    //dem.loadGEBCO1DNetCDFArea(G.pathFileOfGEBCO, l0, b0, l1, b1);

    showBathy = false;
    showSky = false;
    showDem = false;
    showLine = true;

    viewPot.viewType = EVT_Down;
    viewPot.dimMode = EDM_2D;

    //fieldRange

    PythonIf::initPython();

    return 1;
}

void Global::loadConfig()
{
    // 与最近打开声场区相关的
    QSettings & settings = * G.iniFile;
    settings.beginGroup("LastFieldRange");
    fieldRange.centerP.lon = settings.value("lon", 115).toDouble();
    fieldRange.centerP.lat = settings.value("lat", 21).toDouble();
    fieldRange.centerP.height = 0;
    fieldRange.width = settings.value("width", 300).toDouble();
    fieldRange.height = settings.value("height", 200).toDouble();
    fieldRange.depth = settings.value("depth", 30).toDouble();
    settings.endGroup();

    // 下面的参数需要手工配置
    settings.beginGroup("System");
    G.pathFileOfGEBCO = settings.value("pathFileOfGEBCO", "").toString();
    G.pathToDemFile = settings.value("pathToDemFile", "D:\\ShuiJingZhu-GoogleMap").toString();
    qDebug() << "G.pathToDemFile=" << G.pathToDemFile;
    settings.endGroup();

}

void Global::saveConfig()
{
    QSettings & settings = * G.iniFile;
    settings.beginGroup("LastFieldRange");
    settings.setValue("lon", fieldRange.centerP.lon);
    settings.setValue("lat", fieldRange.centerP.lat);
    settings.setValue("width", fieldRange.width);
    settings.setValue("height", fieldRange.height);
    settings.setValue("depth", fieldRange.depth);
    settings.endGroup();
}

int Global::initSence()
{
    if (sceneWin == nullptr)
    {
        sceneWin = new SceneWin();
        scene = sceneWin->scene;
        sceneWin->show();
    }
    return 1;
}

int Global::initWinBox()
{
    if (winBox == nullptr)
    {
        winBox = new WinBox(nullptr, "控制小窗口");
        winBox->setDockNestingEnabled(true);
        winBox->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks);
    }

    lookDownCtrl = new LookDownControl(winBox);
    lookIso3DCmr = new LookIso3DCamera(winBox);
    lookIso3DCtrl = new LookIso3DControl(winBox);
    lookIso3DVal = new LookIso3DValue(winBox);
    lookSideCtrl = new LookSideControl(winBox);

    winBox->addWindow(lookDownCtrl);
    winBox->addWindow(lookIso3DCmr);
    winBox->addWindow(lookIso3DVal);
    winBox->addWindow(lookSideCtrl);
    winBox->addWindow(lookIso3DCtrl);

    winBox->arrangeWindows();
    winBox->show();

    if (eyeBox == nullptr)
    {
        eyeBox = new WinBox(nullptr, "全局图小窗口");
        eyeBox->setDockNestingEnabled(true);
        eyeBox->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks);
        //by sxd
        eyeBox->installEventFilter(nullptr);
        eyeBox->setFocusPolicy(Qt::ClickFocus);
    }

    eagleEye = new Eagle(eyeBox);
    sideEye = new SideEye(eyeBox);

    eyeBox->addWindow(eagleEye);
    eyeBox->addWindow(sideEye);

    eyeBox->arrangeWindows();
    eyeBox->show();
    connect(eagleEye,SIGNAL(drawMyline(double,double)),sideEye,SLOT(dodrawLine(double,double)));

    return 1;
}

void Global::setField(Field field)
{
    this->field = field;
    fieldRange.setField(field);

    // 侧视图，缺省值，相机向北，位域场区正南。
    strcpy(defaultViewPotH.note, "缺省水平视图");
    defaultViewPotH.viewType = EVT_Side;
    defaultViewPotH.dimMode = EDM_2D;
    defaultViewPotH.cubeIndex = 0;
    defaultViewPotH.zoomDepth = 1;
    // 正南方
    defaultViewPotH.cameraPar.x = 0;
    defaultViewPotH.cameraPar.y = -fieldRange.depth/2;
    defaultViewPotH.cameraPar.z = fieldRange.height + fieldRange.outRadius;
    // 相机向北
    defaultViewPotH.cameraPar.yaw = 0;
    defaultViewPotH.cameraPar.pitch = 0;
    defaultViewPotH.cameraPar.roll = 0;
    defaultViewPotH.cameraPar.viewPercent = 100;

    // 俯视图，相机在正上方，
    strcpy(defaultViewPotV.note, "缺省垂直视图");
    defaultViewPotV.viewType = EVT_Down;
    defaultViewPotV.dimMode = EDM_2D;
    defaultViewPotV.cubeIndex = 0;
    defaultViewPotV.zoomDepth = 1;
    //
    defaultViewPotV.cameraPar.x = 0;
    defaultViewPotV.cameraPar.y = fieldRange.aboveField;
    defaultViewPotV.cameraPar.z = 0;
    // 相机
    defaultViewPotV.cameraPar.yaw = 0;
    defaultViewPotV.cameraPar.pitch = 0;
    defaultViewPotV.cameraPar.roll = 0;
    defaultViewPotV.cameraPar.viewPercent = 100;

    // 场区变化之后，最近视点设置为缺省值
    lastViewPotH = defaultViewPotH;
    lastViewPotV = defaultViewPotV;
}

void Global::setViewType(EViewType viewType)
{
    if (viewPot.viewType == viewType)
        return;
    if (viewPot.viewType == EVT_Down)
    {
        lastViewPotV = viewPot;
        viewPot = lastViewPotH;
    }
    else
    {
        lastViewPotH = viewPot;
        viewPot = lastViewPotV;
    }
}

QString Global::parseShaderCode(QString name)
{
    QStringList listShader = shaderAll.split("<<<");
    for (int i=0; i<listShader.count(); i++)
    {
        int index = listShader[i].indexOf(name);
        if (index >= 0)
        {
            //qDebug() << name << listShader[i+1];
            return listShader[i+1];
        }
    }
    return "";
}

void Global::makeShader(Shader & shader, QString name)
{
    QString vert = parseShaderCode(name + ".vert");
    QString frag = parseShaderCode(name + ".frag");
    QString geom = parseShaderCode(name + ".geom");

    if (shader.init(vert, frag, geom) == 1)
        qDebug() << "Shader init OK: " << name;
}

int Global::initGL()
{
    QFile shaderAllFile(":/Shader.all");
    shaderAllFile.open(QIODevice::ReadOnly);
    QTextStream in(&shaderAllFile);
    shaderAll = in.readAll();
    shaderAllFile.close();

    //qDebug() << shaderAll;
    //QStringList listShader = shaderAll.split("<<<");
    //for (int i=0; i<listShader.count(); i++)
    //    qDebug() << i << listShader[i];

    makeShader(shaderSky, "Sky");
    makeShader(shaderDem, "Dem");
    makeShader(shaderLine, "Line");
    makeShader(shaderScrnLine, "ScrnLine");

    makeShader(shaderText, "Text");
    //    makeShader(shaderCube, "Cube");

    texter.initTexter();
    return 1;
}

/*
void Global::prepareColorMapColors()
{
    colorMapColorNum = 0;
    mapperDbMin = colorMap.colorKeys[0].v;
    mapperDbMax = mapperDbMin;
    for (int i=0; i<ColorMapKeyN_MAX; i++)
    {
        ColorKey & key = colorMap.colorKeys[i];
        if (key.v >= ColorMapV_MAX)
            break;
        QColor c = key.color;
        mapperDbMax = key.v;
        colorMapV4D[colorMapColorNum] = QVector4D(c.red()/255.0f, c.green()/255.0f,
                    c.blue()/255.0f, key.v);
        colorMapColorNum ++;
    }
}
*/


