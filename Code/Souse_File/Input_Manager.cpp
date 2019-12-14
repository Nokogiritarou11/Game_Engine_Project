#include "Input_Manager.h"

Input_Manager::Input_Manager()
{
	mouse = make_unique<Mouse>();
	gamepad = make_unique<GamePad>();
	keyboard = make_unique<Keyboard>();

}

void Input_Manager::Update()
{
	mouse->SetMode(Mouse::MODE_RELATIVE);

	kb = keyboard->GetState();
	key_tracker.Update(kb);
	ms = mouse->GetState();

	mouse_tracker.Update(ms);
	pad = gamepad->GetState(0);
	if (pad.IsConnected())
	{
		pad_tracker.Update(pad);
	}

}