#version 450 core
layout(location =0 ) in vec3 vs_p;
layout(location =1 ) in vec3 vs_cd;
out vec3 vs_color;
void main(){
    gl_Position = vec4(vs_p,1.0);
    vs_color = vs_cd;
}
