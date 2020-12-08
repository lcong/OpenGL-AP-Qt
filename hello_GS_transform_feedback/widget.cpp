#include "widget.h"

GLuint VBO, VAO;

const char *vertexShaderSource =
    "#version 420 core\n"
    "in float inValue;\n"
    "out float geoValue;\n"
    "void main(){\n"
    "   geoValue = sqrt(inValue);\n"
    "}\n\0";

const char *geoShaderSrc =
    "#version 420 core\n"
    "layout(points) in;\n"
    "layout(triangle_strip, max_vertices = 3) out;\n"
    "in float[] geoValue;\n"
    "out float outValue;\n"
    "void main(){\n"
    "for (int i = 0; i < 3; i++) {\n"
    "   outValue = geoValue[0] + i;\n"
    "   EmitVertex();\n"
    "}\n"
    "EndPrimitive();\n"
    "}\n\0";

Triangle::Triangle()
{
}


Triangle::~Triangle()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(geoShader);

    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}


void Triangle::initializeGL()
{
    //着色器部分
    this->initializeOpenGLFunctions();

    GLint success;
    GLchar infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<  endl;
    }

    geoShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geoShader, 1, &geoShaderSrc, NULL);
    glCompileShader(geoShader);
    glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geoShader);


    /*
     * 编译着色器，创建一个程序并附加着色器，但是不要调用glLinkProgram()！
     * 在链接程序之前，我们必须告诉OpenGL，我们想要捕获到一个缓冲区中的输出属性。
     *
     */
    const GLchar *feedbackVaryings[] = { "outValue" };
    glTransformFeedbackVaryings(shaderProgram, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);


    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glUseProgram(shaderProgram);

    // Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create input VBO and vertex format
    GLfloat data[] = { 1.0f, 4.0f, 9.0f, 16.0f, 25.0f };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLint inputAttrib = glGetAttribLocation(shaderProgram, "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // Create transform feedback buffer
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(data), nullptr, GL_STATIC_READ);

    // Perform feedback transform
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

    // 在进行绘制调用之前，您必须设置transform_feedback模式, 注意使用GL_TRIAGNLES ,与仅有vertex shader(为GL_POINTS)的设置不一样。
    glBeginTransformFeedback(GL_TRIANGLES);

    // 如果你只有一个顶点着色器，就像我们现在所做的那样，普通模式必须与被绘制的那个元素匹配:
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

    glFlush();

    // Fetch and print results, 结果的个数为3倍，也就是3*5=15
    GLfloat feedback[15];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

    for (int i = 0; i < 15; i++)
    {
        qDebug("feedback[%d]：%f", i, feedback[i]);
        //printf("%f\n", feedback[i]);
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

    /*
     *
     *
     * // Perform feedback transform
     * glEnable(GL_RASTERIZER_DISCARD);
     *
     * glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
     *
     * // 在进行绘制调用之前，您必须设置transform_feedback模式, 注意使用GL_TRIAGNLES ,与仅有vertex shader(为GL_POINTS)的设置不一样。
     * glBeginTransformFeedback(GL_TRIANGLES);
     *
     * // 如果你只有一个顶点着色器，就像我们现在所做的那样，普通模式必须与被绘制的那个元素匹配:
     * glDrawArrays(GL_POINTS, 0, 5);
     * glEndTransformFeedback();
     *
     * glDisable(GL_RASTERIZER_DISCARD);
     *
     * glFlush();
     *
     * // Fetch and print results, 结果的个数为3倍，也就是3*5=15
     * GLfloat feedback[15];
     * glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
     *
     * for (int i = 0; i < 15; i++)
     * {
     *  qDebug("feedback[%d]：%f", i, feedback[i]);
     *  //printf("%f\n", feedback[i]);
     * }
     *
     *
     */


    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
}
