#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject_Manager.h"
using namespace std;

GameObject::GameObject()
{
    AddComponent<Transform>();
}

GameObject::~GameObject()
{
    Component_List.clear();
}

bool GameObject::CompareTag(string _tag)
{
    if (tag == _tag)
    {
        return true;
    }
    return false;
}

bool GameObject::activeSelf()
{
    return Active;
}

void GameObject::SetActive(bool value)
{
    Active = value;
}

weak_ptr<GameObject> GameObject::Find(string Name)
{
    return GameObject_Manager::Find(Name);
}

weak_ptr<GameObject> GameObject::FindWithTag(string Tag)
{
    return GameObject_Manager::FindWithTag(Tag);
}