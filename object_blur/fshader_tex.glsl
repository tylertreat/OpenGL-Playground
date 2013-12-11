#version 150

uniform sampler2D tex;
in  vec2 fTexCoord;
out vec4 color;

void main() 
{ 
  color = texture2D( tex, fTexCoord );  
} 