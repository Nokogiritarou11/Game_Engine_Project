
Texture2D tex2d : register(t0);
SamplerState smpState : register(s0);

#define MAX_BONES 32
cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	//float4 bone_weights : WEIGHTS;
	//uint4 bone_indices : BONES;
};

VS_OUT VSMain(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD, float4 bone_weights : WEIGHTS, uint4 bone_indices : BONES)
{
	VS_OUT vout;

	vout.position = mul(position, world_view_projection);
	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(light_direction);

	//vout.color = material_color * max(0, dot(L, N)) * 0.5f + 0.5f;
	vout.color = material_color * max(0.5f, dot(L, N));
	vout.color.a = material_color.a;
	vout.texcoord = texcoord;
	return vout;
}

float4 PSMain(VS_OUT pin) : SV_TARGET
{
	return tex2d.Sample(smpState, pin.texcoord) * pin.color;
}