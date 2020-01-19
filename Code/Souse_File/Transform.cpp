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
	// ÉèÅ[ÉãÉhçsóÒÇçÏê¨
	{
		XMMATRIX S, R, T;
		S = XMMatrixScaling(scale.x, scale.y, scale.z);
		R = XMMatrixRotationRollPitchYaw(XMConvertToRadians(eulerAngles.x), XMConvertToRadians(eulerAngles.y), XMConvertToRadians(eulerAngles.z));	// ZXYâÒì]
		T = XMMatrixTranslation(position.x, position.y, position.z);

		XMStoreFloat4x4(&world, S * R * T);
	}
}