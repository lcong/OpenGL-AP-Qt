#version 330 core
layout(points) in ;
layout(points, max_vertices = 1) out;

in gl_PerVertex
{
    vec4  gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

// 直通的几何着色器 原样输出
void main()
{
    gl_Position = gl_in[0].gl_Position;
    gl_PointSize = gl_in[0].gl_PointSize;
    EmitVertex();
    EndPrimitive();
}