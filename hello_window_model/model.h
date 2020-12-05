#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include <QString>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct aiMesh;
struct aiScene;
class QString;
class QOpenGLTexture;
class Model
{
public:
    Model() = default;
    explicit Model(const QString& path);
    ~Model();
    //加载模型
    bool loadModel(const QString& path);


    void processNode(aiNode *node, const aiScene *scene);
    Mesh *processMesh(aiMesh *mesh, const aiScene *scene);

    /////////////////////////////////////
    bool initFromScene(const aiScene *pscene, const QString& path);
    Mesh *initMesh(const aiScene *pscene, const aiMesh *pmesh);

    //QSharedPointer<QOpenGLTexture> textureFile();
    std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, const QString& typeName);

    void draw(QOpenGLShaderProgram *program);

    //目录
    QString directory;
    std::vector<Mesh *> meshes;
    std::vector<Texture> textures_loaded;
};
#endif // !MODEL_H
