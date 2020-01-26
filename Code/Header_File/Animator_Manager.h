#pragma once
#include "Animator.h"

class Animator_Manager
{
public:
	Animator_Manager();
	~Animator_Manager();

	static void Reset();

	static void Update();
	static void Add(std::shared_ptr<Animator> aniamtor);

private:
	static std::list<std::weak_ptr<Animator>> Animator_list;

};