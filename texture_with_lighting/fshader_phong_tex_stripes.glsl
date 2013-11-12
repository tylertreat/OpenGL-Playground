//
// Shader that uses texture coordinates to make blue and yellow stripes.
// Illustrates simple antialiasing with the smoothstep() function.
//
#version 150

uniform mat3 materialProperties;
uniform mat3 lightProperties;
uniform float shininess;
uniform sampler2D texture;  // not used

in vec3 fN;
in vec3 fL;
in vec3 fV;
in  vec2 fTexCoord;

out vec4 color;

void main() 
{ 
 // get rows from material and light properties
  mat3 material = transpose(materialProperties);
  mat3 light = transpose(lightProperties);
  vec4 ambientLight = vec4(light[0], 1.0);
  vec4 diffuseLight = vec4(light[1], 1.0);
  vec4 specularLight = vec4(light[2], 1.0);
  vec4 ambientSurface = vec4(material[0], 1.0);
  vec4 diffuseSurface = vec4(material[1], 1.0);
  vec4 specularSurface = vec4(material[2], 1.0);


  // procedural texture - blue with yellow stripes
  float scale = 5.0;
  float fuzz = 0.02;
  
  // blue, yellow
  vec4 backcolor = vec4(0.0, 0.0, 1.0, 1.0);
  vec4 frontcolor = vec4(1.0, 1.0, 0.0, 1.0);
  
  // Scale the t-coordinate of texture, then take fractional part
  // only (to get stripes)
  float scaledT = fract(fTexCoord.t * scale);
  //float scaledT = fract((gl_FragCoord.y / 2 + 0.5) * scale * 3);
  
#if 1 
  // without antialiasing
  // yellow on the first half of interval, blue on second half
  if (scaledT < 0.5)
  {
    diffuseSurface = frontcolor;
  }
  else
  {
    diffuseSurface = backcolor;
  }
#endif

  // m is zero for scaledT < 0.5 and 1 for scaledT > 0.5, where smoothstep
  // is used for the transitions at 0, .5, and 1 of +/- fuzz
#if 0
  float m;
  if (scaledT < .25)
  {
    m = 1.0 - smoothstep(-fuzz, fuzz, scaledT);
  }
  else if (scaledT > .75)
  {
    m = 1.0 - smoothstep(1 - fuzz, 1 + fuzz, scaledT);
  }
  else
  {
    m = smoothstep(0.5 - fuzz, 0.5 + fuzz, scaledT);
  }
  diffuseSurface = mix(frontcolor, backcolor, m);
#endif  
  diffuseSurface.a = 1.0;
  ambientSurface = diffuseSurface;
  specularSurface = vec4(1.0, 1.0, 1.0, 1.0);
  
  // have to normalize after interpolation
  vec3 N = normalize(fN);
  vec3 L = normalize(fL);
  vec3 V = normalize(fV);
  vec3 R = normalize(reflect(-L, N));
  
  // multiply material by light 
  vec4 ambientProduct = ambientLight * ambientSurface;
  vec4 diffuseProduct = diffuseLight * diffuseSurface;
  vec4 specularProduct = specularLight * specularSurface;  
  
  // ambient intensity
  vec4 ia = ambientProduct;
  
  // diffuse intensity
  float diffuseFactor = max(dot(L, N), 0.0);
  vec4 id = diffuseFactor * diffuseProduct;
  
  // specular intensity
  vec4 is = vec4(0.0, 0.0, 0.0, 1.0);  
  if (dot(L, N) >= 0.0) 
  {
    float specularFactor = pow(max(dot(R, V), 0.0), shininess); 
    is = specularFactor * specularProduct;
  }
  
  color = ia + id + is;
  color.a = 1.0;

} 

