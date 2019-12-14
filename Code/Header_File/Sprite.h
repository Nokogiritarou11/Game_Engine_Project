#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
#include <wrl.h>
#include <d3dcompiler.h>
#include "Shader.h"
#include "Texture.h"

class Sprite
{
private:
	ComPtr<ID3D11Buffer> VertexBuffer;
	Texture* texture;

	ComPtr<ID3D11DepthStencilState> DepthStencilState;

public:
	Sprite();
	Sprite(const wchar_t* filename);
	~Sprite();
	void Render(Shader* shader,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float alpha = 1.0f
	);
	void Render(Shader* shader,
		Texture* tex,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float alpha = 1.0f
	);

	ID3D11RenderTargetView* GetRenderTarget() { return texture->GetRenderTarget(); }
	void SetTexture(u_int slot) { texture->Set(slot); }

};

/*

class sprite_batch
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> g_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> g_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> g_pVertexLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> g_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> g_pRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_pShaderResourceView;
	D3D11_TEXTURE2D_DESC g_TEXTURE2D_DESC;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> g_pSamplerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> g_pDepthStencilState;

	const size_t MAX_INSTANCES = 256;
	struct instance
	{
		XMFLOAT4X4 ndc_transform;
		XMFLOAT4 texcoord_transform;
		XMFLOAT4 color;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_buffer;
public:
	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoord;
	};

	sprite_batch(ID3D11Device* device, const wchar_t* TexName, size_t max_instance = 256);

	void begin(ID3D11DeviceContext* g_pImmediateContext);
	void render(ID3D11DeviceContext* g_pImmediateContext,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle/*degree,
		float r, float g, float b, float a);
	void end(ID3D11DeviceContext* g_pImmediateContext);

private:
	D3D11_VIEWPORT viewport;

	UINT count_instance = 0;
	instance* instances = nullptr;
};
*/