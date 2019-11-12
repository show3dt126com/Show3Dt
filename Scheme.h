#ifndef SCHEME_H
#define SCHEME_H

/*
  方案概念，包括若干Cube数据，对应的颜色映射，存储的视图动画等等
  预先配置，存储，以后打开

*/

#include <QObject>
#include "Defines.h"
#include "Cube.h"
//#include "Model.h"
#include "CubeModel.h"

class BaseModel;

// Cube 在 Scheme 中的所有场区，添加场区过程中，可能扩大原有区域范围
// 添加的场区在场区管理界面中占一行，
//
class CubeMan
{
public:
    CubeMan();
    ~ CubeMan();

    int addCube(CubeModel * cubeUse);
    void mergeCubeRange();
    QList <CubeModel *> cubeModels;

    // 所有Cube合并的总的范围
    Field mrgField;
};

// 从模型的角度，收集所有需要输出的对象
class ModelMan
{
public:

    ModelMan();
    void removeAllModels();
    int addModel(BaseModel * model);
    int prepareAllDarw();
    QList<BaseModel *> models;
};

// 方案，对应工程，仅保存原始的场区数据文件名，声源数据文件名，以及用户定义的视景点
class Scheme : public QObject
{
    Q_OBJECT
public:
    explicit Scheme(QObject *parent = nullptr);
    int saveToFile(QString file);
    int loadFromFile(QString file);
    // 创建新的
    int emptyContent();

    // 是否进行了修改
    bool needSave;

    // 说明信息
    QString schemeInfo;
    // 包含的数据文件，含绝对路径的文件名
    QStringList cubeFiles;
    // 保存时所使用的颜色映射表
    ColorMap colorMap;
    // 声源文件
    QStringList soundSourceFiles;
    // 保存的视景点
    QList<ViewPot> viewPots;

signals:

public slots:
};



#endif // SCHEME_H
