#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QDebug>

class Triangle : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    Triangle();
    ~Triangle();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    GLuint shaderProgram;
    GLuint vbo, vao, tbo;
    GLint vertexShader;
    GLint geoShader;
};

#endif // WIDGET_H
