//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix WorldInverse;
	matrix View;
	matrix Projection;
	float4 vLightPos[4];
	float4 vLightColour[4];
	float4 vCameraPos;
	float numOfLights;
	float time;
	float2 padding;
}

TextureCube txColour : register(t0);
SamplerState txSampler : register(s0);


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float3 Tan : TANGENT;
	float3 BiTan : BITANGENT;
	float3 InstancePos : INSTANCEPOSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Tex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection).xyww;
	output.Tex = input.Pos;

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	return txColour.Sample(txSampler, input.Tex);
}