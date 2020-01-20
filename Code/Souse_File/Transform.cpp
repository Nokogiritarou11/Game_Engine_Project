#include "Transform.h"
#include "GameObject.h"
using namespace DirectX;
using namespace std;

Transform::Transform()
{

}

Transform::Transform(XMFLOAT3 _position, XMFLOAT4 _rotation)
{
	position = _position;
	rotation = _rotation;
}

Transform::Transform(XMFLOAT3 _position, XMFLOAT3 _euler)
{
	position = _position;
	eulerAngles = _euler;
}

Transform::~Transform()
{
}

void Transform::Initialize(shared_ptr<GameObject> obj)
{
	gameObject = obj;
	obj->transform = static_pointer_cast<Transform>(shared_from_this());
	transform = static_pointer_cast<Transform>(shared_from_this());
}

void Transform::Update()
{
	// ワールド行列を作成
	{
		XMMATRIX S, R, T;
		S = XMMatrixScaling(scale.x, scale.y, scale.z);
		R = XMMatrixRotationRollPitchYaw(XMConvertToRadians(eulerAngles.x), XMConvertToRadians(eulerAngles.y), XMConvertToRadians(eulerAngles.z));	// ZXY回転
		T = XMMatrixTranslation(position.x, position.y, position.z);

		XMStoreFloat4x4(&scale_matrix, S);
		XMStoreFloat4x4(&rotation_matrix, R);
		XMStoreFloat4x4(&translation_matrix, T);
		XMStoreFloat4x4(&world, S * R * T);

		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(XMConvertToRadians(eulerAngles.y));

		XMVECTOR right_v, up_v, forward_v;

		forward_v = XMVector3TransformCoord(XMLoadFloat4(&DefaultForward), RotateYTempMatrix);
		right_v = XMVector3TransformCoord(XMLoadFloat4(&DefaultRight), RotateYTempMatrix);
		up_v = XMVector3TransformCoord(up_v, RotateYTempMatrix);

		XMStoreFloat4(&forward, forward_v);
		XMStoreFloat4(&right, right_v);
		XMStoreFloat4(&up, up_v);
	}
}