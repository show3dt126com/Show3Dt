#include "Texture2D.h"


Texture2D::Texture2D():
    internal_format(QOpenGLTexture::RGBFormat),
    wrap_s(QOpenGLTexture::Repeat),
    wrap_t(QOpenGLTexture::Repeat),
    filter_min(QOpenGLTexture::Linear),
    filter_max(QOpenGLTexture::Linear)
{
    texture = nullptr;
}

Texture2D::~Texture2D()
{
    //一样的析构问题，不会解决 擦，在ResourceManager里 擦
    //    delete texture;
    texture = nullptr;
}

void Texture2D::generate(const QString& file, GLboolean alpha)
{

    //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setFormat(internal_format);
    texture->setData(QImage(file).mirrored(), QOpenGLTexture::GenerateMipMaps);

    // 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);

    //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setMinificationFilter(filter_min);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setMagnificationFilter(filter_max);

    if (alpha)
    {
        internal_format = QOpenGLTexture::RGBAFormat;
        //在纹理的边界部分，没有任何处理。
        wrap_s = QOpenGLTexture::ClampToBorder;
        wrap_t = QOpenGLTexture::ClampToBorder;
    }

}

void Texture2D::bind() const
{
    texture->bind();
}
