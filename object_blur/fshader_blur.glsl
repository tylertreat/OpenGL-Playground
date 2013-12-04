#version 150

   uniform sampler2D uTexInput;    // texture we're blurring
   uniform sampler2D uTexVelocity; // velocity buffer
   
   uniform float uVelocityScale;   // currentFPS / targetFPS

   out vec4 oResult;

   const int MAX_SAMPLES = 32;

   void main(void) {
      vec2 texelSize = 1.0 / vec2(textureSize(uTexInput, 0));
      vec2 screenTexCoords = gl_FragCoord.xy * texelSize;

      vec2 velocity = texture2D(uTexVelocity, screenTexCoords).xy;
      velocity *= uVelocityScale;

	  float speed = length(velocity / texelSize);
      int nSamples = clamp(int(speed), 1, MAX_SAMPLES);

	  // Blur by averaging the samples
	  oResult = texture2D(uTexInput, screenTexCoords);
      for (int i = 1; i < nSamples; ++i) {
          vec2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
          oResult += texture2D(uTexInput, screenTexCoords + offset);
      }
      oResult /= float(nSamples);
   }