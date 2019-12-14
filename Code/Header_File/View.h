#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class View
{
private:
protected:
	XMFLOAT3  Pos,Target,Up;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
	FLOAT Fov;
	FLOAT Aspect;
	FLOAT Near;
	FLOAT Far;
	FLOAT Width;
	FLOAT Height;
	bool bView;
	
public:
	View();
	virtual ~View();
	void Set(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up);
	void SetProjection(FLOAT fov, FLOAT aspect, FLOAT min, FLOAT max);
	void SetOrtho(FLOAT width, FLOAT height, FLOAT min, FLOAT max);

	void Activate();
	XMMATRIX GetView() { return XMLoadFloat4x4(&ViewMatrix); }
	XMMATRIX GetProjection() { return XMLoadFloat4x4(&ProjectionMatrix); }
	XMFLOAT3 GetPos() { return Pos; }

};