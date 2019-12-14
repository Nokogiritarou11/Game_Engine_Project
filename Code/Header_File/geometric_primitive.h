#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
#include <wrl.h>

class geometric_primitive
{
private:

	Microsoft::WRL::ComPtr <ID3D11VertexShader> g_pVertexShader;
	Microsoft::WRL::ComPtr <ID3D11PixelShader> g_pPixelShader;
	Microsoft::WRL::ComPtr <ID3D11InputLayout> g_pVertexLayout;
	Microsoft::WRL::ComPtr <ID3D11Buffer> g_pVertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer> g_pIndexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer> g_pConstantBuffer;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState> g_pRasterizerState;

public:

	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
	};

	struct cbuffer
	{
		XMFLOAT4X4 world_view_projection; //���[���h�E�r���[�E�v���W�F�N�V���������s��
		XMFLOAT4X4 world; //���[���h�ϊ��s��
		XMFLOAT4 material_color; //�ގ��F
		XMFLOAT4 light_direction; //���C�g�i�s���� 
	};

	geometric_primitive(ID3D11Device* device);
	//~geometric_primitive();

	void render(
		ID3D11DeviceContext* g_pImmediateContext, //�f�o�C�X�R���e�L�X�g
		const XMFLOAT4& color, //�ގ��F
		const XMFLOAT4& light_direction, //���C�g�i�s����
		const XMFLOAT4X4& world_view_projection, //���[���h�E�r���[�E�v���W�F�N�V���������s��
		const XMFLOAT4X4& world) const; //���[���h�ϊ��s��
};