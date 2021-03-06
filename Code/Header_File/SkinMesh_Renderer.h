#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"

class SkinMesh_Renderer : public Renderer
{
public:

	SkinMesh_Renderer();
	~SkinMesh_Renderer();

	void Initialize(std::shared_ptr<GameObject> obj);
	void Set_Mesh(std::shared_ptr<Mesh> Mesh_Data);
	void Render(std::shared_ptr<Camera> Render_Camera);

	int Animation_Index = 0;
	float Animation_Time = 0;
	float Animation_Rate = 0;
	bool Animation_Loop = false;
	bool Animation_End = false;

private:

	struct cbuffer /////////////////////////////////////////////////////要変更
	{
#define MAX_BONES 32
		DirectX::XMFLOAT4X4 world_view_projection; //ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4 world; //ワールド変換行列
		DirectX::XMFLOAT4 material_color; //材質色
		DirectX::XMFLOAT4 light_direction; //ライト進行方向 
		DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES] = { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
	};

	ComPtr <ID3D11Buffer> ConstantBuffer; //コンスタントバッファ
	std::shared_ptr<Mesh> mesh_data;
};