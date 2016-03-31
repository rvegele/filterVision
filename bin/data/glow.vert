#version 120

//attribute vec4 position;
varying vec2 texcoord;

uniform float radius;
uniform float nebulaRadius;
uniform float randomas;
uniform float pointSize;

varying vec4 vVertex;

void main()
{
    
    //vVertex = vec4(gl_Vertex);
    vVertex = gl_Vertex;
    //vVertex.xyz *= radius;
    
    //vVertex.xyz *= nebulaRadius;
    
    //gl_Position = gl_ModelViewProjectionMatrix * vVertex;
    
    // does not work
    //gl_Position = gl_ModelViewMatrix * gl_Vertex;
    
    // does not work
    gl_Position = ftransform();
    gl_PointSize = pointSize;
    
    texcoord = vec2 (gl_TextureMatrix[0] * gl_MultiTexCoord0);
    
}

/*
 void main() {
 gl_TexCoord[0] = gl_MultiTexCoord0;
 vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
 gl_Position = gl_ProjectionMatrix * eyeCoord;
 
 float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
 float att	 = 600.0 / dist;
 
 gl_PointSize = gl_Normal.x * att;
 //gl_FrontColor = gl_Color;
 }
 */