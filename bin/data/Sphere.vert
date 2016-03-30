#version 120

varying vec3 N;
varying vec3 v;

uniform float radius;

varying vec2 texcoord;

void main(void)
{
    
    texcoord = vec2 (gl_TextureMatrix[0] * gl_MultiTexCoord0);
    
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    N = normalize(gl_NormalMatrix * gl_Normal);
    
    vec4 vVertex = vec4(gl_Vertex);
    
    vVertex.xyz *= radius;
    
    gl_Position = gl_ModelViewProjectionMatrix * vVertex;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
