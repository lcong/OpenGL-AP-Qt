#version 330 core
layout(location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.5, 1.0);
    gl_PointSize = 2.8; // 指定点大小 需要在主程序中开启 glEnable(GL_PROGRAM_POINT_SIZE); 
}