#include "../structures.hlsl"
#include "../vertexStructs.hlsl"


ps_in VS_Main(vs_in input)
{
    float4x4 mvp = mul(transform, viewProj);
    ps_in output;
    output.position = mul(input.position, mvp);
    output.color = input.color;
    output.normal = normalize(mul(input.normal, transform));
    output.texCoord = input.texCoord;
    return output;
}