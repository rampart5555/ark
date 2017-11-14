
//
// fire.frag: Fragment shader for procedural fire effect
//
// author: Randi Rost
//
// Copyright (c) 2002: 3Dlabs, Inc.
//

varying float LightIntensity; 
varying vec3  MCposition;
varying vec2  vTexCoord;
uniform sampler2D Noise;

const vec3 Color1 = vec3(0.8, 0.7, 0.0);
const vec3 Color2 = vec3(0.6, 0.1, 0.0);
const float NoiseScale = 1.2;

uniform vec2 Offset;

void main (void)
{
    vec2 pos_1 = vec2(MCposition.x,MCposition.y);
    vec4 noisevec = texture2D(Noise, NoiseScale * (vTexCoord.st + Offset));


    float intensity = abs(noisevec.x - 0.25) +
                      abs(noisevec.y - 0.125) +
                      abs(noisevec.z - 0.0625) +
                      abs(noisevec.w - 0.03125);

    intensity    = clamp(intensity * 6.0, 0.0, 1.0);
    vec3 color   = mix(Color1, Color2, intensity) * LightIntensity;
    color = clamp(color, 0.0, 1.0);
    //gl_FragColor = vec4 (color, 1.0);

	gl_FragColor = noisevec;
}
