/////////////////////
// GLOBALS
/////////////////////
cbuffer MatrixBuffer
{
	matrix MVP;
	matrix World;
};

struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Norm		: NORMAL;
	float3 Color	: COLOR;
};

struct VS_OUTPUT
{
	float4 Position	: SV_POSITION;	// Required semantic for the pixel shader
	float3 Norm		: TEXCOORD0;
	float3 Color	: COLOR0;
};

VS_OUTPUT main(VS_INPUT InputVertex)
{
	VS_OUTPUT OutputVertex;

	// Change position to float4 structure for matrix calculations	
	OutputVertex.Position = mul(float4(InputVertex.Position, 1.0f), MVP);

	OutputVertex.Norm = mul(InputVertex.Norm, (float3x3)World);

	OutputVertex.Color = InputVertex.Color;
	return OutputVertex;
}