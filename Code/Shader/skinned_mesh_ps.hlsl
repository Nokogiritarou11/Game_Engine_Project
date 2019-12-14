Texture2D tex2d : register(t0);
SamplerState smpState : register(s0);
#include "skinned_mesh.hlsli"

float4 main(VS_OUT pin) : SV_TARGET
{
	return tex2d.Sample(smpState, pin.texcoord) * pin.color;
}