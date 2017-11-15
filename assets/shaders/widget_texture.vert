#version 100 
varying vec4 vColor;
varying vec2 vTexCoord0; 
uniform float uAlpha;

void main(void) 
{	
	vColor = vec4(1.0, 1.0, 1.0, uAlpha);	
	vTexCoord0 = gl_MultiTexCoord0.xy; 
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}

