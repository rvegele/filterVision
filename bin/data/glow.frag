uniform sampler2D tex0; // smoke puffs
uniform sampler2D tex1; // spectum color
uniform sampler2D tex2; // small cross

uniform float radius;
uniform float alpha;

varying vec4 vVertex;

varying vec2 texcoord;
uniform float specSpalva;  // spectrum pick

uniform float randomas;

uniform float angle;


void main(){
    vec2 coord = gl_TexCoord[0].st;
    float sin_factor = sin(angle);
    float cos_factor = cos(angle);
    coord = (coord - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
    coord += 0.5;
    /*
    vec2 v2 = gl_TexCoord[0].st;
    float cosX, sinX;
    float cosY, sinY;
    sinX = sin(angle);
    cosX = cos(angle);
    sinY = sin(angle);
    cosY = cos(angle);
    mat2 rotationMatrix = mat2( cosX, -sinX, sinY, cosX);
    vec2 offset = vec2(.5, .5);
    vec2 newcoords = ((v2-offset) * (rotationMatrix));
    newcoords += offset;
    */
    
    // FADE NEBULA CLOSE TO STAR SURFACE TO PREVENT VISIBLE CLIPPING
    float dist = length( vVertex.xyz );
    if( dist < radius * 0.5 ) discard;
    
    float maxAlpha = 1.0;
    float distThresh = (radius * 0.5) * 1.05; //  110
    float distDelta  = distThresh - (radius * 0.5); // 10
    if ( dist < distThresh ){
        maxAlpha = ( dist - (radius * 0.5) ) / distDelta;
    }
    // END FADE
    
    
    vec4 glowCol = texture2D(tex0,coord);
    vec3 spectrumCol = texture2D(tex1, vec2( specSpalva+ 0.05, 0.25 ) ).rgb;
    vec4 smallCrossTexture = texture2D(tex2, coord);
    
    //float nebulaAlpha	= smokeTexture.a * alpha;
    float glowAlpha		= glowCol.a * alpha * maxAlpha;
    
    //vec3 offColor		= vec3( mix( smallCrossTexture.r, 1.0, nebulaAlpha ) );
    vec3 onColor		= vec3( spectrumCol );
    
    
    float offAlpha		= clamp( smallCrossTexture.a, 0.0, 1.0 );
    float onAlpha		= glowAlpha;
    
    /* a cross instead of a smoke cloud - for debug? :)
     float offAlpha		= clamp( smallCrossTexture.a, 0.0, 1.0 );
     
     // not needed?
     //vec3 offColor		= vec3( mix( gridCol.r, power, nebulaAlpha ) );
     */
    
    // change to ON for production rendering
    gl_FragColor.rgb = onColor;
    gl_FragColor.a = onAlpha;
    //gl_FragColor.a	= smallCrossTexture.a;//onAlpha;
    
    
    //gl_FragColor.rgb = vec3(spectrumCol);
    //gl_FragColor.a = onAlpha; //coronaa.a;
}



//vec3 spectrumCol	= texture2D( spectrumTex, vec2( color, 0.25 ) ).rgb; // spectrum
//vec4 nebulaCol		= texture2D( nebulaTex, gl_TexCoord[0].st );     // smoke
//vec4 gridCol		= texture2D( gridTex, gl_TexCoord[0].st );           // cross





