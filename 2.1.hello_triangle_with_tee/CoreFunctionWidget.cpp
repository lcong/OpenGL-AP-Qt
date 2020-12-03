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
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // Triangle
    float vertices[] = {
        // positions          // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,	0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    glEnable(GL_DEPTH_TEST);

    // first, configure the VAO (and VBO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    glBindVertexArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
}


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

bool CoreFunctionWidget::createShader()
{
    bool success = mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/2.1.hellotriangle.vert");

    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/2.1.hellotriangle.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/2.1.hellotriangle.tcs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << mShader.log();
        return (success);
    }

    success = mShader.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/2.1.hellotriangle.tes");
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
