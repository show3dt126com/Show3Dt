#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>
#include "Shader.h"

// 根据学习，似乎可以抽象以下的基础对象，对需要输出的3D对象进行封装
class BaseModel : QObject
{
    Q_OBJECT
public:
    BaseModel(QObject *parent = nullptr) {}
    virtual ~ BaseModel() {}

    //
    virtual void prepareDraw() {}
    virtual void draw() {}

    GLuint vbo, vao, ebo;//VBO, VAO, EBO;

    bool dataPrepared;
    bool drawPrepared;

    Shader * shader;
};

#endif // BASEMODEL_H
