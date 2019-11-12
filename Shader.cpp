#include "Shader.h"

Shader::Shader()
{
}

int Shader::init(const QString& vert, const QString& frag, const QString& geom)
{
    linked = false;
    shaderProgram = nullptr;
    //qDebug() << "vertexSource" << vert;
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceCode(vert);
    if (!success)
    {
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
        qDebug() << vertexShader.log() << endl;
        return -1;
    }

    //qDebug() << "fragmentSource" << frag;
    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success  = fragmentShader.compileSourceCode(frag);
    if (!success)
    {
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
        qDebug() << fragmentShader.log() << endl;
        return -1;
    }
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShader(&vertexShader);
    shaderProgram->addShader(&fragmentShader);

    //qDebug() << "geometrySource" << geom;
    if (geom != "")
    {
        QOpenGLShader geometryShader(QOpenGLShader::Geometry);
        success  = geometryShader.compileSourceCode(geom);
        shaderProgram->addShader(&geometryShader);
        if (!success)
        {
            qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED" << endl;
            qDebug() << geometryShader.log() << endl;
            return -1;
        }
    }

    //qDebug() << "link";
    linked = shaderProgram->link();
    if (!linked)
    {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
        qDebug() << shaderProgram->log() << endl;
        return -1;
    }
    return 1;
}

Shader::~Shader()
{
    //没有析构的ShaderProgram，可tnd的自带opengl函数没有对QOpenGLShaderProgram的 delete删除函数啊
    //    delete shaderProgram;
}

//
Shader & Shader::bind()
{
    shaderProgram->bind();
    return *this;
}

//还是把设置着色器uniform变量操作写成Shader里的inline成员函数管理，真的方便很多。
void Shader::setFloat(const QString& name, const GLfloat& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setInteger(const QString& name, const GLint& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setVector2f(const QString& name, const GLfloat& x, const GLfloat& y)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, QVector2D(x, y));
}

void Shader::setVector2f(const QString& name, const QVector2D& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, QVector3D(x, y, z));
}

void Shader::setVector3f(const QString& name, const QVector3D& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setVector4f(const QString& name, const GLfloat& x, const GLfloat& y,
                 const GLfloat& z, const GLfloat& w)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, QVector4D(x, y, z, w));
}

void Shader::setVector4f(const QString& name, const QVector4D& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setMatrix4f(const QString& name, const QMatrix4x4& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setBool(const QString& name, const GLboolean& value)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValue(loc, value);
}

void Shader::setFloatArray(const QString& name, const GLfloat *values, int count, int tupleSize)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValueArray(loc, values, count, tupleSize);
}

void Shader::setIntArray(const QString& name, const GLint *values, int count)
{
    int loc = shaderProgram->uniformLocation(name);
    shaderProgram->setUniformValueArray(loc, values, count);
}



