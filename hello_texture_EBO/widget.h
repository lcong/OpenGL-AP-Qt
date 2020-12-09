#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>  //新添项
#include "shader.h"

class Triangle : public QOpenGLWidget
{
public:
    Triangle();
    GLuint a;
    ~Triangle();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    Shader *ourShader;
    QOpenGLTexture *texture;//新添项
    QOpenGLFunctions_3_3_Core *core;
    GLuint VBO, VAO, EBO;
};

#endif // WIDGET_H
