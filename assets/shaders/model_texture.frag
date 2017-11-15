#version 100

precision mediump float;

varying float vLightIntensity;
varying vec2 vTexCoord;
uniform sampler2D uSampler; 

void main (void)
{

    vec3 lightColor = vec3(texture2D(uSampler, vTexCoord)) * vLightIntensity;
    vec3 ct = clamp(lightColor, 0.0, 1.0);
    //gl_FragColor = vec4 (ct, 1.0);
    gl_FragColor = texture2D(uSampler, texcoord).aaaa;
}

/*
const vec3 cLightPos = vec3(5.0, 5.0, 5.0);
const vec4 cLightColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 cMatDiff = vec4(1.0, 0.0, 0.0, 1.0);
varying vec3 vModel;
varying vec3 vNormal;

void main (void)
{
    vec4 MatAmb = vec4(0.1, 0.1, 0.1,1.0)*cMatDiff;
    vec3 L = normalize(cLightPos - vModel);
    vec4 Idiff = MatAmb + cMatDiff *(cLightColor * max(dot(vNormal,L), 0.0));
    Idiff = clamp(Idiff, 0.0, 1.0); 
    gl_FragColor = Idiff;
}
*/


