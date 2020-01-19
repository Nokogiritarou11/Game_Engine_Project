#pragma once
#include <stdio.h>
#include "Behaviour.h"

class MonoBehaviour : public Behavior
{
public:
	std::list<std::shared_ptr<MonoBehaviour>>::iterator it;

	MonoBehaviour();
	~MonoBehaviour();

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};

	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnDestroy() {};

	void Initialize(shared_ptr<GameObject> obj);
private:

};