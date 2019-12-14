#include "Game_System.h"
#include <algorithm>
#include <random>
#include <time.h>

Game_System::Game_System(ID3D11Device* device, ID3D11DeviceContext* g_pImmediateContext)
{
	D3D11_VIEWPORT viewport;
	UINT num_viewports = 1;
	g_pImmediateContext->RSGetViewports(&num_viewports, &viewport);
	screen_width = viewport.Width;
	screen_height = viewport.Height;
	blend = make_unique<blender>(device);

	camera_trans = make_shared<camera>(g_pImmediateContext);
	//////      3D      //////
	//  タイトル  //

	//  ゲーム  //

	//  リザルト  //


	//////      2D      //////
	//  タイトル  //
	//  ゲーム  //
	//  リザルト  //
	//  初期配置  //
}

Game_System::~Game_System()
{

}

void Game_System::Update(float elapsed_time)
{
	switch (Scene)
	{
	case Title:
		title_update(elapsed_time);
		break;

	case Game:
		game_update(elapsed_time);
		break;

	case Result:
		result_update(elapsed_time);
		break;

	default:
		break;
	}
}


void Game_System::title_update(float elapsed_time)
{
	if (Old_Scene != Title)
	{
		Old_Scene = Title;
	}

	if (Input.key_tracker.pressed.Space)
	{
		Scene = Game;
	}
	camera_trans->Update();
}

void Game_System::game_update(float elapsed_time)
{
	if (Old_Scene != Game)
	{
		Old_Scene = Game;
	}
	if (Input.key_tracker.pressed.Space)
	{
		Scene = Result;
	}
	camera_trans->Update();
}

void Game_System::result_update(float elapsed_time)
{
	if (Old_Scene != Result)
	{
		Old_Scene = Result;
	}

	if (Input.key_tracker.pressed.Space)
	{
		Scene = Title;
	}
	camera_trans->Update();
}

void Game_System::Render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time)
{
	switch (Scene)
	{
	case Title:
		title_render(g_pImmediateContext, light_direction, elapsed_time);
		break;

	case Game:
		game_render(g_pImmediateContext, light_direction, elapsed_time);
		break;

	case Result:
		result_render(g_pImmediateContext, light_direction, elapsed_time);
		break;

	default:
		break;
	}
}

void Game_System::title_render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time)
{
	g_pImmediateContext->OMSetBlendState(blend->states[blender::BS_ALPHA].Get(), nullptr, 0xFFFFFFFF);
}

void Game_System::game_render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time)
{
	//      3D      //
	//g_pImmediateContext->OMSetBlendState(blend->states[blender::BS_NONE].Get(), nullptr, 0xFFFFFFFF);
	//      2D      //
	//g_pImmediateContext->OMSetBlendState(blend->states[blender::BS_ALPHA].Get(), nullptr, 0xFFFFFFFF);

}

void Game_System::result_render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time)
{
}