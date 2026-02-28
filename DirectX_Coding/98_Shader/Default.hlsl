// 버텍스 쉐이더와 픽셀 쉐이더를 한 파일안에 넣어두는게 더 합리적일 때도 있다.

struct VS_INPUT	// 기하학적인 물체의 고유 형태를 얘기하기에, 절대 수정하면 안됨
{
	float4 position : POSITION;
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

// vertex shader
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	// float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    float4 offset;	
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

// IA - VS - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input)	// 버텍스 쉐이더 단계에서 SRT를 연산하여 모델을 변환함
{
	VS_OUTPUT output;
	output.position = input.position + offset;
	/*output.color = input.color;*/
	output.uv = input.uv;

	return output;
}

float4 PS(VS_OUTPUT output) : SV_Target
{
	float4 color = texture0.Sample(sampler0, output.uv);		// 어떠한 샘플의 색상인지를 결정함 (Sample)
	// return output.color;
	return color;
}