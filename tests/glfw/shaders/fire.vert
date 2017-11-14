
//
// fire.vert: Vertex shader for producing a fire effect
//
// author: Randi Rost
//
// Copyright (c) 2002: 3Dlabs, Inc.
//

varying float LightIntensity;
varying vec3  MCposition;
varying vec2  vTexCoord;

const vec3  LightPosition = vec3(10.0,10.0,10.0);
const float Scale=0.5;

void main(void)
{
    vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
    MCposition      = vec3 (gl_Vertex) * Scale;
    vec3 tnorm      = normalize(vec3 (gl_NormalMatrix * gl_Normal));
    LightIntensity  = dot(normalize(LightPosition - vec3 (ECposition)), tnorm) * 1.5;
    vTexCoord = gl_MultiTexCoord0.xy;
    gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
}
