﻿#include "QtFunctionWidget.h"
#include <QDebug>
#include <QTimer>

// lighting
static QVector3D lightPos(1.2f, 1.0f, 2.0f);

QtFunctionWidget::QtFunctionWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    camera = std::make_unique<Camera>(QVector3D(0.0f, 0.0f, 3.0f));

    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, [ = ] {
        m_nTimeValue += 1;
        update();
    });
    m_pTimer->start(40);//25 fps
}


QtFunctionWidget::~QtFunctionWidget()
{
    makeCurrent();

    lightVAO.destroy();
    cubeVAO.destroy();
    vbo.destroy();

    m_pDiffuseMap.reset();
    m_pSpecularMap.reset();

    doneCurrent();
}


void QtFunctionWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    createShader();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,	0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,	0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,	0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,	0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        0.5f,	0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,	0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f,	0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,	0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,	0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f,	0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,	0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,	0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,	0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,	0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,	0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,	0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,	0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,	0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    {
        QOpenGLVertexArrayObject::Binder vaoBind(&cubeVAO);

//        position attribute
//        int attr = -1;

//        attr = lightingShader.attributeLocation("aPos");
//        lightingShader.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
//        lightingShader.enableAttributeArray(attr);

//        attr = lightingShader.attributeLocation("aNormal");
//        lightingShader.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
//        lightingShader.enableAttributeArray(attr);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    {
        QOpenGLVertexArrayObject::Binder vaoBind(&lightVAO);

//        position attribute
//        int attr = -1;
//        attr = lightingShader.attributeLocation("aPos");
//        lightingShader.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
//        lightingShader.enableAttributeArray(attr);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    m_pDiffuseMap = std::make_unique<QOpenGLTexture>(QImage(":/container2.png"), QOpenGLTexture::GenerateMipMaps);
    m_pDiffuseMap->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_pDiffuseMap->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_pDiffuseMap->setMinificationFilter(QOpenGLTexture::Linear);
    m_pDiffuseMap->setMagnificationFilter(QOpenGLTexture::Linear);

    m_pSpecularMap = std::make_unique<QOpenGLTexture>(QImage(":/container2_specular.png"), QOpenGLTexture::GenerateMipMaps);
    m_pSpecularMap->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_pSpecularMap->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_pSpecularMap->setMinificationFilter(QOpenGLTexture::Linear);
    m_pSpecularMap->setMagnificationFilter(QOpenGLTexture::Linear);

    // shader configuration
    // --------------------
    lightingShader.bind();
    lightingShader.setUniformValue("material.diffuse", 0);
    lightingShader.setUniformValue("material.specular", 1);
    lightingShader.release();

    vbo.release();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}


void QtFunctionWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void QtFunctionWidget::paintGL()
{
    // input
    // -----
    camera->processInput(0.5f);//speed

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.bind();
    lightingShader.setUniformValue("light.position", lightPos);
    lightingShader.setUniformValue("viewPos", camera->position);

    // light properties
    lightingShader.setUniformValue("light.ambient", QVector3D(0.2f, 0.2f, 0.2f));
    lightingShader.setUniformValue("light.diffuse", QVector3D(0.5f, 0.5f, 0.5f));
    lightingShader.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

    // material properties
    lightingShader.setUniformValue("material.shininess", 64.0f);

    // view/projection transformations
    QMatrix4x4 projection;
    projection.perspective(camera->zoom, 1.0f * width() / height(), 0.1f, 100.0f);
    QMatrix4x4 view = camera->getViewMatrix();
    lightingShader.setUniformValue("projection", projection);
    lightingShader.setUniformValue("view", view);

    // world transformation
    QMatrix4x4 model;
    lightingShader.setUniformValue("model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    m_pDiffuseMap->bind();

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    m_pSpecularMap->bind();

    {// render the cube
        QOpenGLVertexArrayObject::Binder vaoBind(&cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    lightingShader.release();

    // also draw the lamp object
    lampShader.bind();
    lampShader.setUniformValue("projection", projection);
    lampShader.setUniformValue("view", view);
    model = QMatrix4x4();
    model.translate(lightPos);
    model.scale(0.2f); // a smaller cube
    lampShader.setUniformValue("model", model);
    {
        QOpenGLVertexArrayObject::Binder vaoBind(&lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    lampShader.release();
}


void QtFunctionWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if ((key >= 0) && (key < 1024)) {
        camera->keys[key] = true;
    }
}


void QtFunctionWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    if ((key >= 0) && (key < 1024)) {
        camera->keys[key] = false;
    }
}


void QtFunctionWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bLeftPressed = true;
        m_lastPos = event->pos();
    }
}


void QtFunctionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bLeftPressed = false;
}


void QtFunctionWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bLeftPressed) {
        int xpos = event->pos().x();
        int ypos = event->pos().y();

        int xoffset = xpos - m_lastPos.x();
        int yoffset = m_lastPos.y() - ypos;
        m_lastPos = event->pos();
        camera->processMouseMovement(xoffset, yoffset);
    }
}


void QtFunctionWidget::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();

    camera->processMouseScroll(offset.y()/20.0f);
}


bool QtFunctionWidget::createShader()
{
    bool success = lightingShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighting_maps.vert");

    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lightingShader.log();
        return (success);
    }

    success = lightingShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighting_maps.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lightingShader.log();
        return (success);
    }

    success = lightingShader.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << lightingShader.log();
    }

    success = lampShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lamp.vert");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lampShader.log();
        return (success);
    }

    success = lampShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lamp.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lampShader.log();
        return (success);
    }

    success = lampShader.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << lampShader.log();
    }

    return (success);
}
