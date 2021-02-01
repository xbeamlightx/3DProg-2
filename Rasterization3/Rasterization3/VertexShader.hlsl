struct VertexShaderInput
{
	float3 position : POSITION;
	float2 uv : UV;
	//float3 color : COLOR;

};
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	//float3 color : COLOR;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;
	//output.color = input.color;
	return output;
}
