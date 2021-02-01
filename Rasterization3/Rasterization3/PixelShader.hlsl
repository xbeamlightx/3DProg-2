//Texture2D meshTexture : register(t0);
//SamplerState mySampler : register(s0);
//
//struct PixelShaderInput
//{
//	float4 position : SV_POSITION;
//	float2 uv : UV;
//	float3 color : COLOR;
//};
//
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//	float3 textureColor = meshTexture.Sample(mySampler, input.uv);
//	return float4(textureColor, 1.0f);
//}




struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 color : COLOR;
};

SamplerState mySampler : register(s0);
Texture2D meshTexture : register(t0);

VS_OUTPUT main(PixelShaderInput input) : SV_TARGET
{
	float3 textureColor = meshTexture.Sample(mySampler, input.uv);
	return float4(textureColor, 1.0f);
}