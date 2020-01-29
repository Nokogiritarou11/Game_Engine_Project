#include "Player.h"
#include "Game_Manager.h"

void Player::Start()
{
	shared_ptr<GameObject> obj = GameObject::Find("Game_Manager").lock();
	game_manager = obj->GetComponent<Game_Manager>();
}

void Player::Update()
{
	shared_ptr<Game_Manager> game_m = game_manager.lock();
	if (!game_m->Locking)
	{
		float Lposx = Input_Manager::pad.thumbSticks.leftX;
		float Lposy = Input_Manager::pad.thumbSticks.leftY;

		if (Lposx != 0 || Lposy != 0)
		{
			transform->eulerAngles.y = -XMConvertToDegrees(atan2(Lposy, Lposx)) - 90;
			transform->position.x += Lposx * move_speed * Time::deltaTime;
			transform->position.z += Lposy * move_speed * Time::deltaTime;
		}
	}
}