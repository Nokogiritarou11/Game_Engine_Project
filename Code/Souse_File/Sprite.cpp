#include "Sprite.h"
#include "DxSystem.h"

Sprite::Sprite() :texture(nullptr)
{

	VERTEX v[] = {
		XMFLOAT3(-0.5f, 0.5f,0), XMFLOAT3(0,0,1), XMFLOAT2(0,0), XMFLOAT4(1,1,1,1), //左上
		XMFLOAT3(0.5f, 0.5f,0),  XMFLOAT3(0,0,1), XMFLOAT2(1,0), XMFLOAT4(1,1,1,1), //右上
		XMFLOAT3(-0.5f,-0.5f,0), XMFLOAT3(0,0,1), XMFLOAT2(0,1), XMFLOAT4(1,1,1,1), //左下
		XMFLOAT3(0.5f,-0.5f,0),  XMFLOAT3(0,0,1), XMFLOAT2(1,1), XMFLOAT4(1,1,1,1), //右下
	};

	//	頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(v);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA res;
	ZeroMemory(&res, sizeof(res));
	res.pSysMem = v;

	DxSystem::Device->CreateBuffer(&bd, &res, VertexBuffer.GetAddressOf());


	//デプスステンシルステート
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = FALSE;
	DxSystem::Device->CreateDepthStencilState(&depth_stencil_desc, DepthStencilState.GetAddressOf());

}


Sprite::Sprite(const wchar_t* filename) :texture(nullptr)
{

	VERTEX v[] = {
		XMFLOAT3(-0.5f, 0.5f,0), XMFLOAT3(0,0,1), XMFLOAT2(0,0), XMFLOAT4(1,1,1,1), //左上
		XMFLOAT3(0.5f, 0.5f,0),  XMFLOAT3(0,0,1), XMFLOAT2(1,0), XMFLOAT4(1,1,1,1), //右上
		XMFLOAT3(-0.5f,-0.5f,0), XMFLOAT3(0,0,1), XMFLOAT2(0,1), XMFLOAT4(1,1,1,1), //左下
		XMFLOAT3(0.5f,-0.5f,0),  XMFLOAT3(0,0,1), XMFLOAT2(1,1), XMFLOAT4(1,1,1,1), //右下
	};

	//	頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd)); // 全０
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(v);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA res;
	ZeroMemory(&res, sizeof(res));
	res.pSysMem = v;

	DxSystem::Device->CreateBuffer(&bd, &res, VertexBuffer.GetAddressOf());

	//	テクスチャ読み込み
	if (filename) {
		texture = new Texture();
		texture->Load(filename);
	}


	//デプスステンシルステート
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = FALSE;
	DxSystem::Device->CreateDepthStencilState(&depth_stencil_desc, DepthStencilState.GetAddressOf());

}


Sprite::~Sprite()
{
	SAFE_DELETE(texture);

}


void Sprite::Render(Shader* shader,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float alpha
)
{
	shader->Activate();
	//頂点データ設定
	VERTEX data[4];

	data[0].Pos.x = dx;
	data[0].Pos.y = dy;
	data[0].Pos.z = 0.0f;

	data[1].Pos.x = dx + dw;
	data[1].Pos.y = dy;
	data[1].Pos.z = 0.0f;

	data[2].Pos.x = dx;
	data[2].Pos.y = dy + dh;
	data[2].Pos.z = 0.0f;

	data[3].Pos.x = dx + dw;
	data[3].Pos.y = dy + dh;
	data[3].Pos.z = 0.0f;


	// 正規化デバイス座標系
	for (int i = 0; i < 4; i++) {
		data[i].Pos.x = 2.0f * data[i].Pos.x / DxSystem::GetScreenWidth() - 1.0f;
		data[i].Pos.y = 1.0f - 2.0f * data[i].Pos.y / DxSystem::GetScreenHeight();
		data[i].Pos.z = 0.0f;
	}


	//テクスチャ座標設定
	data[0].Tex.x = sx;
	data[0].Tex.y = sy;
	data[1].Tex.x = sx + sw;
	data[1].Tex.y = sy;
	data[2].Tex.x = sx;
	data[2].Tex.y = sy + sh;
	data[3].Tex.x = sx + sw;
	data[3].Tex.y = sy + sh;

	//UV座標
	for (int i = 0; i < 4; i++) {
		data[i].Tex.x = data[i].Tex.x / texture->GetWidth();
		data[i].Tex.y = data[i].Tex.y / texture->GetHeight();
	}
	//頂点カラー
	data[0].Color = XMFLOAT4(1, 1, 1, alpha);
	data[1].Color = XMFLOAT4(1, 1, 1, alpha);
	data[2].Color = XMFLOAT4(1, 1, 1, alpha);
	data[3].Color = XMFLOAT4(1, 1, 1, alpha);
	//法線
	data[0].Normal = XMFLOAT3(0, 0, 1);
	data[1].Normal = XMFLOAT3(0, 0, 1);
	data[2].Normal = XMFLOAT3(0, 0, 1);
	data[3].Normal = XMFLOAT3(0, 0, 1);

	//頂点データ更新
	DxSystem::DeviceContext->UpdateSubresource(VertexBuffer.Get(), 0, NULL, data, 0, 0);

	//	頂点バッファの指定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DxSystem::DeviceContext->IASetVertexBuffers(
		0, 1, VertexBuffer.GetAddressOf(), // スロット, 数, バッファ
		&stride,		// １頂点のサイズ
		&offset			// 開始位置
	);
	DxSystem::DeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
	//テクスチャの設定
	if (texture) texture->Set();

	DxSystem::DeviceContext->Draw(4, 0);
}

void Sprite::Render(Shader* shader,
	Texture* tex,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float alpha
)
{
	shader->Activate();
	//頂点データ設定
	VERTEX data[4];

	data[0].Pos.x = dx;
	data[0].Pos.y = dy;
	data[0].Pos.z = 0.0f;

	data[1].Pos.x = dx + dw;
	data[1].Pos.y = dy;
	data[1].Pos.z = 0.0f;

	data[2].Pos.x = dx;
	data[2].Pos.y = dy + dh;
	data[2].Pos.z = 0.0f;

	data[3].Pos.x = dx + dw;
	data[3].Pos.y = dy + dh;
	data[3].Pos.z = 0.0f;


	// 正規化デバイス座標系
	for (int i = 0; i < 4; i++) {
		data[i].Pos.x = 2.0f * data[i].Pos.x / DxSystem::GetScreenWidth() - 1.0f;
		data[i].Pos.y = 1.0f - 2.0f * data[i].Pos.y / DxSystem::GetScreenHeight();
		data[i].Pos.z = 0.0f;
	}


	//テクスチャ座標設定
	data[0].Tex.x = sx;
	data[0].Tex.y = sy;
	data[1].Tex.x = sx + sw;
	data[1].Tex.y = sy;
	data[2].Tex.x = sx;
	data[2].Tex.y = sy + sh;
	data[3].Tex.x = sx + sw;
	data[3].Tex.y = sy + sh;

	//UV座標
	for (int i = 0; i < 4; i++) {
		data[i].Tex.x = data[i].Tex.x / tex->GetWidth();
		data[i].Tex.y = data[i].Tex.y / tex->GetHeight();
	}
	//頂点カラー
	data[0].Color = XMFLOAT4(1, 1, 1, alpha);
	data[1].Color = XMFLOAT4(1, 1, 1, alpha);
	data[2].Color = XMFLOAT4(1, 1, 1, alpha);
	data[3].Color = XMFLOAT4(1, 1, 1, alpha);
	//法線
	data[0].Normal = XMFLOAT3(0, 0, 1);
	data[1].Normal = XMFLOAT3(0, 0, 1);
	data[2].Normal = XMFLOAT3(0, 0, 1);
	data[3].Normal = XMFLOAT3(0, 0, 1);

	//頂点データ更新
	DxSystem::DeviceContext->UpdateSubresource(VertexBuffer.Get(), 0, NULL, data, 0, 0);

	//	頂点バッファの指定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DxSystem::DeviceContext->IASetVertexBuffers(
		0, 1, VertexBuffer.GetAddressOf(), // スロット, 数, バッファ
		&stride,		// １頂点のサイズ
		&offset			// 開始位置
	);
	DxSystem::DeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
	DxSystem::DeviceContext->OMSetDepthStencilState(DepthStencilState.Get(), 1);

	//テクスチャの設定
	if (tex) tex->Set();

	DxSystem::DeviceContext->Draw(4, 0);
}

/*
//スプライトバッチ
sprite_batch::sprite_batch(ID3D11Device* device, const wchar_t* TexName, size_t max_instance) : MAX_INSTANCES(max_instance)
{
	HRESULT hr = S_OK;

	vertex vertices[] =
	{
		{ XMFLOAT3(0, 0, 0), XMFLOAT2(0, 0) },
		{ XMFLOAT3(1, 0, 0), XMFLOAT2(1, 0) },
		{ XMFLOAT3(0, 1, 0), XMFLOAT2(0, 1) },
		{ XMFLOAT3(1, 1, 0), XMFLOAT2(1, 1) },
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;				// 頂点のアドレス
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&bd, &InitData, g_pVertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NDC_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NDC_TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NDC_TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "NDC_TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	create_vs_from_cso(device, "sprite_batch_vs.cso", g_pVertexShader.GetAddressOf(), g_pVertexLayout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	create_ps_from_cso(device, "sprite_batch_ps.cso", g_pPixelShader.GetAddressOf());

	instance* instances = new instance[MAX_INSTANCES];
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(instance) * MAX_INSTANCES;
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = instances;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, instance_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	delete[] instances;

	D3D11_RASTERIZER_DESC rasterizer_desc = {};
	rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
	rasterizer_desc.CullMode = D3D11_CULL_NONE; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = FALSE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = device->CreateRasterizerState(&rasterizer_desc, g_pRasterizerState.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	load_texture_from_file(device, TexName, g_pShaderResourceView.GetAddressOf(), &g_TEXTURE2D_DESC);

	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	memcpy(sampler_desc.BorderColor, &XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMFLOAT4));
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampler_desc, g_pSamplerState.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	depth_stencil_desc.DepthEnable = FALSE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.StencilEnable = FALSE;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = device->CreateDepthStencilState(&depth_stencil_desc, g_pDepthStencilState.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void sprite_batch::begin(ID3D11DeviceContext* g_pImmediateContext)
{
	HRESULT hr = S_OK;

	UINT strides[2] = { sizeof(vertex), sizeof(instance) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer* vbs[2] = { g_pVertexBuffer.Get(), instance_buffer.Get() };
	g_pImmediateContext->IASetVertexBuffers(0, 2, vbs, strides, offsets);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout.Get());
	g_pImmediateContext->OMSetDepthStencilState(g_pDepthStencilState.Get(), 1);
	g_pImmediateContext->RSSetState(g_pRasterizerState.Get());
	g_pImmediateContext->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShaderResources(0, 1, g_pShaderResourceView.GetAddressOf());
	g_pImmediateContext->PSSetSamplers(0, 1, g_pSamplerState.GetAddressOf());

	UINT num_viewports = 1;
	g_pImmediateContext->RSGetViewports(&num_viewports, &viewport);

	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mapped_buffer;
	hr = g_pImmediateContext->Map(instance_buffer.Get(), 0, map, 0, &mapped_buffer);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	instances = static_cast<instance*>(mapped_buffer.pData);

	count_instance = 0;
}
void sprite_batch::render(ID3D11DeviceContext* g_pImmediateContext,
	float dx, float dy,
	float dw, float dh,
	float sx, float sy,
	float sw, float sh,
	float angle,
	float r, float g, float b, float a)
{
	_ASSERT_EXPR(count_instance < MAX_INSTANCES, L"Number of instances must be less than MAX_INSTANCES.");

	float cx = dw * 0.5f, cy = dh * 0.5f; //中心座標
	FLOAT c = cos(XMConvertToRadians(angle));
	FLOAT s = sin(XMConvertToRadians(angle));
	FLOAT w = 2.0f / viewport.Width;
	FLOAT h = -2.0f / viewport.Height;
	instances[count_instance].ndc_transform._11 = w * dw * c;
	instances[count_instance].ndc_transform._21 = h * dw * s;
	instances[count_instance].ndc_transform._31 = 0.0f;
	instances[count_instance].ndc_transform._41 = 0.0f;
	instances[count_instance].ndc_transform._12 = w * dh * -s;
	instances[count_instance].ndc_transform._22 = h * dh * c;
	instances[count_instance].ndc_transform._32 = 0.0f;
	instances[count_instance].ndc_transform._42 = 0.0f;
	instances[count_instance].ndc_transform._13 = 0.0f;
	instances[count_instance].ndc_transform._23 = 0.0f;
	instances[count_instance].ndc_transform._33 = 1.0f;
	instances[count_instance].ndc_transform._43 = 0.0f;
	instances[count_instance].ndc_transform._14 = w * (-cx * c + -cy * -s + cx + dx) - 1.0f;
	instances[count_instance].ndc_transform._24 = h * (-cx * s + -cy * c + cy + dy) + 1.0f;
	instances[count_instance].ndc_transform._34 = 0.0f;
	instances[count_instance].ndc_transform._44 = 1.0f;

	float tw = static_cast<float>(g_TEXTURE2D_DESC.Width);
	float th = static_cast<float>(g_TEXTURE2D_DESC.Height);
	instances[count_instance].texcoord_transform = XMFLOAT4(sx / tw, sy / th, sw / tw, sh / th);
	instances[count_instance].color = XMFLOAT4(r, g, b, a);

	count_instance++;
}

void sprite_batch::end(ID3D11DeviceContext* g_pImmediateContext)
{
	g_pImmediateContext->Unmap(instance_buffer.Get(), 0);

	g_pImmediateContext->DrawInstanced(4, count_instance, 0, 0);
}
*/