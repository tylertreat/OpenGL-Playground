#version 150
  
in vec4 position;
in vec4 prevPosition;

out vec2 velocity;

void main(void)
{
    // Perform perspective divide and scale/bias
    vec2 a = (position.xy / position.w) * 0.5 + 0.5;
    vec2 b = (prevPosition.xy / prevPosition.w) * 0.5 + 0.5;

    velocity = a - b;
}