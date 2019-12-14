#include "DxSystem.h"

#include "View.h"

View::View()
{
	ZeroMemory(this, 0);
	ViewMatrix = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	ProjectionMatrix = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
}
View::~View()
{
}

void View::Set(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up)
{
	Pos = pos;
	Target = target;
	Up = up;
}

void View::SetProjection(FLOAT fov, FLOAT aspect, FLOAT min, FLOAT max)
{
	bView = true;
	Fov = fov;
	Aspect = aspect;
	Near = min;
	Far = max;

}

void View::SetOrtho(FLOAT width, FLOAT height, FLOAT min, FLOAT max)
{
	bView = false;
	Width = width;
	Height = height;
	Near = min;
	Far = max;

}


void View::Activate()
{
	XMVECTOR vPos, vTarget, vUp;
	vPos = XMVectorSet(Pos.x, Pos.y, Pos.z, 0);
	vTarget = XMVectorSet(Target.x, Target.y, Target.z, 0);
	vUp = XMVectorSet(Up.x, Up.y, Up.z, 0);


	XMMATRIX vm = XMMatrixLookAtLH(vPos, vTarget, vUp);
	XMStoreFloat4x4(&ViewMatrix, vm);

	
	XMMATRIX pm = XMMatrixIdentity();
	if (bView) {
		pm = XMMatrixPerspectiveFovLH(Fov, Aspect, Near, Far);
		XMStoreFloat4x4(&ProjectionMatrix, pm);
	}
	else {
		pm = XMMatrixOrthographicLH(Width, Height, Near, Far);
		XMStoreFloat4x4(&ProjectionMatrix, pm);

	}

}
