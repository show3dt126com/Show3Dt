
#include <QMainWindow>
#include "Global.h"
#include "BBS.h"
#include "Scene.h"
#include "Shader.h"


QOpenGLFunctions_4_0_Core * C = nullptr;
Global * GD = nullptr;


Global::Global()
{
    scene = nullptr;
    winBox = nullptr;
    eyeBox = nullptr;
}

int Global::init()
{
    codec = QTextCodec::codecForName("GB18030");

    iniFile = new QSettings(mainPath + "/config/DFShow3D.ini", QSettings::IniFormat);
    dem.initDemData();
    sky.initSkyData();

    showBathy = false;
    showSky = false;
    showDem = false;
    showLine = true;

    return 1;
}

int Global::initSence()
{
    if (scene == nullptr)
    {
        scene = new Scene();
        scene->show();
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
    }

    eagleEye = new Eagle(eyeBox);
    sideEye = new SideEye(eyeBox);

    eyeBox->addWindow(eagleEye);
    eyeBox->addWindow(sideEye);

    eyeBox->arrangeWindows();
    eyeBox->show();

    return 1;
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

