#version 450

layout(binding = 0) uniform sampler2D tex;
layout(location = 0) out vec4 outColor;

void main() {
    // No textureSize, no branches, just a single sample in the middle
    vec4 c = texture(tex, vec2(0.5, 0.5));
    outColor = c;
}
