/////////////////////
// GLOBALS
/////////////////////
cbuffer MatrixBuffer
{
	matrix MVP;
};

struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Color	: COLOR;
};

struct VS_OUTPUT
{
	float4 Color	: COLOR;
	float4 Position	: SV_POSITION;	// Required semantic for the pixel shader
};

VS_OUTPUT main(VS_INPUT InputVertex)
{
	VS_OUTPUT OutputVertex;

	// Change position to float4 structure for matrix calculations	
	OutputVertex.Position = mul(float4(InputVertex.Position, 1.0f), MVP);

	OutputVertex.Color = float4(InputVertex.Color, 1.0f);
	return OutputVertex;
}