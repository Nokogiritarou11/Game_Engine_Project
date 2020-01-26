#include "Animator_Manager.h"
#include "GameObject.h"
using namespace std;

list<weak_ptr<Animator>> Animator_Manager::Animator_list;

Animator_Manager::Animator_Manager()
{
}

Animator_Manager::~Animator_Manager()
{
}

void Animator_Manager::Reset()
{
	Animator_list.clear();
}

void Animator_Manager::Update()
{
	list<weak_ptr<Animator>>::iterator itr_end = Animator_list.end();
	for (list<weak_ptr<Animator>>::iterator itr = Animator_list.begin(); itr != itr_end;)
	{
		if (itr->expired())
		{
			Animator_list.erase(itr);
			continue;
		}
		shared_ptr<Animator> animator = itr->lock();
		if (animator->gameObject->activeSelf())
		{
			if (animator->enabled)
			{
				animator->Update();
			}
		}
		itr++;
	}
}

void Animator_Manager::Add(shared_ptr<Animator> animator)
{
	Animator_list.emplace_back(animator);
}
