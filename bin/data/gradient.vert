#version 120

//varying vec3 N;
//varying vec3 v;
varying vec4 globalColor;
varying vec2 texcoord;

uniform float radius;

void main()
{
    //v = vec3(gl_ModelViewMatrix * gl_Vertex);
    //N = normalize(gl_NormalMatrix * gl_Normal);
    
    // from original gradient
    //texcoord = gl_MultiTexCoord0.st;
    
    // from corona
    texcoord = vec2 (gl_TextureMatrix[0] * gl_MultiTexCoord0);
    // vector position
    vec4 fragmentPos = gl_ModelViewMatrix * gl_Vertex;
    gl_Position = gl_ProjectionMatrix * fragmentPos;
    
}
