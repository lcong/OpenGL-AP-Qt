#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;


void main() {


fColor = gs_in[0].color;
gl_Position = gl_in[0].gl_Position + vec4(-0.2, +0.2, 0.0, 0.0);
EmitVertex();

gl_Position = gl_in[0].gl_Position + vec4(+0.2, -0.2, 0.0, 0.0);
EmitVertex();

fColor = gs_in[1].color;
gl_Position = gl_in[1].gl_Position + vec4( 0.0, -0.2, +0.2, 0.0);
EmitVertex();

gl_Position = gl_in[1].gl_Position + vec4( 0.0, +0.2, -0.2, 0.0);
EmitVertex();

fColor = gs_in[2].color;
gl_Position = gl_in[2].gl_Position + vec4(+0.2,  0.0, -0.2, 0.0);
EmitVertex();

gl_Position = gl_in[2].gl_Position + vec4(-0.2,  0.0, +0.2, 0.0);
EmitVertex();

fColor = vec3(1.0, 1.0, 1.0); //叠加白光
EmitVertex();
EndPrimitive();

}
