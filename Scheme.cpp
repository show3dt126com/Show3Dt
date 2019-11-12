#include "Global.h"
#include <QtMath>
#include "Scheme.h"


CubeMan::CubeMan()
{

}

CubeMan::~ CubeMan()
{

}

int CubeMan::addCube(CubeModel * cubeModel)
{
    cubeModels.append(cubeModel);
    mergeCubeRange();
    return 1;
}

void  CubeMan::mergeCubeRange()
{
    if (cubeModels.count() == 0)
        return;
    CubeModel * cubeUse = cubeModels[0];
    mrgField = cubeUse->field;
    for (int i=1; i<cubeModels.count(); i++)
    {
        CubeModel * cubeModel = cubeModels[i];
        mrgField.merge(cubeModel->field);
    }
}

ModelMan::ModelMan()
{

}

void ModelMan::removeAllModels()
{
    //models.empty();
}

int ModelMan::addModel(BaseModel * model)
{
    models.append(model);
    return 1;
}

int ModelMan::prepareAllDarw()
{

    return 1;
}


Scheme::Scheme(QObject *parent) : QObject(parent)
{

}

int Scheme::saveToFile(QString file)
{
    QFile f(file);
    bool r = f.open(QIODevice::WriteOnly);
    if (! r)
        return -1;
    QDataStream fs(&f);

    fs << schemeInfo;

    int cubeFileN = cubeFiles.count();
    fs << cubeFileN;
    for (int i=0; i<cubeFileN; i++)
        fs << cubeFiles[i].toLatin1().data();

    fs.writeBytes((char *)&colorMap, sizeof(ColorMap));

    int ssFileN = soundSourceFiles.count();
    fs << ssFileN;
    for (int i=0; i<ssFileN; i++)
        fs << soundSourceFiles[i].toLatin1().data();

    int vpCount = viewPots.count();
    fs << vpCount;
    for (int i=0; i<vpCount; i++)
        fs.writeBytes((char *)&viewPots[i], sizeof(ViewPot));

    return 1;
}

int Scheme::loadFromFile(QString file)
{
    QFile f(file);
    bool r = f.open(QIODevice::ReadOnly);
    if (! r)
        return -1;
    QDataStream fs(&f);

    fs >> schemeInfo;

    int cubeFileN;
    fs >> cubeFileN;
    for (int i=0; i<cubeFileN; i++)
    {
        char * buf;
        fs >> buf;
        cubeFiles.append(buf);
        delete [] buf;
    }

    fs.readRawData((char *)&colorMap, sizeof(ColorMap));

    int ssFileN = soundSourceFiles.count();
    fs >> ssFileN;
    for (int i=0; i<ssFileN; i++)
    {
        char * buf;
        fs >> buf;
        soundSourceFiles.append(buf);
        delete [] buf;
    }

    int vpCount;
    fs >> vpCount;
    for (int i=0; i<vpCount; i++)
    {
        ViewPot vp;
        fs.readRawData((char *)&vp, sizeof(ViewPot));
        viewPots.append(vp);
    }

    return 1;
}

int Scheme::emptyContent()
{
    needSave = false;
    cubeFiles.clear();
    //ColorMap colorMap;
    soundSourceFiles.clear();
    viewPots.clear();

    return 1;
}

