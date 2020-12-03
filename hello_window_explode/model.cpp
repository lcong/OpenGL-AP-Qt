#include "model.h"
#include <string>
#include <QImage>
#include <QDebug>
#include <QSharedPointer>
#include <QOpenGLTexture>
static QSharedPointer<QOpenGLTexture> textureFromFile(const QString& path, const QString& directory)
{
    QString fileName = directory + '/' + path;

    QImage image(fileName);
    QSharedPointer<QOpenGLTexture> texture(new QOpenGLTexture(image));

    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    return (texture);
}


Model::Model(const QString& path)
{
    //Clear();
    if (!loadModel(path)) {
        qDebug() << "Model init failed!";
    }
}


Model::~Model()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
    meshes.clear();
}


bool Model::loadModel(const QString& path)
{
    if (path.isEmpty()) {
        return (false);
    }
    // 通过ASSIMP读文件
    Assimp::Importer importer;
    const aiScene *pscene = importer.ReadFile(path.toLocal8Bit().constData(), aiProcess_Triangulate | aiProcess_FlipUVs);
    // 检查错误
    if (!pscene || pscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pscene->mRootNode) { // 如果不是0
        qDebug() << "error::ASSIMP:: " << importer.GetErrorString() << endl;
        return (false);
    }
    //返回路径字符串最后一次出现'/'的最左边的字符串，也就是文件的目录。
    directory = path.left(path.lastIndexOf('/'));
    bool ret(false);
    if (pscene) {
        //如果采用前两行代码，遍历所有的网格
        //如果采用现在没注释的代码，表示通过父类遍历子类。具有父子关系
        //int a = pscene->mRootNode->mNumChildren;
        //ret = initFromScene(pscene, path);
        processNode(pscene->mRootNode, pscene);
    }
    return (ret);
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh *Model::processMesh(aiMesh *pmesh, const aiScene *pscene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    //打印网格的顶点数值
    qDebug() << "Vertices = " << pmesh->mNumVertices;
    //是否存在顶点数据
    if (pmesh->HasPositions()) {
        for (unsigned int i = 0; i < pmesh->mNumVertices; i++)
        {
            Vertex vertex;
            QVector3D vector;
            //位置
            vector.setX(pmesh->mVertices[i].x);
            vector.setY(pmesh->mVertices[i].y);
            vector.setZ(pmesh->mVertices[i].z);
            vertex.Position = vector;
            //法线是否存在？
            if (pmesh->HasNormals()) {
                vector.setX(pmesh->mNormals[i].x);
                vector.setY(pmesh->mNormals[i].y);
                vector.setZ(pmesh->mNormals[i].z);
                vertex.Normal = vector;
            }

            //// 纹理坐标是否存在
            //if (pmesh->HasTextureCoords(i))
            //{

            //}
            // 顶点最多可包含8个不同的纹理坐标。我们只关心第一个纹理坐标
            if (pmesh->mTextureCoords[0]) {
                QVector2D vec;
                vec.setX(pmesh->mTextureCoords[0][i].x);
                vec.setY(pmesh->mTextureCoords[0][i].y);
                vertex.TexCoords = vec;
            }else  {
                vertex.TexCoords = QVector2D(0.0f, 0.0f);
            }

            ////UV向量是否存在？
            //if (pmesh->HasTangentsAndBitangents())
            //{
            //	// u向量
            //	vector.setX(mesh->mTangents[i].x);
            //	vector.setY(mesh->mTangents[i].y);
            //	vector.setZ(mesh->mTangents[i].z);
            //	vertex.Tangent = vector;
            //	// v向量
            //	vector.setX(mesh->mBitangents[i].x);
            //	vector.setY(mesh->mBitangents[i].y);
            //	vector.setZ(mesh->mBitangents[i].z);
            //}
            vertices.push_back(vertex);
        }
    }
    //现在遍历每个网格面（一个面是一个三角形的网格）并检索相应的顶点索引。
    for (unsigned int i = 0; i < pmesh->mNumFaces; i++)
    {
        aiFace face = pmesh->mFaces[i];
        // 检索面的所有索引并将它们存储在索引向量中
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //场景中包含材质？
    if (pscene->HasMaterials()) {
        aiMaterial *material = pscene->mMaterials[pmesh->mMaterialIndex];
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    return (new Mesh(vertices, indices, textures));
}


bool Model::initFromScene(const aiScene *pscene, const QString& filename)
{
    bool ret(true);

    //场景中包含动画？
    if (pscene->HasAnimations()) {
        for (unsigned int i = 0; i < pscene->mNumAnimations; i++)
        {
            const aiAnimation *panimation = pscene->mAnimations[i];
        }
    }
    //场景中包含相机？
    if (pscene->HasCameras()) {
        //int a = 10;
    }
    //场景中包含光照？
    if (pscene->HasLights()) {
        //int a = 10;
    }

    //场景中包含网格？
    if (pscene->HasMeshes()) {
        for (unsigned int i = 0; i < pscene->mNumMeshes; i++)
        {
            const aiMesh *pmesh = pscene->mMeshes[i];
            Mesh *me = initMesh(pscene, pmesh);
            meshes.push_back(me);
        }
    }
    //场景中包含纹理？
    if (pscene->HasTextures()) {
        int a = 10;
    }

    return (ret);
}


void Model::draw(QOpenGLShaderProgram *program)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->draw(program);
    }
}


Mesh *Model::initMesh(const aiScene *pscene, const aiMesh *pmesh)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    //打印网格的顶点数值
    qDebug() << "Vertices = " << pmesh->mNumVertices;
    //是否存在顶点数据
    if (pmesh->HasPositions()) {
        for (unsigned int i = 0; i < pmesh->mNumVertices; i++)
        {
            Vertex vertex;
            QVector3D vector;
            //位置
            vector.setX(pmesh->mVertices[i].x);
            vector.setY(pmesh->mVertices[i].y);
            vector.setZ(pmesh->mVertices[i].z);
            vertex.Position = vector;
            //法线是否存在？
            if (pmesh->HasNormals()) {
                vector.setX(pmesh->mNormals[i].x);
                vector.setY(pmesh->mNormals[i].y);
                vector.setZ(pmesh->mNormals[i].z);
                vertex.Normal = vector;
            }

            //// 纹理坐标是否存在
            //if (pmesh->HasTextureCoords(i))
            //{

            //}
            // 顶点最多可包含8个不同的纹理坐标。我们只关心第一个纹理坐标
            if (pmesh->mTextureCoords[0]) {
                QVector2D vec;
                vec.setX(pmesh->mTextureCoords[0][i].x);
                vec.setY(pmesh->mTextureCoords[0][i].y);
                vertex.TexCoords = vec;
            }else  {
                vertex.TexCoords = QVector2D(0.0f, 0.0f);
            }

            ////UV向量是否存在？
            //if (pmesh->HasTangentsAndBitangents())
            //{
            //	// u向量
            //	vector.setX(mesh->mTangents[i].x);
            //	vector.setY(mesh->mTangents[i].y);
            //	vector.setZ(mesh->mTangents[i].z);
            //	vertex.Tangent = vector;
            //	// v向量
            //	vector.setX(mesh->mBitangents[i].x);
            //	vector.setY(mesh->mBitangents[i].y);
            //	vector.setZ(mesh->mBitangents[i].z);
            //}
            vertices.push_back(vertex);
        }
    }
    //现在遍历每个网格面（一个面是一个三角形的网格）并检索相应的顶点索引。
    for (unsigned int i = 0; i < pmesh->mNumFaces; i++)
    {
        aiFace face = pmesh->mFaces[i];
        // 检索面的所有索引并将它们存储在索引向量中
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //场景中包含材质？
    if (pscene->HasMaterials()) {
        aiMaterial *material = pscene->mMaterials[pmesh->mMaterialIndex];
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    return (new Mesh(vertices, indices, textures));
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, const QString& typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip(false);
        QString path(str.C_Str());
        path = path.right(path.length() - path.lastIndexOf('\\') - 1);
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            //路径相等打破这个循环
            if (textures_loaded[j].filename == path) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.filename = path;
            texture.type = typeName;
            texture.texture = textureFromFile(path, directory);
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return (textures);
}
