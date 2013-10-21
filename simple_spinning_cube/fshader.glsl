#version 150

in  vec4 fColor;
out vec4 color;

void main() 
{ 
    //color = vec4(1 - gl_FragCoord.z, 0, 0, 1);
    color = fColor;
    //color = vec4(1.0, 0,0,1);
    
} 

