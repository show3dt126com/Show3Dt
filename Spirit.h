#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLTexture>
#include <QMap>
#include "Texture2D.h"

// 模型中 对象
class Article;

// 模型中的纹理
class Material;

// 从标准格式读取模型，小物件，小实体：潜艇，飞鸟，
// 一般有 .obj .jpg .png .mtl
class Spirit
{
public:
    Spirit();
    // path: obj文件的路径
    bool init(const QString& path);
    // 绘制
    void draw(GLboolean isOpenLighting = GL_FALSE);
private:
    // 读取 obj文件以及相关的其它纹理等文件
    bool loadSpirit(const QString& path);
    // 为模型中的每个对象构建 Buffer
    void bindBufferData();

    bool loaded;
    QVector<Article> articels;
    QMap<QString, Material> map_materials;
    QMap<QString, Texture2D> mapTextures;
};

// 模型中的一个对象部件
class Article
{
public:
    // 构建的 Buffer
    GLuint positionVBO;
    GLuint uvVBO;
    GLuint normalVBO;

    // 模型中的原始数据 顶点
    QVector<QVector3D> positions;
    // 纹理坐标
    QVector<QVector2D> uvs;
    // 法线
    QVector<QVector3D> normals;

    //材质名称
    QString matName;
};

//一个简易的材质类
class Material
{
public:
    //ambient反射系数
    QVector3D Ka;
    //diffuse反射系数
    QVector3D Kd;
    //specular反射系数
    QVector3D Ks;

    //环境反射贴图
    //  QOpenGLTexture* map_Ka;
    //漫反射贴图
    //  QOpenGLTexture* map_Kd;

    double shininess;
    QString name_map_Ka;
    QString name_map_Kd;
};

#endif // MODEL_H
