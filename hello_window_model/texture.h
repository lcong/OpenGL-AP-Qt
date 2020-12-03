#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
#include <QOpenGLTexture>
#include <QSharedPointer>

//纹理
struct Texture {
    QString				type;
    QString				filename;
    QSharedPointer<QOpenGLTexture>	texture;
};

#endif // !TEXTURE_H
