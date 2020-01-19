#include "Scene_Manager.h"
using namespace std;

//**********************************************
//
//		�V�[���̊Ǘ�
//
//**********************************************

Scene_Manager::Scene_Manager()
{
}

Scene_Manager::~Scene_Manager()
{
}

void Scene_Manager::CreateScene(Scene Scene_Class, string Scene_Name)
{
	list<Scene_Data>::iterator itr_end = Scene_List.end();
	for (list<Scene_Data>::iterator itr = Scene_List.begin(); itr != itr_end; itr++)
	{
		if (itr->Name == Scene_Name)
		{
			return;
		}
	}
	shared_ptr<Scene> new_scene = make_shared<Scene>();
	Scene_Data new_data = { Scene_Name,new_scene };
	Scene_List.emplace_back(new_data);
}

void Scene_Manager::LoadScene(string Scene_Name)
{
	Load = true;
	Next_Scene_Name = Scene_Name;
}

void Scene_Manager::Instance_GameObject(shared_ptr<GameObject> gameObject)
{
	shared_ptr<Scene> scene = Active_Scene.lock();
	scene->Instance_GameObject(gameObject);
}

void Scene_Manager::Destroy_GameObject(shared_ptr<GameObject> gameObject)
{
	shared_ptr<Scene> scene = Active_Scene.lock();
	scene->Destroy_GameObject(gameObject);
}

void Scene_Manager::Update()
{
	if (Load)
	{
		list<Scene_Data>::iterator itr_end = Scene_List.end();
		for (list<Scene_Data>::iterator itr = Scene_List.begin(); itr != itr_end; itr++)
		{
			if (itr->Name == Next_Scene_Name)
			{
				Active_Scene = itr->Scene_ptr;
				Load = false;
				break;
			}
		}
	}

	shared_ptr<Scene> Play_Scene = Active_Scene.lock();
	Play_Scene->Update();
}