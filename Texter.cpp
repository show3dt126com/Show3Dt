
#include <QVector>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLTexture>
#include <QMap>

#include "Texter.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "Global.h"

Texter::Texter()
{

}

int Texter::initTexter()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        qDebug() << "ERROR::FREETYPE: Could not init FreeType Library\n";
    FT_Face face;
    if (FT_New_Face(ft, "resource/Arial.ttf", 0, &face))
        qDebug() << "ERROR::FREETYPE: Failed to load font\n";
    else
        qDebug() << "Font Face: Arial\n";

    FT_Set_Pixel_Sizes(face, 0, 48);

    //禁用byte-alignment限制
    C->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        // 加载字符的字形
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            qDebug() << "ERROR::FREETYTPE: Failed to load Glyph\n";
            continue;
        }

        // 生成字形纹理
        GLuint texture;
        C->glGenTextures(1, &texture);
        C->glBindTexture(GL_TEXTURE_2D, texture);
        C->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                      0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        //qDebug() << "WH" << face->glyph->bitmap.width << face->glyph->bitmap.rows;

        // 设置纹理选项
        C->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        C->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        C->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        C->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 将字符存储到字符表中备用
        Character character = {
            texture,
            QVector2D(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            QVector2D(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x};
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    C->glGenVertexArrays(1, &VAO);
    C->glGenBuffers(1, &VBO);
    C->glBindVertexArray(VAO);
    C->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    C->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    C->glEnableVertexAttribArray(0);
    C->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    C->glBindBuffer(GL_ARRAY_BUFFER, 0);
    C->glBindVertexArray(0);

    return 1;
}

// RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
// RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

void Texter::renderText(const char * text, GLfloat x, GLfloat y, GLfloat scale, QVector3D color)
{
    //Shader &shader = G.shaderText;
    //shader.bind();
    //glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
    //shader.setVector3f("textColor", color.x(), color.y(), color.z());

    C->glActiveTexture(GL_TEXTURE0);
    C->glBindVertexArray(VAO);

    // 对文本中的所有字符迭代
    for (int i=0; i<strlen(text); i++)
    {
        char c = text[i];
        Character ch = Characters[c];
        GLfloat xpos = x + ch.Bearing.x() * scale;
        GLfloat ypos = y - (ch.Size.y() - ch.Bearing.y()) * scale;
        GLfloat w = ch.Size.x() * scale;
        GLfloat h = ch.Size.y() * scale;
        // 当前字符的VBO
        GLfloat vertices[6][4] =
        {
            { xpos, ypos + h, 0.0, 0.0 },
            { xpos, ypos, 0.0, 1.0 },
            { xpos + w, ypos, 1.0, 1.0 },
            { xpos, ypos + h, 0.0, 0.0 },
            { xpos + w, ypos, 1.0, 1.0 },
            { xpos + w, ypos + h, 1.0, 0.0 }
        };
        //qDebug() << xpos << ypos << w << h ;
        // 在方块上绘制字形纹理
        C->glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新当前字符的VBO
        C->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        C->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        C->glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制方块
        C->glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; //(2^6 = 64)
    }
    C->glBindVertexArray(0);
    C->glBindTexture(GL_TEXTURE_2D, 0);
}
