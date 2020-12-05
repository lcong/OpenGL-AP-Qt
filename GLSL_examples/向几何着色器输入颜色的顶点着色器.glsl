#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

// 定义输出interface block
out VS_OUT
{
   vec3 vertColor;
}vs_out;

void main()
{
    gl_Position = vec4(position, 0.5, 1.0);
    gl_PointSize = 2.8; 
    vs_out.vertColor = color; 
}