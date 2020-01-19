#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

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
	XMFLOAT3 Pos;	//�ʒu
	XMFLOAT3 Normal;//�@��
	XMFLOAT2 Tex;	//UV���W
	XMFLOAT4 Color;	//���_�F
};