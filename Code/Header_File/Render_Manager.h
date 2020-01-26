#pragma once
#include "Renderer.h"
#include "Mesh_Renderer.h"
#include "SkinMesh_Renderer.h"
#include "Camera.h"

class Render_Manager
{
public:
	Render_Manager();
	~Render_Manager();

	static void Add(std::shared_ptr<Mesh_Renderer> m_rend);
	static void Add(std::shared_ptr<SkinMesh_Renderer> m_rend);
	static void Render(std::shared_ptr<Camera> Render_Camera);
private:

	static std::list<std::weak_ptr<Renderer>> Renderer_list;
};