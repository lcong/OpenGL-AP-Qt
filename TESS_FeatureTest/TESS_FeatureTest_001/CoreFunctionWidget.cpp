#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QDateTime>
#include "CoreFunctionWidget.h"



CoreFunctionWidget::CoreFunctionWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}


CoreFunctionWidget::~CoreFunctionWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void CoreFunctionWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    createShader();

    // set up vertex data (and buffer(s)) and configure vertex attributes

    // ------------------------------------------------------------------
    // Triangle postion and color attributes
    float vertices[] = {
        // positions          // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,	0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    glEnable(GL_DEPTH_TEST);


    // first, configure the VAO (and VBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//  glPatchParameteri(GL_PATCH_VERTICES, 3); //注意！！注释掉这句，才可以让GS生效；
    glDrawArrays(GL_PATCHES, 0, 3);
}


void CoreFunctionWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void CoreFunctionWidget::paintGL()
{
    // render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);
    mShader.bind();

    glDrawArrays(GL_PATCHES, 0, 3);

    glPolygonMode(GL_FRONT, GL_LINE);
}


bool CoreFunctionWidget::createShader()
{
    bool success = mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/TESS_FeatureTest_001.vert");

    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }


    success = mShader.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/TESS_FeatureTest_001.tcs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/TESS_FeatureTest_001.tes");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }


    success = mShader.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/TESS_FeatureTest_001.gs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/TESS_FeatureTest_001.fs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << mShader.log();
    }

    return (success);
}
