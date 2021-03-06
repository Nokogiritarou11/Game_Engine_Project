#include "Camera.h"
#include "DxSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera_Manager.h"
using namespace DirectX;
using namespace std;

Camera::Camera()
{
}

void Camera::Initialize(std::shared_ptr<GameObject> obj)
{
	gameObject = obj;
	transform = obj->transform;
	Camera_Manager::Add(static_pointer_cast<Camera>(shared_from_this()));
	DxSystem::DeviceContext->RSGetViewports(&num_viewports, &viewport);
}

void Camera::Update()
{
	// プロジェクション行列を作成
	// 画面サイズ取得のためビューポートを取得
	{
		// 角度をラジアン(θ)に変換
		float fov_y = XMConvertToRadians(FOV);	// 画角
		float aspect = viewport.Width / viewport.Height;	// 画面比率

		XMStoreFloat4x4(&P, XMMatrixPerspectiveFovLH(fov_y, aspect, near_z, far_z));
	}
	// ビュー行列を作成
	// カメラの設定
	{
		XMFLOAT4 eye = { transform->position.x,transform->position.y,transform->position.z ,0 };
		XMVECTOR eye_v = XMLoadFloat4(&eye);

		XMVECTOR focus_v = eye_v + XMLoadFloat4(&transform->forward);

		XMVECTOR camForward = XMVector3Normalize(focus_v - eye_v);    // Get forward vector based on target
		camForward = XMVectorSetY(camForward, 0.0f);    // set forwards y component to 0 so it lays only on
		camForward = XMVector3Normalize(camForward);
		XMVECTOR camRight = XMVectorSet(-XMVectorGetZ(camForward), 0.0f, XMVectorGetX(camForward), 0.0f);

		XMVECTOR up_v = XMVectorSet(0,1,0,0);
		XMStoreFloat4(&focus, focus_v);
		XMStoreFloat4x4(&V, XMMatrixLookAtLH(eye_v, focus_v, up_v));
	}
}

XMFLOAT2 Camera::WorldToViewportPoint(XMFLOAT3 pos)
{
	XMVECTOR p = XMLoadFloat3(&pos);
	XMVECTOR screen
		= XMVector3Project(
			p,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			XMLoadFloat4x4(&P),
			XMLoadFloat4x4(&V),
			XMMatrixIdentity()
		);
	XMFLOAT2 re;
	XMStoreFloat2(&re, screen);
	return re;
}