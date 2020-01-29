#pragma once
#include "MonoBehaviour.h"
#include "Transform.h"
using namespace DirectX;
using namespace std;

class Game_Manager;

class UI_Controller : public MonoBehaviour
{
public:

	void Start();
	void Update();

private:

	weak_ptr<Game_Manager> game_manager;

	weak_ptr<GameObject> Cursor_Set;
	weak_ptr<GameObject> Cursor_Move;
};