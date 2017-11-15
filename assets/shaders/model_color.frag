#version 100 
precision mediump float;

const vec3 cLightPos = vec3(5.0, 5.0, 5.0);
const vec4 cLightColor = vec4(1.0, 1.0, 1.0, 1.0);

varying vec3 vModel;
varying vec3 vNormal;
uniform vec4 uEntityDiffuseColor;

void main (void)
{
    vec4 MatAmb = vec4(0.1, 0.1, 0.1,1.0)*uEntityDiffuseColor;
    vec3 L = normalize(cLightPos - vModel);
    vec4 Idiff = MatAmb + uEntityDiffuseColor*(cLightColor * max(dot(vNormal,L), 0.0));
    Idiff = clamp(Idiff, 0.0, 1.0); 
    gl_FragColor = Idiff;
}


