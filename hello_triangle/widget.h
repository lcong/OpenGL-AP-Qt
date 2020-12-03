#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QDebug>

class Triangle : public QOpenGLWidget, protected QOpenGLExtraFunctions
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
};

#endif // WIDGET_H
