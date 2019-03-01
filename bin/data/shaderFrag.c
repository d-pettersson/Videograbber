// fragment shader (where all the magic happens!)
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect camTex;
uniform sampler2DRect wifiTex;
uniform vec3 resolution;

uniform float time;

vec2 interlace(vec2 pos, float s) {
    pos.x += s * (4.0 * fract(pos.y * ))
}

void main() {
    vec2 pos = gl_TexCoord[0].xy;
    
    vec4 dispAmt = vec4(2, 2, 2, 0);
    
    vec4 wavyDispl = mix(vec4(1,0,0,1), vec4(0,1,0,1), (sin(pos.y * 10) * time * 20) / 2);
    
    //Displacement section
    vec2 displUV = (dispAmt.xy * 20) - (dispAmt.xy * 20);
    displUV += (dispAmt.zw * wavyDispl.r) - (dispAmt.zw * wavyDispl.g);
    
    float chromR = texture2DRect(camTex, pos + displUV).r;
    float chromG = texture2DRect(camTex, pos + displUV).g;
    float chromB = texture2DRect(camTex, pos + displUV).b;
    
    float wifiR = texture2DRect(wifiTex, pos).r;
    float wifiG = texture2DRect(wifiTex, pos).g;
    float wifiB = texture2DRect(wifiTex, pos).b;
    
    vec4 color = vec4(chromR, chromG, chromB, 1);
    
    gl_FragColor = color;
//    gl_FragColor = texture2DRect(camTex, pos);
}
