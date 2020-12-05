#include "LoadEngineWidget.h"
#include "model.h"
#include <QOPenglTexture>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <Qdir>



LoadEngineWidget::LoadEngineWidget(QWidget *parent) : QOpenGLWidget(parent),
    program(new QOpenGLShaderProgram),
    pmodel(nullptr)
{
    //设置OpenGL的版本信息
    QSurfaceFormat format;

    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //指定opengl版本为3.3
    format.setVersion(3, 3);
    setFormat(format);
    setCursor(Qt::ArrowCursor);
}


LoadEngineWidget::~LoadEngineWidget()
{
    //删除所有之前添加到program的着色器
    program->removeAllShaders();
    if (program) {
        delete program;
        program = nullptr;
    }
    if (pmodel) {
        delete pmodel;
        pmodel = nullptr;
    }
}


void LoadEngineWidget::initializeGL()
{
    //初始化OpenGL函数
    initializeOpenGLFunctions();

    glEnable(GL_CULL_FACE);

    //设置全局变量
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //将文件内容编译为指定类型的着色器，并将其添加到着色器程序program
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/model_loading.vs")) {
        //如果执行不成功，打印错误信息
        qDebug() << "compile error" << program->log();
    }
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model_loading.fs")) {
        //如果执行不成功，打印错误信息
        qDebug() << "compile error" << program->log();
    }
    //将顶点着色器跟片元着色器链接起来
    if (!program->link()) {
        //如果连接不成功，打印错误信息
        qDebug() << "link error" << program->log();
    }


    QDir temDir("./objects/backpack.obj");
    QString filePath = temDir.absolutePath();

    //模型网上自己找个，注意格式要符合assimp库支持的。
    // pmodel = new Model(":/backpack.obj");

    pmodel = new Model(filePath);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);//结束记录状态信息

    if (!program->bind()) {
        qDebug() << "bind error" << program->log();
    }
    //开启定时器
    time.start();

    //给着色器变量赋值,projextion,view默认构造是生成单位矩阵
    QMatrix4x4 projection, view, model;
    view.translate(QVector3D(-0.5f, 0.0f, -5.0f));
    projection.perspective(30.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 100.0f);

    /*
     * 将此程序绑定到active的OPenGLContext，并使其成为当前着色器程序
     * 先前绑定的着色器程序将被释放，等价于调用glUseProgram(GLuint id)
     * id为着色器程序调用programId()接口返回的GLuint类型的值
     */
    if (!program->bind()) {
        qDebug() << "bind error" << program->log();
    }
    program->setUniformValue("view", view);
    program->setUniformValue("model", model);
    program->setUniformValue("projection", projection);

    /* 固定属性区域 */
    glEnable(GL_DEPTH_TEST); //开启深度测试
}


void LoadEngineWidget::paintGL()
{
    //清理屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
    QMatrix4x4 model;

    model.scale(0.3f, 0.3f, 0.3f);
    model.rotate((float)time.elapsed() / 20, QVector3D(0.0f, 0.5f, 1.0f));
    if (!program->bind()) {
        qDebug() << "bind error" << program->log();
    }
    program->setUniformValue("model", model);
    pmodel->draw(program);
    program->release();
    update();
}


void LoadEngineWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    update();
}
