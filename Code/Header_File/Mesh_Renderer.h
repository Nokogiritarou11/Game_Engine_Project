#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"

class Mesh_Renderer : public Renderer
{
public:

	Mesh_Renderer();
	~Mesh_Renderer();

	void Initialize(std::shared_ptr<GameObject> obj);
	void Set_Mesh(std::shared_ptr<Mesh> Mesh_Data);
	void Render(std::shared_ptr<Camera> Render_Camera);
private:

	std::shared_ptr<Mesh> mesh_data;
};