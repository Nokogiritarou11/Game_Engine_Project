#pragma once
#include <stdio.h>
#include <string>
#include <list>
#include <memory>

class Scene
{
public:

	Scene();
	~Scene();

	void Instance_GameObject(std::shared_ptr<GameObject> gameObject);
	void Destroy_GameObject(std::shared_ptr<GameObject> gameObject);
	void Update();

private:

	std::list<std::shared_ptr<GameObject>> gameObject_List;

};