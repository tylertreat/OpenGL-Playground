#version 150
  
   in vec4 position;
   in vec4 prevPosition;

   out vec2 oVelocity;

   void main(void) {
      vec2 a = (position.xy / position.w) * 0.5 + 0.5;
      vec2 b = (prevPosition.xy / prevPosition.w) * 0.5 + 0.5;
      oVelocity = a - b;
   }