#include "geometric_primitive.h"
#include "framework.h"
#include "Resource_Manager.h"
#include "misc.h"

geometric_primitive::geometric_primitive(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	// 頂点バッファの準備
	vertex vertices[] = {
		// 正面
		{ XMFLOAT3(-0.5, +0.5, -0.5), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(+0.5, +0.5, -0.5), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(+0.5, -0.5, -0.5), XMFLOAT3(0, 0, -1) },
		// 背面
		{ XMFLOAT3(-0.5, +0.5, +0.5), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(+0.5, +0.5, +0.5), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(-0.5, -0.5, +0.5), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(+0.5, -0.5, +0.5), XMFLOAT3(0, 0, 1) },
		// 右面
		{ XMFLOAT3(+0.5, +0.5, -0.5), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(+0.5, +0.5, +0.5), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(+0.5, -0.5, -0.5), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(+0.5, -0.5, +0.5), XMFLOAT3(1, 0, 0) },
		// 左面
		{ XMFLOAT3(-0.5, +0.5, -0.5), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-0.5, +0.5, +0.5), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-0.5, -0.5, +0.5), XMFLOAT3(-1, 0, 0) },
		// 上面
		{ XMFLOAT3(-0.5, +0.5, +0.5), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(+0.5, +0.5, +0.5), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(-0.5, +0.5, -0.5), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(+0.5, +0.5, -0.5), XMFLOAT3(0, 1, 0) },
		// 下面
		{ XMFLOAT3(-0.5, -0.5, +0.5), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(+0.5, -0.5, +0.5), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(+0.5, -0.5, -0.5), XMFLOAT3(0, -1, 0) },
	};

	// インデックスデータ
	unsigned int indices[] = {
		// 正面
		0, 1, 2,
		2, 1, 3,
		// 背面
		5, 4, 7,
		7, 4, 6,
		// 右面
		8, 9, 10,
		10, 9, 11,
		// 左面
		13, 12, 15,
		15, 12, 14,
		// 上面
		16, 17, 18,
		18, 17, 19,
		// 下面
		21, 20, 23,
		23, 20, 22,
	};

	// 頂点バッファの生成
	{
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = vertices;				// 頂点のアドレス
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&bd, &InitData, g_pVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// インデックスバッファの生成
	{
		D3D11_BUFFER_DESC bd = {};
		//bd.Usage = D3D11_USAGE_DEFAULT;
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(indices);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = indices;				// 頂点のアドレス
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&bd, &InitData, g_pIndexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// 定数バッファの生成
	{
		D3D11_BUFFER_DESC bd = {};
		//bd.Usage = D3D11_USAGE_DEFAULT;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(cbuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		hr = device->CreateBuffer(&bd, nullptr, g_pConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	// GPUに頂点データの内容を教えてあげるための設定
	// UNIT10.5
	// 入力レイアウトの要素に法線を加える。
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// シェーダーファイルの読み込み
	//create_vs_from_cso(device, "geometric_primitive_vs.cso", g_pVertexShader.GetAddressOf(), g_pVertexLayout.GetAddressOf(), layout, numElements);
	//create_ps_from_cso(device, "geometric_primitive_ps.cso", g_pPixelShader.GetAddressOf());

	//ラスタライザ設定
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;	// 塗りつぶし
	rd.CullMode = D3D11_CULL_BACK;	// カリング
	rd.FrontCounterClockwise = FALSE;	// 三角形の時計回りが正面
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = FALSE;	// 距離に基づくクリッピングを有効か
	rd.ScissorEnable = FALSE;	// シザー長方形カリングを有効か
	rd.MultisampleEnable = FALSE;	// MSAAで四辺形かアルファ線を設定
	rd.AntialiasedLineEnable = FALSE;	// ラインAAが有効か
	hr = device->CreateRasterizerState(&rd, g_pRasterizerState.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void geometric_primitive::render(
	ID3D11DeviceContext* g_pImmediateContext, //デバイスコンテキスト
	const XMFLOAT4& color, //材質色
	const XMFLOAT4& light_direction, //ライト進行方向
	const XMFLOAT4X4& world_view_projection, //ワールド・ビュー・プロジェクション合成行列
	const XMFLOAT4X4& world) const //ワールド変換行列
{
	// 定数バッファの内容を更新
	cbuffer data;
	data.world_view_projection = world_view_projection;
	data.material_color = color;
	// UNIT10.5
	// ライティング計算用のパラメータ
	data.world = world;
	data.light_direction = light_direction;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer.Get(), 0, nullptr, &data, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, g_pConstantBuffer.GetAddressOf());


	// 使用する頂点バッファやシェーダーなどをGPUに教えてやる。
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, g_pVertexBuffer.GetAddressOf(), &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ポリゴンの描き方の指定
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout.Get());

	g_pImmediateContext->RSSetState(g_pRasterizerState.Get());

	g_pImmediateContext->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader.Get(), nullptr, 0);

	// ↑で設定したリソースを利用してポリゴンを描画する。
	g_pImmediateContext->DrawIndexed(36, 0, 0);
}