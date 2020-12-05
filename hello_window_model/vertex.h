#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>

//顶点类
struct Vertex {
    QVector3D	Position;       //位置
    QVector3D	Normal;         //法向量
    QVector2D	TexCoords;      //纹理坐标
    QVector3D	Tangent;        //u向量
    QVector3D	Bitangent;      //v向量
};
#endif  // !VERTEX_H
