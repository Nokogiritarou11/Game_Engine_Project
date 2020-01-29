#include "Scene.h"
#include "GameObject_Manager.h"
#include "MonoBehaviour_Manager.h"
#include "Mesh_Renderer.h"
#include "SkinMesh_Renderer.h"
#include "Sprite_Renderer.h"
#include "Animator.h"
#include "Transform.h"
#include "All_Script.h"
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
	shared_ptr<GameObject> player = GameObject::Instantiate("player");
	player->tag = "Player";
	player->transform->eulerAngles = { -90,0,0 };
	player->transform->scale = { 0.1f,0.1f ,0.1f };
	player->AddComponent<Player>();
	shared_ptr<SkinMesh_Renderer> renderer = player->AddComponent<SkinMesh_Renderer>();
	renderer->Set_Mesh(Mesh::Load_Mesh("Model\\danbo_fbx\\", "danbo_taiki.txt"));
	shared_ptr<Animator> animator = player->AddComponent<Animator>();
	animator->Set_Skin_Renderer(renderer);
	animator->Animation_Loop = true;

	shared_ptr<GameObject> Floor = GameObject::Instantiate("Floor");
	shared_ptr<SkinMesh_Renderer> f_renderer = Floor->AddComponent<SkinMesh_Renderer>();
	f_renderer->Set_Mesh(Mesh::Load_Mesh("Model\\untitled_fbx\\", "untitled.txt"));

	shared_ptr<GameObject> G_Manager = GameObject::Instantiate("Game_Manager");
	G_Manager->AddComponent<Game_Manager>();
	G_Manager->AddComponent<UI_Controller>();

	shared_ptr<GameObject> enemy = GameObject::Instantiate("enemy");
	enemy->transform->position = { 25,0,0 };
	enemy->transform->scale = { 0.2f,0.2f ,0.2f };
	enemy->AddComponent<Enemy>();
	shared_ptr<SkinMesh_Renderer> e_renderer = enemy->AddComponent<SkinMesh_Renderer>();
	e_renderer->Set_Mesh(Mesh::Load_Mesh("Model\\danbo_fbx\\", "danbo_taiki.txt"));
	shared_ptr<Animator> e_animator = enemy->AddComponent<Animator>();
	e_animator->Set_Skin_Renderer(e_renderer);
	e_animator->Animation_Loop = true;

	/*
	shared_ptr<GameObject> image = GameObject::Instantiate("image");
	shared_ptr<Sprite_Renderer> s_renderer = image->AddComponent<Sprite_Renderer>();
	s_renderer->Set_Texture("Test", L"Code/Shader/2D.fx", L"Image\\UI”Ä—pŽlŠp.png");
	image->transform->position = {100,100,0};
	s_renderer->material->color = { 1.0f,1.0f,1.0f,1.0f };
	*/

	shared_ptr<GameObject> camera = GameObject::Instantiate("Main_Camera");
	shared_ptr<Camera> camera_Comp = camera->AddComponent<Camera>();
	camera->transform->position = { 0,150,-80.0f };
	camera->transform->eulerAngles = { 60,0,0 };
}