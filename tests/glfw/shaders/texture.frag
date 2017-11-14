
//precision mediump float;

varying float vLightIntensity;
varying vec2 vTexCoord;
uniform sampler2D uSampler; 

void main (void)
{
    vec4 color = (texture2D(uSampler, vTexCoord));
    vec3 lightColor = vec3(texture2D(uSampler, vTexCoord)) * vLightIntensity;
    vec3 ct = clamp(lightColor, 0.0, 1.0);
    gl_FragColor = vec4 (ct, color.a);
    //gl_FragColor = texture2D(uSampler, vTexCoord).aaaa;
}

