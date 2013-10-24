#version 150

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in  vec4 vPosition;

void main() 
{
  gl_Position = projection * view * model * vPosition;
} 
