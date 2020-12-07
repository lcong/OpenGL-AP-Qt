#ifndef QTFUNCTIONWIDGET_H
#define QTFUNCTIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <memory>
#include "Camera.h"

class QtFunctionWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    QtFunctionWidget(QWidget *parent = nullptr);
    ~QtFunctionWidget() Q_DECL_OVERRIDE;

protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    bool createShader();

private:
    QOpenGLShaderProgram lightingShader, lampShader;
    QOpenGLBuffer vbo{ QOpenGLBuffer::VertexBuffer };
    QOpenGLVertexArrayObject cubeVAO, lightVAO;

    std::unique_ptr<QOpenGLTexture> m_pDiffuseMap;
    std::unique_ptr<QOpenGLTexture> m_pSpecularMap;
    std::unique_ptr<QOpenGLShaderProgram> m_program;

    QTimer *m_pTimer{ nullptr };
    int m_nTimeValue{ 0 };

    // camera
    std::unique_ptr<Camera> camera;
    bool m_bLeftPressed{ false };
    QPoint m_lastPos;
};

#endif // QTFUNCTIONWIDGET_H
