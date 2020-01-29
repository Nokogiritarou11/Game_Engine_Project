#include "Enemy.h"
#include "Player.h"
#include "Game_Manager.h"

void Enemy::OnEnable()
{
	if (game_manager.expired())
	{
		shared_ptr<GameObject> obj = GameObject::Find("Game_Manager").lock();
		game_manager = obj->GetComponent<Game_Manager>();
	}

	shared_ptr<Game_Manager> manager = game_manager.lock();
	manager->LockOn_able_List.emplace_back(gameObject);
}

void Enemy::Start()
{
	player = GameObject::Find("Player");
}

void Enemy::Update()
{
	//shared_ptr<GameObject> p = player.lock();
}
