#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <QOpenGLTexture>

// 2D 纹理管理

//
class Texture2D
{
public:
    Texture2D();
    ~Texture2D();
    // 读取纹理文件，创建 QOpenGLTexture::Target2D(QOpenGLTexture::Target2D)
    void generate(const QString& file, GLboolean alpha = false);
    void bind() const;

    //Format of texture object
    QOpenGLTexture::TextureFormat internal_format;
    QOpenGLTexture::WrapMode wrap_s;
    QOpenGLTexture::WrapMode wrap_t;
    QOpenGLTexture::Filter filter_min;
    QOpenGLTexture::Filter filter_max;

private:
    QOpenGLTexture* texture;
};

#endif // TEXTURE2D_H
