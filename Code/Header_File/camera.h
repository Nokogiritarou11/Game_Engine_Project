#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <stdio.h>
#include <memory>
using namespace std;


class camera
{
public:

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camRight_set = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cam_Light = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMFLOAT4 eye = { 0.0f, 5.0f, -0.5f, 0.0f };
	XMFLOAT4 focus = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMFLOAT4 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;
	float eye_x = 0.0f;
	float eye_y = 0.0f;
	float eye_z = 0.0f;

	float lerp_timer = 0;

	XMFLOAT3 pos = { 0.0f,400.0f,-400.0f };
	XMFLOAT3 rot = { 0.0f,1.0f,0.0f };
	XMFLOAT3 look = { 0.0f,0.0f,0.0f };

	XMFLOAT4X4 V = { 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	XMFLOAT4X4 P = { 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	camera(ID3D11DeviceContext* g_pImmediateContext);

	//void Init_Camera(shared_ptr<player> player);
	void Update();
	void Update_FPS();
	void Update_TPS(XMFLOAT3 pos);
	void Update_LookAt(XMFLOAT3 root, XMFLOAT3 pos, float elapsed_time);
	XMFLOAT2 ToScreenPos(XMFLOAT3 pos);
private:

	D3D11_VIEWPORT viewport;
	UINT num_viewports = 1;
};