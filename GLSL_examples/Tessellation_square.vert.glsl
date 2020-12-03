#version 400 core
in  vec4  vPosition;
void main()
{
    //gl_Position is used to access the vertex position that is input from application
    gl_Position = vPosition;
}