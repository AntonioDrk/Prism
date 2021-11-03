struct PS_OUTPUT
{
	float4 RGBColor : COLOR0;
};

struct PS_INPUT
{
	float4 Position	: SV_Position;
	float4 Color	: COLOR;
};

PS_OUTPUT main(PS_INPUT Input) : SV_TARGET0
{
	PS_OUTPUT Output;
	Output.RGBColor = Input.Color;
	return Output;
}