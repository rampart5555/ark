const vec3 cLightPos = vec3(5.0,5.0, 10.0);
const float cSpecularContribution = 0.1;
const float cDiffuseContribution  = (1.0 - cSpecularContribution);

varying float vLightIntensity;
varying vec2 vTexCoord;


void main(void) 
{
    vec4 pos        = gl_ModelViewMatrix * gl_Vertex;
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(cLightPos - vec3(pos));
    vec3 reflectVec = reflect(lightVec, tnorm);
    vec3 viewVec    = normalize(vec3(pos));

    float spec = max(dot(reflectVec, viewVec), 0.0);
    spec = pow(spec, 16.0);

    vLightIntensity = cDiffuseContribution * dot(lightVec, tnorm) +
                     cSpecularContribution * spec;

    vTexCoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}


