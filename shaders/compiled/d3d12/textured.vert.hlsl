struct VSInput {
    float2 in_pos : POSITION;
    float2 in_uv  : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = float4(input.in_pos, 0.0f, 1.0f);
    output.uv = input.in_uv;
    return output;
}
