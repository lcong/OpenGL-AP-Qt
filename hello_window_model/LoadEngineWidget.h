#ifndef LOADENGINE_H
#define LOADENGINE_H


#include <QOpenGLTexture>
#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QString>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>
class Model;
class QOpenGLTexture;
class QOpenGLShaderProgram;

class LoadEngineWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit LoadEngineWidget(QWidget *parent = 0);
    ~LoadEngineWidget();
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;

private:
    //着色器程序
    QOpenGLShaderProgram *program;
    Model *pmodel;
    //时间
    QElapsedTimer time;
};

#endif // !LOADMODEL_H
