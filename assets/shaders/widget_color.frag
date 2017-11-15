#version 100
precision mediump float;

varying vec4 vColor;
varying vec2 vTexCoord0; 
uniform sampler2D GlyphTexture; 

void main(void)
{
    gl_FragColor = vColor * texture2D(GlyphTexture, vTexCoord0).aaaa; 
}


