/////////////////////
// GLOBALS
/////////////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
	OutputVertex.Position = float4(InputVertex.Position, 1.0f);	
	/*OutputVertex.Position = mul(InputVertex.Position, worldMatrix);
	OutputVertex.Position = mul(OutputVertex.Position, viewMatrix);
	OutputVertex.Position = mul(OutputVertex.Position, projectionMatrix);*/

	OutputVertex.Color = float4(InputVertex.Color, 1.0f);
	return OutputVertex;
}