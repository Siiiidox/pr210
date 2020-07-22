#include "../structures.hlsl"

float4 PS_Main(ps_in input) : SV_TARGET
{
    float4 lightDir = float4(1, -1, -1, 0);
    float4 ambientColor = float4(0.529411, 0.807843, 0.921686, 1.0);
    float ambientIntensity = 0.1f;
    float intensity = saturate(dot(normalize(-lightDir), input.normal));
    return input.color * intensity + ambientColor * ambientIntensity;
}