#pragma once
#include "Renderer.h"
#include "Mesh_Renderer.h"
#include "Camera.h"

class Render_Manager
{
public:
	Render_Manager();
	~Render_Manager();

	static void Add(std::shared_ptr<Mesh_Renderer> m_rend);
	static void Render(std::shared_ptr<Camera> Render_Camera);
private:

	static std::list<std::weak_ptr<Mesh_Renderer>> Mesh_Renderer_list;
	//static std::list<std::weak_ptr<SkinMesh_Renderer>> SkinMesh_Renderer_list;
};