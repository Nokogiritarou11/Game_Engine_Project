#include "UI_Controller.h"
#include "Game_Manager.h"
#include "Sprite_Renderer.h"

void UI_Controller::Start()
{
	shared_ptr<GameObject> image_set = GameObject::Instantiate("Cursor_Set");
	shared_ptr<Sprite_Renderer> s_renderer = image_set->AddComponent<Sprite_Renderer>();
	s_renderer->Set_Texture("Cursor_Set", L"Code/Shader/2D.fx", L"Image\\ka-soru_R.png");
	Cursor_Set = image_set;

	shared_ptr<GameObject> image_move = GameObject::Instantiate("Cursor_Move");
	shared_ptr<Sprite_Renderer> m_renderer = image_move->AddComponent<Sprite_Renderer>();
	m_renderer->Set_Texture("Cursor_Move", L"Code/Shader/2D.fx", L"Image\\ka-soru_B.png");
	Cursor_Move = image_move;

	shared_ptr<GameObject> obj = GameObject::Find("Game_Manager").lock();
	game_manager = obj->GetComponent<Game_Manager>();
}

void UI_Controller::Update()
{
	shared_ptr<Game_Manager> g_manager = game_manager.lock();
	shared_ptr<GameObject> cursor_set = Cursor_Set.lock();
	shared_ptr<GameObject> cursor_move = Cursor_Move.lock();

	if (g_manager->Change_State == 1)
	{
		if (!cursor_set->activeSelf())
		{
			cursor_set->SetActive(true);
			cursor_set->transform->position = { g_manager->Select_pos.x ,g_manager->Select_pos.y,0 };
		}
	}
	else
	{
		if (cursor_set->activeSelf())
		{
			cursor_set->SetActive(false);
		}
	}

	if (g_manager->Locking)
	{
		if (!cursor_move->activeSelf())
		{
			cursor_move->SetActive(true);
		}
		cursor_move->transform->position = { g_manager->target_pos.x ,g_manager->target_pos.y,0 };
	}
	else
	{
		if (cursor_move->activeSelf())
		{
			cursor_move->SetActive(false);
		}
	}
}