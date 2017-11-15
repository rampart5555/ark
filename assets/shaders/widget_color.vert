#version 100 

#pragma import_defines ( WIDGET_GEOM, WIDGET_TEXT)

varying vec4 vColor;
varying vec2 vTexCoord0; 
uniform float uAlpha;

void main(void) 
{	
#if defined(WIDGET_GEOM)
	vColor = vec4(1.0, 0.0, 0.0, uAlpha);		
#elif defined(WIDGET_TEXT)
	vColor = vec4(1.0, 1.0, 1.0, uAlpha);	
#else 
    vColor = vec4(0.5, 0.5, 0.5, uAlpha);
#endif	
	vTexCoord0 = gl_MultiTexCoord0.xy; 
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}

