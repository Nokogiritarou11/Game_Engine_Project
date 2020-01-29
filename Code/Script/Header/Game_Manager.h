#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Transform.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

class Game_Manager : public MonoBehaviour
{
public:
	list<weak_ptr<GameObject>> LockOn_able_List;
	vector<weak_ptr<Transform>> Change_Object;
	bool Locking;
	weak_ptr<Transform> target;
	Vector2 target_pos;
	Vector2 Select_pos;
	weak_ptr<GameObject> player;
	weak_ptr<Camera> camera;
	int Change_State;

	void Start();
	void Update();
	void Targeting_Object();
	void Select_Object();

private:
	int LockOn_Direction_Old;
};