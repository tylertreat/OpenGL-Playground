#version 150

//
// Shader for per-fragment lighting calculation
//

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform vec4 lightPosition;



in  vec4 vPosition;
in vec3 vNormal;
out vec3 fN;
out vec3 fL;
out vec3 fV;

void main() 
{
  // See vshader_gouraud.glsl for a detailed explanation of 
  // these calculations.  This is the same, but we put the values
  // in 'out' variables. We don't bother to normalize since we'll
  // have to normalize in the fragment shader anyway.

  fN = normalMatrix * vNormal;
  fL = (view * lightPosition - view * model * vPosition).xyz;
  fV = -(view * model * vPosition).xyz;
  
  gl_Position = projection * view * model * vPosition;
} 
