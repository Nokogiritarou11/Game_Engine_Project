#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
#include <wrl.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <locale.h>
#include "Texture.h"
using namespace std;

class skinned_mesh
{
private:

	//Microsoft::WRL::ComPtr <ID3D11VertexShader> g_pVertexShader; //差し替え
	//Microsoft::WRL::ComPtr <ID3D11InputLayout> g_pVertexLayout; //差し替え
	//Microsoft::WRL::ComPtr <ID3D11PixelShader> g_pPixelShader; //差し替え
	//Microsoft::WRL::ComPtr <ID3D11Buffer> g_pConstantBuffer; //差し替え

	//Microsoft::WRL::ComPtr <ID3D11RasterizerState> g_pRasterizerState; //差し替え

	//Microsoft::WRL::ComPtr <ID3D11SamplerState> g_pSamplerState; //差し替え

	XMFLOAT4X4 world_view_projection;
	XMFLOAT4X4 world;

public:

	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotate = { 0,0,0 };
	XMFLOAT3 translate = { 0,0,0 };
	int anim_index = 0;
	float degree = 0;

	float anim_time = 0;
	float anim_rate = 0;
	int old_anim = 0;
	bool anim_start_set = false;
	bool anim_once = false;
	bool anim_once_end = false;

	XMFLOAT4X4 W = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texcoord;
#define MAX_BONE_INFLUENCES 4
		FLOAT bone_weights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
		INT bone_indices[MAX_BONE_INFLUENCES] = {};
	};

	struct cbuffer
	{
#define MAX_BONES 32
		XMFLOAT4X4 world_view_projection; //ワールド・ビュー・プロジェクション合成行列
		XMFLOAT4X4 world; //ワールド変換行列
		XMFLOAT4 material_color; //材質色
		XMFLOAT4X4 bone_transforms[MAX_BONES] = { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
	};

	struct material
	{
		XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f };
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> shader_resource_view;
		Microsoft::WRL::ComPtr <ID3D11PixelShader> g_pPixelShader;
	};

	struct subset
	{
		u_int index_start = 0; // start number of index buffer
		u_int index_count = 0; // number of vertices (indices)
		material diffuse;
	};

	struct bone
	{
		XMFLOAT4X4 transform;
	};

	typedef vector<bone> skeletal;

	struct skeletal_animation : public vector<skeletal>
	{
		float sampling_time = 1 / 24.0f;
		float animation_tick = 0.0f;
	};

	struct mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
		vector<subset> subsets;
		XMFLOAT4X4 global_transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		vector<skeletal_animation> skeletal_animation;

	};
	typedef vector<mesh> meshes;
	struct bone_influence
	{
		int index; // index of bone
		float weight; // weight of bone
	};

	typedef vector<bone_influence> bone_influences_per_control_point;

	meshes skin_meshes;

	// convert coordinate system from 'UP:+Z FRONT:+Y RIGHT-HAND' to 'UP:+Y FRONT:+Z LEFT-HAND'
	XMFLOAT4X4 coordinate_conversion = {
	1, 0, 0, 0,
	0, 0, 1, 0,
	0, 1, 0, 0,
	0, 0, 0, 1
	};

	skinned_mesh();
	/*~skinned_mesh()
	{

	};*/
	void Update_World_Matrix();

	void render();
};