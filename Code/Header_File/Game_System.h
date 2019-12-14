#pragma once
#include <memory>
#include "high_resolution_timer.h"
#include "singleton.h"
#include "sprite.h"
#include "skinned_mesh.h"
#include "Resource_Manager.h"
#include "camera.h"
#include "Input_Manager.h"
#include "Audio_Manager.h"
#include "blender.h"

class Game_System
{
private:

	shared_ptr<camera> camera_trans;

	//////      3D      //////
	//  タイトル  //

	//  ゲーム  //

	//  リザルト  //

	//////      2D      //////

	//  タイトル  //
	//  ゲーム  //
	//  リザルト  //

	//////   システム   //////
	unique_ptr<sprite> font;

	enum Scene_State
	{
		Title,
		Game,
		Result
	};

	int Scene = 0;
	int Old_Scene = 0;
	int Next_Scene = 0;
	int clear_count = 0;

	float screen_width = 0;
	float screen_height = 0;

	unique_ptr <blender> blend;
	Input_Manager& Input = singleton<Input_Manager>::get_instance();
	Audio_Manager& Audio = singleton<Audio_Manager>::get_instance();

public:

	Game_System(ID3D11Device* device, ID3D11DeviceContext* g_pImmediateContext);
	~Game_System();

	void Update(float elapsed_time);
	void title_update(float elapsed_time);
	void game_update(float elapsed_time);
	void result_update(float elapsed_time);

	void Render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time);
	void title_render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time);
	void game_render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time);
	void result_render(ID3D11DeviceContext* g_pImmediateContext, const XMFLOAT4& light_direction, float elapsed_time);
};
