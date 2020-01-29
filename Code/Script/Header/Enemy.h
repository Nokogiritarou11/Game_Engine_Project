#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Transform.h"
using namespace DirectX;
using namespace std;

class Player;
class Game_Manager;

class Enemy : public MonoBehaviour
{
public:
	void OnEnable();
	void Start();
	void Update();

	int HP = 100;

private:
	weak_ptr<GameObject> player;
	weak_ptr<Game_Manager> game_manager;
};