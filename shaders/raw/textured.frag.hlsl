#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;
layout(set = 2, binding = 0) uniform sampler2D tex;

void main() {
    vec4 s = texture(tex, fragUV);
    outColor = vec4(s.rgba); // force alpha to 1

    //outColor = vec4(0, 1, 1, 1);
}
