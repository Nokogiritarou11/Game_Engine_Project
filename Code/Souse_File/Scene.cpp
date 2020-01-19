#include "Scene.h"
#include "GameObject_Manager.h"
#include "MonoBehaviour_Manager.h"
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