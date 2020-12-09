#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QDebug>

class Triangle : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    Triangle();
    ~Triangle();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    GLuint shaderProgram;
    GLuint VBO, VAO, TBO;
    GLuint vertexShader;
    GLuint geoShader;
};

#endif // WIDGET_H
