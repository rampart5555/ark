varying vec2  vTexCoord;
varying vec4  aColor;
void main(void)
{ 
    aColor = gl_Color;
    vTexCoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
