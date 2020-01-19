#pragma once
#include <stdio.h>
#include <string>
#include <list>
#include <memory>
#include "Object.h"

class Component;
class Transform;

class GameObject : public Object
{
public:
	unsigned int ID = NULL;
	int layer = 0;
	std::string tag = nullptr;
	std::shared_ptr<Transform> transform;
	std::list<std::shared_ptr<Component>> Component_List;

	GameObject();
	~GameObject();

	bool CompareTag(std::string _tag);
	bool activeSelf();
	void SetActive(bool value);

	template<class T>
	std::shared_ptr<T> GetComponent();
	template<class T>
	std::shared_ptr<T> AddComponent();

	static std::weak_ptr<GameObject> Find(std::string Name);
	static std::weak_ptr<GameObject> FindWithTag(std::string Tag);

private:
	bool Active = true;
};

template<class T>
std::shared_ptr<T> GameObject::GetComponent()
{
	for (std::shared_ptr<Component> com : Component_List) {
		std::shared_ptr<T> buff = std::static_pointer_cast<T>(com);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;
}

template<class T>
std::shared_ptr<T> GameObject::AddComponent()
{
	std::shared_ptr<T> buff = std::make_shared<T>();
	Component_List.push_back(buff);
	buff->Initialize(std::static_pointer_cast<GameObject>(shared_from_this()));
	return buff;
}
