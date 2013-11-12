#version 150

uniform mat3 materialProperties;
uniform mat3 lightProperties;
uniform float shininess;
uniform sampler2D texture;

in vec3 fN;
in vec3 fL;
in vec3 fV;
in  vec2 fTexCoord;

out vec4 color;

void main() 
{ 
  // have to normalize after interpolation
  vec3 N = normalize(fN);
  vec3 L = normalize(fL);
  vec3 V = normalize(fV);
  
   vec4 texColor = texture2D( texture, fTexCoord );
  
  // get rows from material and light properties
  mat3 material = transpose(materialProperties);
  mat3 light = transpose(lightProperties);
  vec4 ambientLight = vec4(light[0], 1.0);
  vec4 diffuseLight = vec4(light[1], 1.0);
  vec4 specularLight = vec4(light[2], 1.0);
  vec4 ambientSurface = vec4(material[0], 1.0);
  vec4 diffuseSurface = vec4(material[1], 1.0);
  vec4 specularSurface = vec4(material[2], 1.0);
  
  //  Use texture as diffuse color
  //diffuseSurface = texColor;
  
   // Use texture to modulate diffuse color (and/or specular color)
  //diffuseSurface = diffuseSurface  * texColor;
  //diffuseSurface.a = 1.0;
  //specularSurface *= texColor;
  //specularSurface.a = 1.0;
  
  // blend with surface color using image alpha (defaults to 1 for RGB images)
  //float alpha = texColor.a;
  //diffuseSurface = (1 - alpha) * diffuseSurface + alpha * texColor;
  //diffuseSurface.a = 1.0;
  
  // use same texture in ambient light
  ambientSurface = diffuseSurface; 

  vec3 R = normalize(reflect(-L, N));
  //Or use: vec3 R = normalize(2 * (dot(L, N)) * N - L);
    
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
  
  float grey = (color.r + color.g + color.b) / 3;
  
  if (grey < .25)
  {
    color = vec4(0.0, 0.0, 1.0, 1.0);
  }
  else if (grey < .5)
  {
    color = vec4(0.0, 1.0, 0.0, 1.0);
  }
  else if (grey < .75)
  {
    color = vec4(1.0, 0.0, 0.0, 1.0);
  }
  else
  {
    color = vec4(1.0, 1.0, 0.0, 1.0);
  }
  
  color.a = 1.0;
} 

