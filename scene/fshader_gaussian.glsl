#version 150

uniform sampler2D tex;
in  vec2 fTexCoord;
out vec4 color;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;

void main() {
    vec4 sum = vec4(0.0);
    for (int x = -4; x <= 4; x++)
        for (int y = -4; y <= 4; y++)
            sum += texture(tex, vec2(fTexCoord.x + x * blurSizeH, fTexCoord.y + y * blurSizeV)) / 81.0;
    color = sum;
}