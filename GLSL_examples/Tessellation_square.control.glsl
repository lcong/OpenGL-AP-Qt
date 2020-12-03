#version 400 core
//The main purpose of tessellation control shader is:
//Generate the tessellation output patch vertices that are passed to the tessellation
//evaluation shader, as well as update any per-vertex
//Define how many vertices will be used as one patch
layout (vertices = 4) out;
//uniform type is used to define the variable that can be used to communicate between shader and application
uniform float inner;
uniform float outer;
void main()
{
    //gl_TessLevelInner is used to define:
    //how the interior of the domain is subdivided and stored in a two element array named    
    gl_TessLevelInner[0] = inner;
    gl_TessLevelInner[1] = inner;
   
    //gl_TessLevelOuter is used to define:
    //how the perimeter of the domain is subdivided, and is stored in an
    //implicitly declared four-element array
    gl_TessLevelOuter[0] = outer;
    gl_TessLevelOuter[1] = outer;
    gl_TessLevelOuter[2] = outer;
    gl_TessLevelOuter[3] = outer;
   
    //gl_in is used to access the number of elements that are define by glPatchParameteri()
    //glPatchParameteri() define how many vertices as a patch
    //gl_out is used to access the output vertex position of tessellation control shader
    //gl_in vertex shader structure below:
    //in gl_PerVertex {
    //        vec4 gl_Position;
    //        float gl_PointSize;
    //        float gl_ClipDistance[]
    //} gl_in[gl_PatchVerticesIn];   
    //gl_out vertex shader structure is similar to gl_in structure
    //gl_InvocationID is used to access the specific vertex of a patch

    gl_out[gl_InvocationID].gl_Position =  gl_in[gl_InvocationID].gl_Position;
}