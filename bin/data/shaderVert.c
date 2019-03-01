// dummy vertex shader
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

varying vec2 uv;

void main() {
//    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
//    gl_TexCoord[0] = gl_MultiTexCoord0;
//    gl_FrontColor = gl_Color;
    
    uv = gl_MultiTexCoord0.xy;
    gl_Position = ftransform();
    
}
