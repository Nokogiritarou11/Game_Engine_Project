#pragma once
#include <stdio.h>
#include <list>
#include <memory>
#include "Camera.h"

class Camera_Manager
{
public:
	Camera_Manager();
	~Camera_Manager();

	static void Reset();

	static void Update();
	static void Add(std::shared_ptr<Camera> mono);

private:
	static std::list<std::weak_ptr<Camera>> Camera_list;

};