#version 150

//
// Shader for per-vertex lighting calculation
//

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform vec4 lightPosition;

uniform mat3 materialProperties;
uniform mat3 lightProperties;
uniform float shininess;
uniform bool useHalfVector;

in  vec4 vPosition;
in vec3 vNormal;
out vec4 fColor;

void main() 
{
  // for lighting calculation do everything in eye coordinates
  
  // Transform normal vectors into eye coordinates using normal matrix
  vec3 N = normalize(normalMatrix * vNormal);

  // L vector is from vertex position to light position (which is 
  // initially given in world coords)
  vec4 vertexPositionInEyeCoords = view * model * vPosition;
  vec4 lightPositionInEyeCoords = view * lightPosition;
  vec3 L = normalize((lightPositionInEyeCoords - vertexPositionInEyeCoords).xyz);

  // In eye coordinates, view vector is just a vector from vertex position
  // to eye coordinate origin (0, 0, 0)
  vec3 V = normalize(-(view * model * vPosition).xyz);
  
  // Calculate reflected vector using built-in GLSL function, 
  // or use: vec3 R = normalize(2 * (dot(L, N)) * N - L);
  vec3 R = normalize(reflect(-L, N));
  
  // alternative calculation uses "half" vector
  vec3 h = normalize(L + V);
    
  // multiply material properties by light properties using "componentwise"
  // matrix multiplication, and then extract the rows
  // Rows are A, D, and S, and columns are R, G, and B components
  // Note that products[i] is ith *column* in GLSL, which is why we
  // have to transpose.
  // (This is just a shortcut to save having to do 9 separate multiplications.)
  mat3 products = transpose(matrixCompMult(lightProperties, materialProperties));
  vec4 ambientColor = vec4(products[0], 1);
  vec4 diffuseColor = vec4(products[1], 1);
  vec4 specularColor = vec4(products[2], 1);

  // ambient intensity
  vec4 ia = ambientColor;
  
  // diffuse intensity
  float diffuseFactor = max(dot(L, N), 0.0);
  vec4 id = diffuseFactor * diffuseColor;
  
  // specular intensity
  // This can be calculated with R dot V or using the alternative calculation 
  // N dot h.  This changes effect of the specular exponent.
  vec4 is = vec4(0.0, 0.0, 0.0, 1.0);  
  if (dot(L, N) >= 0.0) 
  {
    float specularFactor;
    if (useHalfVector)
    {
      specularFactor = pow(max(dot(N, h), 0.0), shininess);    
    }
    else
    {
      specularFactor = pow(max(dot(R, V), 0.0), shininess);
    }
    is = specularFactor * specularColor;
  }
  
  fColor = ia + id + is;
  fColor.a = 1.0;

  // after all that, don't forget to set the vertex position!
  gl_Position = projection * view * model * vPosition;
} 
