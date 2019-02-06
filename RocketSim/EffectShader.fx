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
    float3 Norm : TEXCOORD1;
	float3 PosWorld : TEXCOORD2;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	input.Pos.w = 1;

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Norm = mul( WorldInverse, float4( input.Norm, 1) ).xyz;
	output.Norm = normalize(output.Norm);
	output.PosWorld = mul( input.Pos, World ).xyz;
	output.Tex = input.Tex;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 finalColour = 0;
	float diffuse = 0;
	float4 diffuseColour = 0;
	float specular = 0;
	float4 specularColour = 0;
	float3 viewDir = normalize(vCameraPos - input.PosWorld);
	float4 materialDiff = float4(0.55, 0.55, 0.55, 1.0);
	float4 materialSpec = float4(0.70, 0.70, 0.70, 1.0);
	float4 texColour = txColour.Sample(txSampler, input.Tex);
	float4 ambient = float4(0.1, 0.1, 0.2, 1.0);


    for(int i = 0; i < numOfLights; i++)
    {
		float3 lightDir = normalize(vLightPos[i] - input.PosWorld);
		float3 reflection = normalize(reflect(-lightDir, input.Norm));

		diffuse = saturate(dot(lightDir, input.Norm));
		specular = pow(saturate(dot(viewDir, reflection)), 0.8 * 128);
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
