#include "skinned_mesh.h"
#include "DxSystem.h"
#include <vector>
#include <functional>
#include "misc.h"
#include "camera.h"
#include "Time.h"

skinned_mesh::skinned_mesh()
{
	HRESULT hr = S_OK;

	// GPUに頂点データの内容を教えてあげるための設定
	// UNIT10.5
	// 入力レイアウトの要素に法線を加える。
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layout);
}

void skinned_mesh::Update_World_Matrix()
{
	// ワールド行列を作成
	{
		XMMATRIX S, R, T;
		S = XMMatrixScaling(scale.x, scale.y, scale.z);
		R = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));	// ZXY回転
		T = XMMatrixTranslation(translate.x, translate.y, translate.z);

		XMStoreFloat4x4(&W, S * R * T);
	}
}

void skinned_mesh::render()
{
	Update_World_Matrix();
	// ワールド行列、ビュー行列、プロジェクション行列を合成し行列データを取り出す。
	{
		XMMATRIX WVP;
		shared_ptr<camera> c = camera_trans.lock();
		WVP = XMLoadFloat4x4(&W) * XMLoadFloat4x4(&c->V) * XMLoadFloat4x4(&c->P);
		XMStoreFloat4x4(&world_view_projection, WVP);
	}
	// ワールド行列の行列データを取り出す
	{
		world = W;
	}

	DxSystem::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ポリゴンの描き方の指定
	//深度ステンシルステートの設定
	DxSystem::DeviceContext->OMSetDepthStencilState(NULL, 1);

	anim_time += DxSystem::elapsed_time;
	if (!anim_once)anim_once_end = false;

	for (auto& mesh : skin_meshes)
	{
		// 使用する頂点バッファやシェーダーなどをGPUに教えてやる。
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		DxSystem::DeviceContext->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		DxSystem::DeviceContext->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		for (auto& subset : mesh.subsets)
		{
			// 定数バッファの内容を更新
			cbuffer data;
			data.material_color = subset.diffuse.color;
			XMStoreFloat4x4(&data.world_view_projection,
				XMLoadFloat4x4(&mesh.global_transform) *
				XMLoadFloat4x4(&coordinate_conversion) *
				XMLoadFloat4x4(&world_view_projection));
			XMStoreFloat4x4(&data.world,
				XMLoadFloat4x4(&mesh.global_transform) *
				XMLoadFloat4x4(&coordinate_conversion) *
				XMLoadFloat4x4(&world));
			if (mesh.skeletal_animation.size() > 0 && mesh.skeletal_animation[anim_index].size() > 0)
			{
				int frame = 0;
				if (old_anim != anim_index)
				{
					anim_once_end = false;
					anim_start_set = true;
				}
				if (anim_start_set)
				{
					anim_time = 0;
					anim_start_set = false;
				}
				mesh.skeletal_animation[anim_index].animation_tick = anim_time;
				if (!anim_once_end)
				{
					frame = mesh.skeletal_animation[anim_index].animation_tick / mesh.skeletal_animation[anim_index].sampling_time;
					if (frame > mesh.skeletal_animation[anim_index].size() - 1)
					{
						if (!anim_once)
						{
							frame = 0;
							anim_once_end = false;
							mesh.skeletal_animation[anim_index].animation_tick = 0.0f;
						}
						else
						{
							frame = mesh.skeletal_animation[anim_index].size() - 1;
							anim_once_end = true;
						}
						anim_time = 0.0f;
					}
				}
				else
				{
					frame = mesh.skeletal_animation[anim_index].size() - 1;
				}
				vector<bone> skeletal = mesh.skeletal_animation[anim_index].at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");
				for (size_t i = 0; i < number_of_bones; i++)
				{
					XMStoreFloat4x4(&data.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
				}

				anim_rate = (float)frame / mesh.skeletal_animation[anim_index].size();
				old_anim = anim_index;
			}

			// ↑で設定したリソースを利用してポリゴンを描画する。
			DxSystem::DeviceContext->DrawIndexed(subset.index_count, subset.index_start, 0);
		}
		if (mesh.subsets.size() == 0)
		{
			// 定数バッファの内容を更新
			cbuffer data;
			XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f };
			data.material_color = color;
			// UNIT10.5
			// ライティング計算用のパラメータ
			XMStoreFloat4x4(&data.world_view_projection,
				XMLoadFloat4x4(&mesh.global_transform) *
				XMLoadFloat4x4(&coordinate_conversion) *
				XMLoadFloat4x4(&world_view_projection));
			XMStoreFloat4x4(&data.world,
				XMLoadFloat4x4(&mesh.global_transform) *
				XMLoadFloat4x4(&coordinate_conversion) *
				XMLoadFloat4x4(&world));

			if (mesh.skeletal_animation[anim_index].size() > 0)
			{
				int frame = mesh.skeletal_animation[anim_index].animation_tick / mesh.skeletal_animation[anim_index].sampling_time;
				if (frame > mesh.skeletal_animation[anim_index].size() - 1)
				{
					frame = 0;
					mesh.skeletal_animation[anim_index].animation_tick = 0.0f;
				}
				vector<bone> skeletal = mesh.skeletal_animation[anim_index].at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");
				for (size_t i = 0; i < number_of_bones; i++)
				{
					XMStoreFloat4x4(&data.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
				}
				mesh.skeletal_animation[anim_index].animation_tick += Time::deltaTime;
			}

			DxSystem::DeviceContext->VSSetConstantBuffers(0, 1, g_pConstantBuffer.GetAddressOf());

			// ↑で設定したリソースを利用してポリゴンを描画する。
			//DxSystem::DeviceContext->DrawIndexed(vertex_count_mat, 0, 0);
		}
	}
}