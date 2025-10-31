// ���ؽ� ���̴��� �ȼ� ���̴��� �� ���Ͼȿ� �־�δ°� �� �ո����� ���� �ִ�.

struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

// vertex shader
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// IA - VS - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = input.position;
	output.color = input.color;

	return output;
}

float4 PS(VS_OUTPUT output) : SV_Target
{
	float4 color = float4(1,0,0,0);
	return color;
}