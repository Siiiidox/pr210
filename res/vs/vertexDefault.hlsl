#include "../structures.hlsl"

ps_in VS_Main(vs_in input)
{
	ps_in output;

	output.position = input.position;
	output.color = input.color;
	output.texCoord = input.texCoord;

	return output;
}