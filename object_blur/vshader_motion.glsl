#version 150

   uniform mat4 mvp;
   uniform mat4 prevMvp;

   in vec4 vPosition;

   out vec4 position;
   out vec4 prevPosition;

   void main(void)
   {
      position = mvp * vPosition;
      prevPosition = prevMvp * vPosition;

      gl_Position = position;
   }