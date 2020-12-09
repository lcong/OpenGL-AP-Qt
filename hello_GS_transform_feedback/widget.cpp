#include "widget.h"

// Vertex shader
const GLchar *vertexShaderSrc = R"glsl(
    #version 150 core
    in float inValue;
    out float geoValue;
    void main()
    {
        geoValue = sqrt(inValue);
    }
)glsl";


// Geometry shader
const GLchar *geoShaderSrc = R"glsl(
    #version 150 core
    layout(points) in;
    layout(triangle_strip, max_vertices = 3) out;
    in float[] geoValue;
    out float outValue;
    void main()
    {
        for (int i = 0; i < 3; i++) {
            outValue = geoValue[0] + i;
            EmitVertex();
        }
        EndPrimitive();
    }
)glsl";

Triangle::Triangle()
{
}


Triangle::~Triangle()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(geoShader);

    glDeleteBuffers(1, &TBO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &VAO);
}


void Triangle::initializeGL()
{
    this->initializeOpenGLFunctions();

    GLint success;
    GLchar infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<  Qt::endl;
    }

    geoShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geoShader, 1, &geoShaderSrc, NULL);
    glCompileShader(geoShader);
    glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << Qt::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geoShader);


    /*
     * 编译着色器，创建一个程序并附加着色器，但是不要调用glLinkProgram()！
     * 在链接程序之前，我们必须告诉OpenGL，我们想要捕获到一个缓冲区中的输出属性。
     * 以下两种格式可供选择:
     *
     * --GL_INTERLEAVED_ATTRIBS:将所有属性写入一个缓冲区对象。
     * --GL_SEPARATE_ATTRIBS: 将属性写入多个缓冲区对象，或将不同的偏移量写入缓冲区。
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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create input VBO and vertex format
    GLfloat verticesData[] = { 1.0f, 4.0f, 9.0f, 16.0f, 25.f };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

    GLint inputAttrib = glGetAttribLocation(shaderProgram, "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);


    /*
     * 我们现在传递了一个nullptr，以创建一个足够大的缓冲区，以便容纳所有生成的浮点数，但是没有指定任何初始数据。
     * 设置一个适当的使用类型GL_STATIC_READ，它表示我们打算将OpenGL写入到这个缓冲区中，我们的应用程序可以从中读取。
     * @brief glGenBuffers
     */

    // Create transform feedback buffer
    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(verticesData), nullptr, GL_STATIC_READ);


    /*
     * 现在我们已经为渲染计算过程做了所有的准备。由于我们不打算画任何东西，光栅化器应该被禁用:
     *
     *
     */
    glEnable(GL_RASTERIZER_DISCARD);


    /*
     * 为了实际绑定我们在上面创建的缓冲区作为转换反馈缓冲区，我们必须使用一个名为glBindBufferBase的新函数。
     *
     */
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, TBO);

    // 在进行绘制调用之前，您必须设置transform_feedback模式, 注意使用GL_TRIAGNLES ,与仅有vertex shader(为GL_POINTS)的设置不一样。
    // Perform feedback transform
    glBeginTransformFeedback(GL_TRIANGLES);

    // 如果你只有一个顶点着色器，就像我们现在所做的那样，普通模式必须与被绘制的那个元素匹配:
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();


    //重新开启光栅化
    glDisable(GL_RASTERIZER_DISCARD);

    glFlush();

    // Fetch and print results, 结果的points个数为3倍，也就是3*5=15
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
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
}
