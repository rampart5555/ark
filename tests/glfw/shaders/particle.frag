varying vec4  aColor;
varying vec2  vTexCoord;
uniform sampler2D uSampler;

void main (void)
{
    //vec4 diffuse = vec4 (1.0,0.0,0.0,1.0);    
    //vec4 color=clamp( diffuse * texture2D(uSampler, vTexCoord.st), 0.0, 1.0);
    vec4 texcolor = texture2D(uSampler, vTexCoord.st);
    gl_FragColor = aColor*texcolor.aaaa;
}

