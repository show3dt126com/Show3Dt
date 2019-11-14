#include "Global.h"
#include <QtMath>
#include "Scheme.h"


CubeModelMan::CubeModelMan()
{

}

CubeModelMan::~ CubeModelMan()
{

}

int CubeModelMan::addCubeModel(CubeModel * cubeModel)
{
    cubeModels.append(cubeModel);
    mergeCubeRange();
    return 1;
}

int CubeModelMan::removeCubeModel(CubeModel * cubeModel)
{
    cubeModels.removeOne(cubeModel);
}

void  CubeModelMan::mergeCubeRange()
{
    if (cubeModels.count() == 0)
        return;
    CubeModel * cubeUse = cubeModels[0];
    mergedField = cubeUse->field;
    for (int i=1; i<cubeModels.count(); i++)
    {
        CubeModel * cubeModel = cubeModels[i];
        mergedField.merge(cubeModel->field);
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
    foreach (BaseModel * m, models)
    {
        if (m == model)
            return 0;
    }
    models.append(model);
    return 1;
}

int ModelMan::removeModel(BaseModel * model)
{
    return models.removeOne(model);
}

int ModelMan::prepareAllDarw()
{
    return 1;
}

Scheme::Scheme(QObject *parent) : QObject(parent)
{
    this->schemeInfo = "Test Scheme";
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
        fs << cubeFiles[i];

    fs.writeBytes((char *)&colorMap, sizeof(ColorMap));

    int ssFileN = soundSourceFiles.count();
    fs << ssFileN;
    for (int i=0; i<ssFileN; i++)
        fs << soundSourceFiles[i];

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
        QString f;
        fs >> f;
        cubeFiles.append(f);
    }

    fs.readRawData((char *)&colorMap, sizeof(ColorMap));

    int ssFileN = soundSourceFiles.count();
    fs >> ssFileN;
    for (int i=0; i<ssFileN; i++)
    {
        QString f;
        fs >> f;
        soundSourceFiles.append(f);
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

