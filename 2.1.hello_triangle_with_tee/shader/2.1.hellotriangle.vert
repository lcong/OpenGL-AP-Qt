#version 410 core
layout(location =0 ) in vec3 aPos;
layout(location =1 ) in vec3 aColor;

void main(){
    gl_Position = vec4(aPos,1.0);
}
