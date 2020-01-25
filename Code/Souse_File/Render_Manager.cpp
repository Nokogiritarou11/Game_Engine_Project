#include "Render_Manager.h"
#include "GameObject.h"
#include "Transform.h"
using namespace std;

list<weak_ptr<Mesh_Renderer>> Render_Manager::Mesh_Renderer_list;

Render_Manager::Render_Manager()
{
}

Render_Manager::~Render_Manager()
{
}

void Render_Manager::Add(shared_ptr<Mesh_Renderer> m_rend)
{
	Mesh_Renderer_list.emplace_back(m_rend);
}

void Render_Manager::Render(std::shared_ptr<Camera> Render_Camera)
{
	// ビューポートの設定
	DxSystem::SetViewPort(DxSystem::GetScreenWidth(), DxSystem::GetScreenHeight());
	//ブレンドステート設定
	DxSystem::DeviceContext->OMSetBlendState(DxSystem::GetBlendState(DxSystem::BS_NONE), nullptr, 0xFFFFFFFF);
	//ラスタライザ―設定
	DxSystem::DeviceContext->RSSetState(DxSystem::GetRasterizerState(DxSystem::RS_STANDARD));
	//デプスステンシルステート設定
	DxSystem::DeviceContext->OMSetDepthStencilState(DxSystem::GetDephtStencilState(DxSystem::DS_TRUE), 1);

	list<weak_ptr<Mesh_Renderer>>::iterator itr_end = Mesh_Renderer_list.end();
	for (list<weak_ptr<Mesh_Renderer>>::iterator itr = Mesh_Renderer_list.begin(); itr != itr_end;)
	{
		if (itr->expired())
		{
			Mesh_Renderer_list.erase(itr);
			continue;
		}
		shared_ptr<Mesh_Renderer> m_rend = itr->lock();
		if (m_rend->gameObject->activeSelf())
		{
			if (m_rend->enabled)
			{
				m_rend->Render(Render_Camera);
			}
		}
		itr++;
	}
}