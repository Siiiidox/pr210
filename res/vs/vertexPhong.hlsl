#include "../structures.hlsl"

float4 lightDir = float4(0.0, 0.0, 1.0, 1.0);
float4 lightColor = float4(1.0, 1.0, 1.0, 1.0);
float4 ambientLight = float4(0.529411, 0.807843, 0.921686, 1.0);

float ambientLightStrength = 0.4;
float diffuseStrength = 1;
float specularStrength = 0.4;
ps_in VS_Main(vs_in input)
{
    ps_in output;
    output.position = input.position;
    output.texCoord = input.texCoord;

    float4 Ia = ambientLightStrength * ambientLight;
    float4 Id = diffuseStrength * saturate(dot(input.normal, -lightDir));
    float4 Is = specularStrength * pow(saturate(dot(input.normal, normalize(-lightDir + input.position))), 0.5);
    output.color = Ia + (Id + Is) * lightColor;
    return output;
}