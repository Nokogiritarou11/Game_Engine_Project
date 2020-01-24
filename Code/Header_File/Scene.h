#pragma once
#include <stdio.h>
#include <string>
#include <list>
#include <memory>
#include "GameObject.h"

class Scene
{
public:

	Scene();
	~Scene();

	virtual void Initialize() {};
	void Instance_GameObject(std::shared_ptr<GameObject> gameObject);
	void Destroy_GameObject(std::shared_ptr<GameObject> gameObject);
	void Update();

private:

	std::list<std::shared_ptr<GameObject>> gameObject_List;

};

class Test_Scene : public Scene
{
public:
	void Initialize();
	
private:

};