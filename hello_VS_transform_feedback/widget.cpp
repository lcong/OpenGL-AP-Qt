#include "widget.h"


const char *vertexShaderSource =
    "#version 420 core\n"
    "in float inValue;\n"
    "out float outValue;\n"
    "void main(){\n"
    "   outValue = sqrt(inValue);\n"
    "}\n\0";

const char *fragmentShaderSource =
    "#version 420 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

Triangle::Triangle()
{
}


Triangle::~Triangle()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}


void Triangle::initializeGL()
{
    //着色器部分
    this->initializeOpenGLFunctions();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << Qt::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);

    /*
     * 编译着色器，创建一个程序并附加着色器，但是不要调用glLinkProgram()！在链接程序之前，我们必须告诉OpenGL，我们想要捕获到一个缓冲区中的输出属性。
     *
     *
     */
    const GLchar *feedbackVaryings[] = { "outValue" };
    glTransformFeedbackVaryings(shaderProgram, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);


    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << Qt::endl;
    }

    glUseProgram(shaderProgram);

    // Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create input VBO and vertex format
    GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLint inputAttrib = glGetAttribLocation(shaderProgram, "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // Create transform feedback buffer
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);

    // Perform feedback transform
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

    // 在进行绘制调用之前，您必须设置transform_feedback模式:
    glBeginTransformFeedback(GL_POINTS);
    // 如果你只有一个顶点着色器，就像我们现在所做的那样，普通模式必须与被绘制的那个元素匹配:
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

    glFlush();

    // Fetch and print results
    GLfloat feedback[5];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

    for (uint32_t i = 0; i < (uint32_t)sizeof(feedback)/sizeof(feedback[0]); i++)
    {
        qDebug("feedback[%d]：%f", i, feedback[i]);
    }
}


void Triangle::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void Triangle::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
}
