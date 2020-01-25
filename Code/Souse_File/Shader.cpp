#include "DxSystem.h"
#include "Shader.h"
#include <locale.h>
using namespace DirectX;
using namespace std;

unordered_map<WCHAR, Shader::set_of_vertex_shader_and_input_layout> Shader::vertex_cache;
unordered_map<WCHAR, ID3D11PixelShader*> Shader::pixel_cache;

Shader::~Shader()
{
}

//****************************************************************
//
//
//
//****************************************************************
//------------------------------------------------
//	シェーダー単体コンパイル
//------------------------------------------------
HRESULT Shader::Compile(WCHAR* filename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{
	DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ShaderFlags |= D3DCOMPILE_DEBUG;
	ShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

	ComPtr<ID3DBlob> BlobError = NULL;
	// コンパイル
	HRESULT hr = D3DCompileFromFile(
		filename,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		method,
		shaderModel,
		ShaderFlags,
		0,
		ppBlobOut,
		BlobError.GetAddressOf()
	);

	// エラー出力
	if (BlobError != NULL)
	{
		MessageBoxA(0, (char*)BlobError->GetBufferPointer(), NULL, MB_OK);
	}

	return hr;
}

//------------------------------------------------
//	シェーダーセットコンパイル
//------------------------------------------------
bool Shader::Create(WCHAR* filename, LPCSTR VSFunc, LPCSTR PSFunc)
{
	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> VSBlob = NULL;
	// 頂点シェーダ

	auto it = vertex_cache.find(*filename);
	if (it != vertex_cache.end())
	{
		VS = it->second.vertex_shader;
		VertexLayout = it->second.input_layout;
	}
	else
	{
		hr = Compile(filename, VSFunc, "vs_5_0", VSBlob.GetAddressOf());
		assert(SUCCEEDED(hr));


		// 頂点シェーダ生成
		hr = DxSystem::Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, VS.GetAddressOf());
		assert(SUCCEEDED(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		UINT numElements = ARRAYSIZE(layout);

		// 入力レイアウト生成
		hr = DxSystem::Device->CreateInputLayout(
			layout,
			numElements,
			VSBlob->GetBufferPointer(),
			VSBlob->GetBufferSize(),
			VertexLayout.GetAddressOf()
		);
		assert(SUCCEEDED(hr));

		vertex_cache.insert(make_pair(*filename, set_of_vertex_shader_and_input_layout(VS.Get(), VertexLayout.Get())));

		// 入力レイアウト設定
		DxSystem::DeviceContext->IASetInputLayout(VertexLayout.Get());
	}

	// ピクセルシェーダ
	auto itr = pixel_cache.find(*filename);
	if (itr != pixel_cache.end())
	{
		PS = itr->second;
		VertexLayout = it->second.input_layout;
	}
	else
	{
		ComPtr<ID3DBlob> PSBlob = NULL;
		hr = Compile(filename, PSFunc, "ps_5_0", &PSBlob);
		if (FAILED(hr))
		{
			return false;
		}
		pixel_cache.insert(make_pair(*filename, PS.Get()));
		// ピクセルシェーダ生成
		hr = DxSystem::Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, PS.GetAddressOf());
		//PSBlob->Release();
		assert(SUCCEEDED(hr));
	}


	return true;
}


//------------------------------------------------
//	シェーダーセットコンパイル2
//------------------------------------------------
bool Shader::Create(WCHAR* filename, LPCSTR VSFunc, LPCSTR PSFunc, LPCSTR GSFunc)
{
	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> VSBlob = NULL;
	// 頂点シェーダ
	hr = Compile(filename, VSFunc, "vs_5_0", VSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 頂点シェーダ生成
	hr = DxSystem::Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, VS.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 入力レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layout);

	// 入力レイアウト生成
	hr = DxSystem::Device->CreateInputLayout(
		layout,
		numElements,
		VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(),
		VertexLayout.GetAddressOf()
	);

	assert(SUCCEEDED(hr));

	// 入力レイアウト設定
	DxSystem::DeviceContext->IASetInputLayout(VertexLayout.Get());

	// ピクセルシェーダ
	ComPtr<ID3DBlob> PSBlob = NULL;
	hr = Compile(filename, PSFunc, "ps_5_0", PSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ピクセルシェーダ生成
	hr = DxSystem::Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, PS.GetAddressOf());
	//PSBlob.Get()->Release();
	assert(SUCCEEDED(hr));
	// ジオメトリシェーダ
	ComPtr<ID3DBlob> GSBlob = NULL;
	hr = Compile(filename, GSFunc, "gs_5_0", GSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ジオメトリシェーダ生成
	hr = DxSystem::Device->CreateGeometryShader(GSBlob->GetBufferPointer(), GSBlob->GetBufferSize(), NULL, GS.GetAddressOf());
	assert(SUCCEEDED(hr));

	return true;
}

//------------------------------------------------
//	シェーダーセットコンパイル3
//------------------------------------------------
bool Shader::Create(WCHAR* filename, LPCSTR VSFunc, LPCSTR PSFunc, LPCSTR GSFunc, LPCSTR HSFunc, LPCSTR DSFunc)
{
	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> VSBlob = NULL;
	// 頂点シェーダ
	hr = Compile(filename, VSFunc, "vs_5_0", VSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 頂点シェーダ生成
	hr = DxSystem::Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, VS.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 入力レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layout);

	// 入力レイアウト生成
	hr = DxSystem::Device->CreateInputLayout(
		layout,
		numElements,
		VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(),
		VertexLayout.GetAddressOf()
	);

	assert(SUCCEEDED(hr));
	VSBlob.Get()->Release();

	// 入力レイアウト設定
	DxSystem::DeviceContext->IASetInputLayout(VertexLayout.Get());

	// ピクセルシェーダ
	ComPtr<ID3DBlob> PSBlob = NULL;
	hr = Compile(filename, PSFunc, "ps_5_0", PSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ピクセルシェーダ生成
	hr = DxSystem::Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, PS.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ジオメトリシェーダ
	ComPtr<ID3DBlob> GSBlob = NULL;
	hr = Compile(filename, GSFunc, "gs_5_0", GSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));
	// ジオメトリシェーダ生成
	hr = DxSystem::Device->CreateGeometryShader(GSBlob->GetBufferPointer(), GSBlob->GetBufferSize(), NULL, GS.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ハルシェーダ
	ComPtr<ID3DBlob> HSBlob = NULL;
	hr = Compile(filename, HSFunc, "hs_5_0", HSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ハルシェーダ生成
	hr = DxSystem::Device->CreateHullShader(HSBlob->GetBufferPointer(), HSBlob->GetBufferSize(), NULL, HS.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ドメインシェーダ
	ComPtr<ID3DBlob> DSBlob = NULL;
	hr = Compile(filename, DSFunc, "ds_5_0", DSBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	// ドメインシェーダ生成
	hr = DxSystem::Device->CreateDomainShader(DSBlob->GetBufferPointer(), DSBlob->GetBufferSize(), NULL, DS.GetAddressOf());
	assert(SUCCEEDED(hr));



	return true;
}

//****************************************************************
//
//
//
//****************************************************************
//------------------------------------------------
//	有効化
//------------------------------------------------
void Shader::Activate()
{
	DxSystem::DeviceContext->VSSetShader(VS.Get(), NULL, 0);
	DxSystem::DeviceContext->PSSetShader(PS.Get(), NULL, 0);
	DxSystem::DeviceContext->GSSetShader(GS.Get(), NULL, 0);
	DxSystem::DeviceContext->HSSetShader(HS.Get(), NULL, 0);
	DxSystem::DeviceContext->DSSetShader(DS.Get(), NULL, 0);

}
