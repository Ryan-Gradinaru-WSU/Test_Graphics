struct VSInput {
    float3 pos : TEXCOORD0;
    float2 uv  : TEXCOORD1;
};

struct VSOutput {
    float2 uv  : TEXCOORD0;
    float4 pos : SV_Position;
};

cbuffer TransformBuffer : register(b0, space1) {
    float4x4 transform;
};

VSOutput VSMain(VSInput input) {
    VSOutput output;
    output.uv  = input.uv;
    output.pos = mul(transform, float4(input.pos, 1.0));
    return output;
}






