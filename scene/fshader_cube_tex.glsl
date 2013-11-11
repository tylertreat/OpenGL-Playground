#version 150

uniform samplerCube textureCube;

in  vec3 fTexCoord;
out vec4 color;

void main() 
{ 
    // set to the color sampled from the texture
    color = texture( textureCube, fTexCoord );  
} 

