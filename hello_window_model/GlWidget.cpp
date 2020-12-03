#include "GlWidget.h"

static const char *VERTEX_SHADER_CODE =
    "#version 330 \n"
    "layout(location = 0) in vec3 posVertex;\n"
    "void main() {\n"
    "  gl_Position = vec4(posVertex, 1.0f);\n"
    "}\n";

static const char *FRAGMENT_SHADER_CODE =
    "#version 330\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "  fragColor = vec4(0.5f, 0.2f, 0.6f, 1.0f);\n"
    "}\n";


GlWidget::GlWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , VBO(QOpenGLBuffer::VertexBuffer)      //初始化为顶点缓冲对象
    , IBO(QOpenGLBuffer::IndexBuffer)       //初始化为索引缓冲对象
{
    vertices = {
        0.5f,	0.5f, 0.0f,     // 右上角
        0.5f,  -0.5f, 0.0f,     // 右下角
        -0.5f, -0.5f, 0.0f,     // 左下角
        -0.5f,	0.5f, 0.0f      // 左上角
    };
    indices = {                     // 注意索引从0开始!
        0, 1, 3,                // 第一个三角形
        1, 2, 3                 // 第二个三角形
    };
}


GlWidget::~GlWidget()
{
}


void GlWidget::initializeGL()
{
    this->initializeOpenGLFunctions();                                                              //初始化opengl函数
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/triangle.vert")) {         //添加并编译顶点着色器
        qDebug()<<"ERROR:"<<shaderProgram.log();                                                //如果编译出错,打印报错信息
    }
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/triangle.frag")) {       //添加并编译片段着色器
        qDebug()<<"ERROR:"<<shaderProgram.log();                                                //如果编译出错,打印报错信息
    }
    if (!shaderProgram.link()) {                                                                    //链接着色器
        qDebug()<<"ERROR:"<<shaderProgram.log();                                                //如果链接出错,打印报错信息
    }

//    VAO.create();       //生成VAO对象
//    VAO.bind();         //绑定VAO，之后所以的顶点缓冲对象的操作都会存储到VAO中
    QOpenGLVertexArrayObject::Binder{ &VAO };

    VBO.create();   //生成VBO对象
    VBO.bind();     //将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中
    //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
    VBO.allocate(vertices.data(), sizeof(float)*vertices.size());

    IBO.create();   //生成IBO对象
    IBO.bind();     //将IBO绑定到当前的索引缓冲对象（QOpenGLBuffer::IndexBuffer）中
    //将顶点索引数据分配到IBO中，第一个参数为数据指针，第二个参数为数据的字节长度
    IBO.allocate(indices.data(), sizeof(unsigned int)*indices.size());

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    shaderProgram.enableAttributeArray(0);

//    VAO.release();      //解绑VAO，安全使用
}


void GlWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h); //定义视口区域
}


void GlWidget::paintGL()
{
    this->glClearColor(0.1f, 0.5f, 0.7f, 1.0f);     //设置清屏颜色
    this->glClear(GL_COLOR_BUFFER_BIT);             //清除颜色缓存
    shaderProgram.bind();                           //使用shaderProgram着色程序
    {
//      QOpenGLVertexArrayObject::Binder{ &VAO };
        this->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
