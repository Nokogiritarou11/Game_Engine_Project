#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
#include <wrl.h>
#include <vector>
#include <stdio.h>
using namespace std;

class static_mesh
{
private:

	Microsoft::WRL::ComPtr <ID3D11VertexShader> g_pVertexShader;
	Microsoft::WRL::ComPtr <ID3D11PixelShader> g_pPixelShader;
	Microsoft::WRL::ComPtr <ID3D11InputLayout> g_pVertexLayout;
	Microsoft::WRL::ComPtr <ID3D11Buffer> g_pVertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer> g_pIndexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer> g_pConstantBuffer;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState> g_pRasterizerState;

	u_int current_index = 0;

public:

	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
	};

	struct cbuffer
	{
		XMFLOAT4X4 world_view_projection; //ワールド・ビュー・プロジェクション合成行列
		XMFLOAT4X4 world; //ワールド変換行列
		XMFLOAT4 material_color; //材質色
		XMFLOAT4 light_direction; //ライト進行方向 
	};

	static_mesh(ID3D11Device* device);
	//~static_mesh();

	void render(
		ID3D11DeviceContext* g_pImmediateContext, //デバイスコンテキスト
		const XMFLOAT4& color, //材質色
		const XMFLOAT4& light_direction, //ライト進行方向
		const XMFLOAT4X4& world_view_projection, //ワールド・ビュー・プロジェクション合成行列
		const XMFLOAT4X4& world) const; //ワールド変換行列
};