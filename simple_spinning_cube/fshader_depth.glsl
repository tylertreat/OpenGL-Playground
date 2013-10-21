#version 150

in  vec4 fColor;
out vec4 color;

void main() 
{ 
  vec4 cyan = vec4(0.0, 1.0, 1.0, 1.0);
  color = (1 - gl_FragCoord.z) * cyan;
} 

