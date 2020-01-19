#include "MonoBehaviour_Manager.h"
#include "GameObject.h"
using namespace std;

list<weak_ptr<MonoBehaviour>> MonoBehaviour_Manager::MonoBehaviour_Update_list;
list<weak_ptr<MonoBehaviour>> MonoBehaviour_Manager::MonoBehaviour_Awake_list;
list<weak_ptr<MonoBehaviour>> MonoBehaviour_Manager::MonoBehaviour_Start_list;

MonoBehaviour_Manager::MonoBehaviour_Manager()
{
}

MonoBehaviour_Manager::~MonoBehaviour_Manager()
{
}

void MonoBehaviour_Manager::Reset()
{
	MonoBehaviour_Update_list.clear();
}

void MonoBehaviour_Manager::Update()
{
	list<weak_ptr<MonoBehaviour>>::iterator itr_end = MonoBehaviour_Update_list.end();
	for (list<weak_ptr<MonoBehaviour>>::iterator itr = MonoBehaviour_Update_list.begin(); itr != itr_end;)
	{
		if (itr->expired())
		{
			MonoBehaviour_Update_list.erase(itr);
			continue;
		}
		shared_ptr<MonoBehaviour> mono = itr->lock();
		if (mono->gameObject->activeSelf())
		{
			if (mono->enabled)
			{
				mono->Update();
				mono->LateUpdate();
			}
		}
		itr++;
	}
}

void MonoBehaviour_Manager::Add(shared_ptr<MonoBehaviour> mono)
{
	MonoBehaviour_Update_list.emplace_back(mono);
}