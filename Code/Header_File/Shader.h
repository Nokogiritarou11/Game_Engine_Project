#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <unordered_map>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class Shader
{
protected:
	ComPtr<ID3D11VertexShader>		VS = nullptr; // ���_�V�F�[�_
	ComPtr<ID3D11PixelShader>		PS = nullptr; // �s�N�Z���V�F�[�_
	ComPtr<ID3D11GeometryShader>	GS = nullptr; // �W�I���g���V�F�[�_
	ComPtr<ID3D11HullShader>		HS = nullptr; //�@�n���V�F�[�_
	ComPtr<ID3D11DomainShader>		DS = nullptr;//�@�h���C���l�[���V�F�[�_

	ComPtr<ID3D11InputLayout>		VertexLayout;

	HRESULT Compile(WCHAR* filename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** ppBlobOut);

	struct set_of_vertex_shader_and_input_layout
	{
		set_of_vertex_shader_and_input_layout(ID3D11VertexShader* vertex_shader, ID3D11InputLayout* input_layout) : vertex_shader(vertex_shader), input_layout(input_layout) {}
		ID3D11VertexShader* vertex_shader;
		ID3D11InputLayout* input_layout;
	};
	static std::unordered_map<WCHAR, set_of_vertex_shader_and_input_layout> vertex_cache;
	static std::unordered_map<WCHAR, ID3D11PixelShader*> pixel_cache;

public:
	Shader() { /*ZeroMemory(this, sizeof(Shader));*/ }
	virtual ~Shader();

	bool Create(WCHAR* filename, LPCSTR VSName, LPCSTR PSName);
	bool Create(WCHAR* filename, LPCSTR VSName, LPCSTR PSName, LPCSTR GSName);
	bool Create(WCHAR* filename, LPCSTR VSName, LPCSTR PSName, LPCSTR GSName, LPCSTR HSName, LPCSTR DSName);

	void Activate();
};


//----------------------------------------------
//
//	���_�f�[�^�\���̒�`
//
//----------------------------------------------

struct VERTEX
{
	DirectX::XMFLOAT3 Pos;	//�ʒu
	DirectX::XMFLOAT3 Normal;//�@��
	DirectX::XMFLOAT2 Tex;	//UV���W
	DirectX::XMFLOAT4 Color;	//���_�F
};