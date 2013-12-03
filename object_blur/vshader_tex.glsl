#version 150

in  vec2 vPosition;
in  vec2 texCoord;
out vec2 fTexCoord;

void main() 
{
  // pass to fragment shader as varying
  fTexCoord = texCoord;
  gl_Position = vec4(vPosition, 0.0, 1.0);
} 