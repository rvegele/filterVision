#version 120

attribute vec4 position;
varying vec2 texcoord;

uniform float radius;
uniform float coronaRadius;

void main()
{
    
    vec4 vVertex = vec4(gl_Vertex);
    vVertex.xyz *= radius*coronaRadius;
    //vVertex.xyz *= coronaRadius; //corona multiplier
    
    gl_Position = gl_ModelViewProjectionMatrix * vVertex;
    
    // does not work
    //gl_Position = ftransform();
    texcoord = vec2 (gl_TextureMatrix[0] * gl_MultiTexCoord0);

}