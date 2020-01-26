#include "Scene.h"
#include "GameObject_Manager.h"
#include "MonoBehaviour_Manager.h"
#include "Mesh_Renderer.h"
#include "SkinMesh_Renderer.h"
#include "Animator.h"
#include "Transform.h"
using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Instance_GameObject(shared_ptr<GameObject> gameObject)
{
	gameObject_List.emplace_back(gameObject);
}

void Scene::Destroy_GameObject(shared_ptr<GameObject> gameObject)
{
	list<shared_ptr<GameObject>>::iterator itr_end = gameObject_List.end();
	for (list<shared_ptr<GameObject>>::iterator itr = gameObject_List.begin(); itr != itr_end; itr++)
	{
		if ((*itr)->ID == gameObject->ID)
		{
			gameObject_List.erase(itr);
			return;
		}
	}
}

void Scene::Update()
{
	GameObject_Manager::Update();
	MonoBehaviour_Manager::Update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void Test_Scene::Initialize()
{
	shared_ptr<GameObject> Danbo = GameObject::Instantiate("Danbo");
	shared_ptr<SkinMesh_Renderer> renderer = Danbo->AddComponent<SkinMesh_Renderer>();
	shared_ptr<Animator> animator = Danbo->AddComponent<Animator>();
	animator->Set_Skin_Renderer(renderer);
	animator->Animation_Loop = true;
	renderer->Set_Mesh(Mesh::Load_Mesh("Model\\danbo_fbx\\", "danbo_taiki.txt"));
	Danbo->transform->eulerAngles = { -90,0,0 };
	Danbo->transform->scale = { 0.1f,0.1f ,0.1f };

	shared_ptr<GameObject> camera = GameObject::Instantiate("Main_Camera");
	shared_ptr<Camera> camera_Comp = camera->AddComponent<Camera>();
	camera->transform->position = { 0,5,-25.0f };
}