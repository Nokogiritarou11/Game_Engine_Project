#include "geometric_primitive.h"
#include "framework.h"
#include "Resource_Manager.h"
#include "misc.h"

geometric_primitive::geometric_primitive(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	// ���_�o�b�t�@�̏���
	vertex vertices[] = {
		// ����
		{ XMFLOAT3(-0.5, +0.5, -0.5), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(+0.5, +0.5, -0.5), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(+0.5, -0.5, -0.5), XMFLOAT3(0, 0, -1) },
		// �w��
		{ XMFLOAT3(-0.5, +0.5, +0.5), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(+0.5, +0.5, +0.5), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(-0.5, -0.5, +0.5), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(+0.5, -0.5, +0.5), XMFLOAT3(0, 0, 1) },
		// �E��
		{ XMFLOAT3(+0.5, +0.5, -0.5), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(+0.5, +0.5, +0.5), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(+0.5, -0.5, -0.5), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(+0.5, -0.5, +0.5), XMFLOAT3(1, 0, 0) },
		// ����
		{ XMFLOAT3(-0.5, +0.5, -0.5), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-0.5, +0.5, +0.5), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-0.5, -0.5, +0.5), XMFLOAT3(-1, 0, 0) },
		// ���
		{ XMFLOAT3(-0.5, +0.5, +0.5), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(+0.5, +0.5, +0.5), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(-0.5, +0.5, -0.5), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(+0.5, +0.5, -0.5), XMFLOAT3(0, 1, 0) },
		// ����
		{ XMFLOAT3(-0.5, -0.5, +0.5), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(+0.5, -0.5, +0.5), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(+0.5, -0.5, -0.5), XMFLOAT3(0, -1, 0) },
	};

	// �C���f�b�N�X�f�[�^
	unsigned int indices[] = {
		// ����
		0, 1, 2,
		2, 1, 3,
		// �w��
		5, 4, 7,
		7, 4, 6,
		// �E��
		8, 9, 10,
		10, 9, 11,
		// ����
		13, 12, 15,
		15, 12, 14,
		// ���
		16, 17, 18,
		18, 17, 19,
		// ����
		21, 20, 23,
		23, 20, 22,
	};

	// ���_�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = vertices;				// ���_�̃A�h���X
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&bd, &InitData, g_pVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// �C���f�b�N�X�o�b�t�@�̐���
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
		InitData.pSysMem = indices;				// ���_�̃A�h���X
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&bd, &InitData, g_pIndexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// �萔�o�b�t�@�̐���
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
	// GPU�ɒ��_�f�[�^�̓��e�������Ă����邽�߂̐ݒ�
	// UNIT10.5
	// ���̓��C�A�E�g�̗v�f�ɖ@����������B
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// �V�F�[�_�[�t�@�C���̓ǂݍ���
	//create_vs_from_cso(device, "geometric_primitive_vs.cso", g_pVertexShader.GetAddressOf(), g_pVertexLayout.GetAddressOf(), layout, numElements);
	//create_ps_from_cso(device, "geometric_primitive_ps.cso", g_pPixelShader.GetAddressOf());

	//���X�^���C�U�ݒ�
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;	// �h��Ԃ�
	rd.CullMode = D3D11_CULL_BACK;	// �J�����O
	rd.FrontCounterClockwise = FALSE;	// �O�p�`�̎��v��肪����
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = FALSE;	// �����Ɋ�Â��N���b�s���O��L����
	rd.ScissorEnable = FALSE;	// �V�U�[�����`�J�����O��L����
	rd.MultisampleEnable = FALSE;	// MSAA�Ŏl�ӌ`���A���t�@����ݒ�
	rd.AntialiasedLineEnable = FALSE;	// ���C��AA���L����
	hr = device->CreateRasterizerState(&rd, g_pRasterizerState.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void geometric_primitive::render(
	ID3D11DeviceContext* g_pImmediateContext, //�f�o�C�X�R���e�L�X�g
	const XMFLOAT4& color, //�ގ��F
	const XMFLOAT4& light_direction, //���C�g�i�s����
	const XMFLOAT4X4& world_view_projection, //���[���h�E�r���[�E�v���W�F�N�V���������s��
	const XMFLOAT4X4& world) const //���[���h�ϊ��s��
{
	// �萔�o�b�t�@�̓��e���X�V
	cbuffer data;
	data.world_view_projection = world_view_projection;
	data.material_color = color;
	// UNIT10.5
	// ���C�e�B���O�v�Z�p�̃p�����[�^
	data.world = world;
	data.light_direction = light_direction;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer.Get(), 0, nullptr, &data, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, g_pConstantBuffer.GetAddressOf());


	// �g�p���钸�_�o�b�t�@��V�F�[�_�[�Ȃǂ�GPU�ɋ����Ă��B
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, g_pVertexBuffer.GetAddressOf(), &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// �|���S���̕`�����̎w��
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout.Get());

	g_pImmediateContext->RSSetState(g_pRasterizerState.Get());

	g_pImmediateContext->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader.Get(), nullptr, 0);

	// ���Őݒ肵�����\�[�X�𗘗p���ă|���S����`�悷��B
	g_pImmediateContext->DrawIndexed(36, 0, 0);
}