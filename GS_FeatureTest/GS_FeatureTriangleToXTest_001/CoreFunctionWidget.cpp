#include "CoreFunctionWidget.h"
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QDateTime>

// lighting
static QVector3D lightPos(1.2f, 1.0f, 2.0f);

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
    float vertices[] = {
        // positions    //color
        -0.5f,	0.5f, 1.0f, 0.0f, 0.0f, // top-left
        0.5f,	0.5f, 0.0f, 1.0f, 0.0f, // top-right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };

    glEnable(GL_DEPTH_TEST);

    // first, configure the VAO (and VBO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
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
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


bool CoreFunctionWidget::createShader()
{
    bool success = mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/GS_TriangleToTriangleTest_001.vs");

    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/GS_TriangleToTriangleTest_001.gs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GS_TriangleToTriangleTest_001.fs");
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
