// 버텍스 쉐이더와 픽셀 쉐이더를 한 파일안에 넣어두는게 더 합리적일 때도 있다.

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