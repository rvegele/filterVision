#version 120

varying vec3 N;
varying vec3 v;

//uniform float color;
uniform vec4 color;

uniform float clmpMin; // default is 0.0;
uniform float clmpMax; // default is 1.0;
uniform float dtt; // default is 0.0

uniform sampler2D tex1;
varying vec2 texcoord;
uniform float specSpalva;

void main(void) {
    
    vec3 spectrumCol = texture2D( tex1, vec2( specSpalva, 0.25 ) ).rgb;
    
    //vec4 colorCast = color;
    
    vec4 colorCast = vec4(spectrumCol, 1.0);
    
    //vec4 color = vec4(0.92, 0.82, 0.34, 1.0);
    
    // light direction - camera
    vec3 L = normalize(vec3(gl_ModelViewMatrix).xyz - v);
    
    vec4 Idiff = colorCast  * max(dot(N,L), dtt); // 0?
    
    // fancy stuff
    float ppEyeDiff		= max( dot( N, L ), 0.0 );
    float ppEyeFres		= pow( 1.0 - ppEyeDiff, 2.0 );
    
    // simple
    Idiff = clamp(Idiff, clmpMin, clmpMax);
    
    // fancy
    vec3 darkRoomColor	= vec3( ppEyeDiff + 0.5 ) * 0.7 + colorCast.rgb * 0.4 + 0.4 + pow( ppEyeFres, 2.0 ) * 0.4;
    
    // simple
    //gl_FragColor.rgb = Idiff.rgb;
    //gl_FragColor.rgb = vec3(ppEyeFres);
    
    gl_FragColor.rgb = darkRoomColor * Idiff.rgb;
    gl_FragColor.a = 1.0;
}