Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    return tex.Sample(samp, float2(0.5f, 0.5f));
}
