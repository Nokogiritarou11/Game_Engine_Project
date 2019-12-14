
//--------------------------------------------
//	テクスチャ
//--------------------------------------------

Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//--------------------------------------------
//	グローバル変数
//--------------------------------------------

//--------------------------------------------
//	データーフォーマット
//--------------------------------------------

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------

PSInput VSMain(VSInput input)
{
	PSInput output = (PSInput)0;

	// 出力値設定.
	output.Position = float4(input.Position, 1);
	output.Color = input.Color;
	//output.Color = float4(1,1,1,1);
	output.Tex = input.Tex;

	return output;
}

//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------

float4 PSMain(PSInput input) : SV_TARGET0
{
	float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex) * input.Color;

	return color;
}
