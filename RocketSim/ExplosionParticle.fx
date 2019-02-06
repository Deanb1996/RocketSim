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

Texture2D txColour : register(t0);
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
	float2 Tex : TEXCOORD0;
	float FadeRate : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Tex = input.Tex;

	matrix viewInv = transpose(View);
	float3 vX = viewInv[0].xyz;
	float3 vY = viewInv[1].xyz;
	output.Pos = float4((input.Pos.x * vX + input.Pos.y * vY), 1);

	output.Pos = mul(output.Pos, World);

	float lifeCycle = 8;
	float freq = 5;
	float T = fmod(freq * time + input.InstancePos.z/10, lifeCycle);
	output.FadeRate = (1.0 - T / lifeCycle);

	float height = 0.2;
	float spread = 0.4;
	output.Pos.xyz += float3(spread * cos(300 * input.InstancePos.z), height, spread * sin(300 * input.InstancePos.z)) * T;

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 texColour = txColour.Sample(txSampler, input.Tex);
	float radius = 0.4;
	float range = radius * radius - dot(input.Tex - 0.5, input.Tex - 0.5);
	float shade = 2 / (1 + exp(12 * range));
	return texColour * input.FadeRate * (1 - shade);
}