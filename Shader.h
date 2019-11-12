#ifndef SHADER_H
#define SHADER_H

#include <QObject>
#include <QOpenGLShaderProgram>

class Shader
{
public:
    Shader();
    ~Shader();
    //
    int init(const QString& vert, const QString& frag, const QString& geom = nullptr);
    Shader & bind();

    //还是把设置着色器uniform变量操作写成Shader里的inline成员函数管理，真的方便很多。
    void setFloat(const QString& name, const GLfloat& value);
    void setInteger(const QString& name, const GLint& value);
    void setVector2f(const QString& name, const GLfloat& x, const GLfloat& y);
    void setVector2f(const QString& name, const QVector2D& value);
    void setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z);
    void setVector3f(const QString& name, const QVector3D& value);
    void setVector4f(const QString& name, const GLfloat& x, const GLfloat& y,
                     const GLfloat& z, const GLfloat& w);
    void setVector4f(const QString& name, const QVector4D& value);
    void setMatrix4f(const QString& name, const QMatrix4x4& value);
    void setBool(const QString& name, const GLboolean& value);


    void setFloatArray(const QString& name, const GLfloat *values, int count, int tupleSize);
    void setIntArray(const QString& name, const GLint *values, int count);

    QOpenGLShaderProgram * shaderProgram;
    bool linked;
};

#endif // SHADER_H
