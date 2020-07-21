#include "../structures.hlsl"

ps_in VS_Main(vs_in input)
{
    ps_in output;
    output.position = float4(0.0, 0.0, 0.0, 1.0);
    return output;
}