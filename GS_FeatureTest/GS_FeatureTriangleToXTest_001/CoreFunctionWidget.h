﻿#ifndef COREFUNCTIONWIDGET_H
#define COREFUNCTIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <memory>
#include "Camera.h"

class CoreFunctionWidget : public QOpenGLWidget
               , protected /*QOpenGLExtraFunctions*/ QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit CoreFunctionWidget(QWidget *parent = nullptr);
    ~CoreFunctionWidget() Q_DECL_OVERRIDE;

protected:
    void initializeGL()  Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;


private:
    bool createShader();

private:
    QOpenGLShaderProgram mShader;
    GLuint VBO;
    GLuint VAO;
};

#endif // COREFUNCTIONWIDGET_H
