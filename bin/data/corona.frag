uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 color;

varying vec2 texcoord;
uniform float specSpalva;

void main(){
    
    vec4 coronaa = texture2D(tex0,texcoord);
    vec3 spectrumCol = texture2D( tex1, vec2( specSpalva, 0.25 ) ).rgb;
    
    // scroll
    vec3 col1 = spectrumCol;
    
    vec4 colorCast = color;
    // blend layers
    //vec3 col2 = vec3(0.0, 0.0, 1.0);
    
    gl_FragColor.rgb = vec3(spectrumCol);
    //gl_FragColor.rgb = mix(col1,col2,coronaa.a);
    gl_FragColor.a = coronaa.a;
}

