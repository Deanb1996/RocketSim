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
Texture2D txBump : register(t1);
Texture2D txHeight : register(t2);
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
	float3 Norm : TEXCOORD1;
	float3 PosWorld : TEXCOORD2;
	float3x3 TBN : TEXCOORD5;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	input.Pos.w = 1;

	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(input.Norm, World).xyz;
	output.Norm = normalize(output.Norm);
	output.PosWorld = mul(input.Pos, World).xyz;
	output.Tex = input.Tex;
	float3 tangent = mul(input.Tan, World).xyz; 
	tangent = normalize(tangent);
	float3 biTangent = mul(input.BiTan, World).xyz;
	biTangent = normalize(biTangent);

	output.TBN = float3x3(
		tangent,
		biTangent,
		output.Norm
		);

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColour = 0;
	float diffuse = 0;
	float4 diffuseColour = 0;
	float specular = 0;
	float4 specularColour = 0;
	float4 materialDiff = float4(0.55, 0.55, 0.55, 1.0);
	float4 materialSpec = float4(0.70, 0.70, 0.70, 1.0);
	float4 texColour = txColour.Sample(txSampler, input.Tex);
	float4 ambient = float4(0.1, 0.1, 0.1, 1.0);


	float mappingScale = 0.03f;
	float bias = 0.01f;
	float height = txHeight.Sample(txSampler, input.Tex).x;
	height = mappingScale * height - bias;

	float3 viewDir = normalize(vCameraPos - input.PosWorld);
	float3 viewDirT = normalize(mul(input.TBN, viewDir));
	float2 texCorrect = input.Tex + (height * viewDirT.xy);

	float4 normal = txBump.Sample(txSampler, texCorrect);
	normal = normalize((normal * 2.0) - 1.0);
	//float3 bumpNormal = (bumpMap.x * input.Tan) + (bumpMap.y * input.BiTan) + (bumpMap.z * input.Norm);
	//bumpNormal = normalize(bumpNormal);

	for (int i = 0; i < numOfLights; i++)
	{
		float3 lightDir = normalize(vLightPos[i] - input.PosWorld);
		float3 lightDirT = normalize(mul(input.TBN, lightDir));
		float3 reflection = normalize(reflect(-lightDirT, normal));

		diffuse = saturate(dot(lightDirT, normal));
		specular = pow(saturate(dot(viewDirT, reflection)), 0.9 * 128);
		if (diffuse <= 0)
		{
			specular = 0;
		}
		diffuseColour = vLightColour[i] * diffuse;

		specularColour = vLightColour[i] * specular;

		finalColour += saturate(ambient + (diffuseColour * materialDiff) + (specularColour * materialSpec));
	}
	finalColour.a = 1;
	return finalColour * texColour;
}
