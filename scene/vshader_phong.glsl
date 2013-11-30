#version 150

//
// Shader for per-fragment lighting calculation with texture coordinate
//

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 prevModel;
uniform mat4 prevView;
uniform mat4 prevProjection;

uniform mat3 normalMatrix;
uniform vec4 lightPosition;

in  vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fN;
out vec3 fL;
out vec3 fV;
out vec2 fTexCoord;

out vec4 pos;
out vec4 prevPos;

void main() 
{
  fN = normalMatrix * vNormal;
  fL = (view * lightPosition - view * model * vPosition).xyz;
  fV = -(view * model * vPosition).xyz;
  
  fTexCoord = vTexCoord;
  pos = projection * view * model * vPosition;
  prevPos = prevProjection * prevView * prevModel * vPosition;
  gl_Position = pos;
} 
