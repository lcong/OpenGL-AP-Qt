#include "widget.h"


Triangle::Triangle()
{
    this->setWindowTitle("Texture");
}


Triangle::~Triangle()
{
    delete ourShader;
    core->glDeleteVertexArrays(1, &VAO);
    core->glDeleteBuffers(1, &VBO);
    core->glDeleteBuffers(1, &EBO);
    texture->destroy(); //纹理使用完 进行删除
}


void Triangle::initializeGL()
{
    //着色器部分
    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    ourShader = new Shader(":/vertexshadersource.vert", ":/fragmentshadersource.frag");

    //VAO，VBO数据部分
    GLfloat vertices[] = {
        0.5f,	0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,       // top right //注意新的数据，有纹理单元
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,       // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,       // bottom left
        -0.5f,	0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f        // top left
    };

    GLuint indices[] = {
        0, 1, 3,        // first triangle
        1, 2, 3         // second triangle
    };

    core->glGenVertexArrays(1, &VAO);//两个参数，第一个为需要创建的缓存数量。第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址
    core->glGenBuffers(1, &VBO);
    core->glGenBuffers(1, &EBO);

    core->glBindVertexArray(VAO);

    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    core->glEnableVertexAttribArray(0);

    // color attribute
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    core->glEnableVertexAttribArray(1);

    // texture coord attribute
    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    core->glEnableVertexAttribArray(2);

    //纹理
    texture = new QOpenGLTexture(QImage(":/container.jpg").mirrored(), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if (!texture->isCreated()) {
        qDebug() << "Failed to load texture" << endl;
    }
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);       // 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);       //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    texture->setMinificationFilter(QOpenGLTexture::Linear);         //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->setFormat(QOpenGLTexture::RGBFormat);                  //将纹理储存为rgb值


    core->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}


void Triangle::resizeGL(int w, int h)
{
    core->glViewport(0, 0, w, h);
}


void Triangle::paintGL()
{
    core->glClear(GL_COLOR_BUFFER_BIT);

    texture->bind();
    ourShader->use();
    core->glBindVertexArray(VAO);
    core->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
