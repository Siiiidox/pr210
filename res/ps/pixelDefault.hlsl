#include "../structures.hlsl"
#include "../cameraStruct.hlsl"

float4 PS_Main(ps_in input) : SV_TARGET
{
    float3 eye = float3(0, 0, 3);
	float4 lightDir = normalize(float4( -0.5, 0, -1, 0));
    float diffuseLighting = saturate(dot(input.normal, -lightDir));
    float3 h = normalize(-lightDir + eye);
    float specLighting = pow(saturate(dot(h, input.normal)), 5.0f);

    float4 ambientColor = float4(0.529411, 0.807843, 0.921686, 1.0);
    float ambientIntensity = 0.01f;

    return input.color * diffuseLighting + ambientColor * ambientIntensity + specLighting;


}
