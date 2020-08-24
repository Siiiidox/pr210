#include "../structures.hlsl"
#include "../cameraStruct.hlsl"

texture2D textureObj;
sampler samplerObj;


float median(float a, float b, float c)
{
    return max(min(a, b), min(max(a, b), c));
}
float4 PS_Main(ps_in input) : SV_TARGET
{
    float4 outsideColor =  float4(0.f, 0.1f, 0.2f, 1.f);
    float4 insideColor = float4(.76f, .237f, .74f, 1.f);
    float3 s = textureObj.Sample(samplerObj, input.texCoord).rgb;
    float d = median(s.r, s.g, s.b) - 0.5;
    float w = clamp(d / fwidth(d) + 0.5, 0.0, 1.0);

    return lerp(outsideColor, insideColor, w);
}
