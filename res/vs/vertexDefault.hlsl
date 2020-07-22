#include "../structures.hlsl"
#include "../vertexStructs.hlsl"


ps_in VS_Main(vs_in input)
{
    matrix mvp = mul(transform, viewProj);
    ps_in output;
    output.position = mul(input.position, mvp);
    output.color = input.color;
    output.texCoord = input.texCoord;
    return output;
}