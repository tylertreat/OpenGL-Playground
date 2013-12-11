#version 150


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
in  vec4 vPosition;
out vec3 fTexCoord;

void main() 
{
  // texture coordinate for cube map is just a normalized vector pointing
  // to the vertex
  fTexCoord = normalize((vPosition).xyz);
  gl_Position = projection * view * model * vPosition;
} 
