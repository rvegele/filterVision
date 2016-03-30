#version 120

//varying vec3 v;
//varying vec3 N;

varying vec2 texcoord;
uniform sampler2DRect tex0;

uniform float radius;

uniform vec4 color;

uniform sampler2D tex1;
uniform float specSpalva;

void main() {
    vec3 spectrumCol = texture2D( tex1, vec2( specSpalva, 0.25 ) ).rgb;
    vec4 colorCast = color;
    //vec3 L = normalize(vec3(gl_ModelViewMatrix).xyz - v);
    //vec4 Idiff = vec4(0.75, 0.0, 0.0, 1.0)  * max(dot(N,L), 0.5); // 0?
    
    float i = texcoord.s;
    float j = texcoord.t;
    
    vec4 alpha;
    alpha.r = sqrt(( (i - 0.5)*(i - 0.5)) + ((j - 0.5)*(j - 0.5)));
    alpha.r = alpha.r / 0.40; //scale by half
    
    gl_FragColor.rgb = vec3(spectrumCol);//, color2, color.r);
    
    //gl_FragColor.r = 1.0;
    //gl_FragColor.g = 0.0;
    //gl_FragColor.b = 0.0;
    
    gl_FragColor.a = (1.0-alpha.r)* 0.2; //radius;
    
    //float invertedAlpha = (1.0 - alpha.r);
    //invertedAlpha = clamp(alpha.r, 0.00, 1.00);
    //float invertedAlphaDecreased = 0.20*(1.00 - invertedAlpha);
    //gl_FragColor.a =invertedAlphaDecreased;
}

