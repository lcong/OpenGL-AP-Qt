#ifndef MESH_H
#define MESH_H
#include "vertex.h"
#include "texture.h"

#include <vector>
#include <string>

#include "qopenglfunctions_3_3_core.h"
class QOpenGLShaderProgram;

class Mesh : protected QOpenGLFunctions_3_3_Core
{
public:
    Mesh() = default;
    ~Mesh();
    //构造
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices,
        const std::vector<Texture>& textures);
    //画网格模型
    void draw(QOpenGLShaderProgram *program);

    //顶点
    std::vector<Vertex> vertices;
    //索引
    std::vector<GLuint> indices;
    //纹理
    std::vector<Texture> textures;
    //是否进行过setupMesh初始化操作
    bool isinitialized;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
private:
    // 初始化所有缓冲区对象/数组
    void setupMesh(QOpenGLShaderProgram *program);
};

#endif // !MESH_H
