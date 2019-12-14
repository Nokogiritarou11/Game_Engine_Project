#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <stdio.h>
using namespace std;
#include <Keyboard.h>
#include <GamePad.h>
#include <Mouse.h>

class Input_Manager
{
public:
	Keyboard::State kb;
	Mouse::State ms;
	GamePad::State pad;
	Keyboard::KeyboardStateTracker key_tracker;
	Mouse::ButtonStateTracker mouse_tracker;
	GamePad::ButtonStateTracker pad_tracker;
	unique_ptr<Mouse> mouse;
	unique_ptr<GamePad> gamepad;
	unique_ptr<Keyboard> keyboard;

	Input_Manager();
	//~Input_Manager();

	void Update();
private:
};
