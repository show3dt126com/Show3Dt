#ifndef TEXTER_H
#define TEXTER_H


#include <QDebug>
#include <qopengl.h>
#include <map>
#include <QString>

#include <QVector>

#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLTexture>
#include <QMap>



class Shader;

// 字体管理，文本绘制器
struct Character
{
    GLuint TextureID;   // 字形纹理ID
    // 字形大大小 glm::ivec2 Size;
    QVector2D Size;
    // glm::ivec2 Bearing;  字形基于基线和起点的位置
    QVector2D Bearing;
    GLuint Advance;     // 起点到下一个字形起点的距离
};

class Texter
{
public:
    Texter();
    int initTexter();
    void renderText(const char * text, GLfloat x, GLfloat y, GLfloat scale, QVector3D color);

    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
};


#endif // TEXTER_H
