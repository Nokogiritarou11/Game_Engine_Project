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
	ComPtr<ID3D11VertexShader>		VS = nullptr; // 頂点シェーダ
	ComPtr<ID3D11PixelShader>		PS = nullptr; // ピクセルシェーダ
	ComPtr<ID3D11GeometryShader>	GS = nullptr; // ジオメトリシェーダ
	ComPtr<ID3D11HullShader>		HS = nullptr; //　ハルシェーダ
	ComPtr<ID3D11DomainShader>		DS = nullptr;//　ドメインネームシェーダ

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
//	頂点データ構造体定義
//
//----------------------------------------------

struct VERTEX
{
	DirectX::XMFLOAT3 Pos;	//位置
	DirectX::XMFLOAT3 Normal;//法線
	DirectX::XMFLOAT2 Tex;	//UV座標
	DirectX::XMFLOAT4 Color;	//頂点色
};