#include "Mesh_Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Render_Manager.h"
using namespace std;

Mesh_Renderer::Mesh_Renderer()
{
}

Mesh_Renderer::~Mesh_Renderer()
{
}

void Mesh_Renderer::Initialize(shared_ptr<GameObject> obj)
{
	gameObject = obj;
	transform = obj->transform;
	Render_Manager::Add(static_pointer_cast<Mesh_Renderer>(shared_from_this()));
}

void Mesh_Renderer::Set_Mesh(std::shared_ptr<Mesh> Mesh_Data)
{
	mesh_data = move(Mesh_Data);
	//material.clear();
	unsigned long Subset_ID = 0;
	for (int i = 0; i < mesh_data->skin_meshes.size(); i++)
	{
		for (int j = 0; j < mesh_data->skin_meshes[i].subsets.size(); j++)
		{
			mesh_data->skin_meshes[i].subsets[j].diffuse.ID = Subset_ID;
			string Mat_Name = mesh_data->name + "_" + mesh_data->skin_meshes[i].subsets[j].diffuse.TexName;
			shared_ptr<Material> Mat = Material::Create(Mat_Name, L"Code/Shader/Default_Mesh.fx", mesh_data->skin_meshes[i].subsets[j].diffuse.TexPass.c_str());
			material.emplace_back(Mat);
			Subset_ID++;
		}
	}
}

void Mesh_Renderer::Render(std::shared_ptr<Camera> Render_Camera)
{
	// ���[���h�s��A�r���[�s��A�v���W�F�N�V�����s����������s��f�[�^�����o���B
	XMMATRIX WVP;
	XMFLOAT4X4 world_view_projection;
	WVP = XMLoadFloat4x4(&transform->world) * XMLoadFloat4x4(&Render_Camera->V) * XMLoadFloat4x4(&Render_Camera->P);
	XMStoreFloat4x4(&world_view_projection, WVP);

	DxSystem::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : mesh_data->skin_meshes)
	{
		// �g�p���钸�_�o�b�t�@��V�F�[�_�[�Ȃǂ�GPU�ɋ����Ă��B
		UINT stride = sizeof(Mesh::vertex);
		UINT offset = 0;
		DxSystem::DeviceContext->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		DxSystem::DeviceContext->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		for (auto& subset : mesh.subsets)
		{
			// �萔�o�b�t�@�̓��e���X�V
			Mesh::cbuffer data;
			data.material_color = material[subset.diffuse.ID]->color;
			data.light_direction = DxSystem::Light_Direction;
			XMStoreFloat4x4(&data.world_view_projection,
				XMLoadFloat4x4(&mesh.global_transform) *
				XMLoadFloat4x4(&transform->coordinate_conversion) *
				XMLoadFloat4x4(&world_view_projection));
			XMStoreFloat4x4(&data.world,
				XMLoadFloat4x4(&mesh.global_transform) *
				XMLoadFloat4x4(&transform->coordinate_conversion) *
				XMLoadFloat4x4(&transform->world));
			DxSystem::DeviceContext->UpdateSubresource(mesh_data->ConstantBuffer.Get(), 0, nullptr, &data, 0, 0);
			DxSystem::DeviceContext->VSSetConstantBuffers(0, 1, mesh_data->ConstantBuffer.GetAddressOf());
			DxSystem::DeviceContext->IASetInputLayout(material[subset.diffuse.ID]->shader->VertexLayout.Get());
			//�V�F�[�_�[���\�[�X�̃o�C���h
			material[subset.diffuse.ID]->texture->Set(); //PSSetSamplar PSSetShaderResources
			material[subset.diffuse.ID]->shader->Activate(); //PS,VSSetShader
			// ���Őݒ肵�����\�[�X�𗘗p���ă|���S����`�悷��B
			DxSystem::DeviceContext->DrawIndexed(subset.index_count, subset.index_start, 0);
		}
	}
}