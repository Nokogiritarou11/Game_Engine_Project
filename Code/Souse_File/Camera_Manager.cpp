#include "Camera_Manager.h"
#include "GameObject.h"
using namespace std;

list<weak_ptr<Camera>> Camera_Manager::Camera_list;

Camera_Manager::Camera_Manager()
{
}

Camera_Manager::~Camera_Manager()
{
}

void Camera_Manager::Reset()
{
	Camera_list.clear();
}

void Camera_Manager::Update()
{
	list<weak_ptr<Camera>>::iterator itr_end = Camera_list.end();
	for (list<weak_ptr<Camera>>::iterator itr = Camera_list.begin(); itr != itr_end;)
	{
		if (itr->expired())
		{
			Camera_list.erase(itr);
			continue;
		}
		shared_ptr<Camera> camera = itr->lock();
		if (camera->gameObject->activeSelf())
		{
			if (camera->enabled)
			{
				camera->Update();
			}
		}
		itr++;
	}
}

void Camera_Manager::Add(shared_ptr<Camera> mono)
{
	Camera_list.emplace_back(mono);
}