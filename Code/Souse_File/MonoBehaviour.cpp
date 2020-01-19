#include "MonoBehaviour.h"
#include "MonoBehaviour_Manager.h"
#include "GameObject.h"
using namespace std;

MonoBehaviour::MonoBehaviour()
{
}

MonoBehaviour::~MonoBehaviour()
{
}

void MonoBehaviour::Initialize(shared_ptr<GameObject> obj)
{
	transform = obj->transform;
	MonoBehaviour_Manager::Add(dynamic_pointer_cast<MonoBehaviour>(shared_from_this()));
}