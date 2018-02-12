#version 100

varying vec3 vModel;
varying vec3 vNormal;

void main(void) 
{  
    vModel = vec3(gl_ModelViewMatrix * gl_Vertex);
    vNormal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
