struct DirectionalLight
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer HemiConstants 
{
	DirectionalLight light;
}

struct PS_INPUT
{
	float4 Position	: SV_POSITION;
	float3 Norm		: TEXCOORD0;
	float3 Color	: COLOR0;
};

float4 main(PS_INPUT Input) : SV_TARGET
{
	Input.Norm = normalize(Input.Norm);

	float3 finalCol;
	finalCol = light.ambient;
	finalCol += saturate(dot(light.dir, Input.Norm) * light.diffuse * Input.Color);

	return float4(finalCol, 1.0f);
}