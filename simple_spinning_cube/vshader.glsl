#version 150

uniform mat4 transform;
in  vec4 vPosition;
in  vec4 vColor;
out vec4 fColor;

void main() 
{
  gl_Position = transform * vPosition;
  fColor = vColor;
} 
