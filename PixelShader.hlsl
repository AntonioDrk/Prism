struct PS_INPUT
{
	float4 Color	: COLOR;
};

float4 main(PS_INPUT Input) : SV_TARGET
{
	return Input.Color;
}