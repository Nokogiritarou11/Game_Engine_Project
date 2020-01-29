#pragma once
#include "MonoBehaviour.h"
#include "Transform.h"
using namespace DirectX;
using namespace std;

class Game_Manager;

class Player : public MonoBehaviour
{
public:
	void Start();
	void Update();

private:
	float move_speed = 30.0f;
	weak_ptr<Game_Manager> game_manager;
};