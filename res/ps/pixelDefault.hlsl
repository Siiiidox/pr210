#include "../structures.hlsl"

float4 PS_Main(ps_in input) : SV_TARGET
{
	return input.color;
}