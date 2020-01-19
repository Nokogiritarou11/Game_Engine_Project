#include "Engine.h"

Engine::Engine()
{

}

Engine::~Engine()
{
	DxSystem::Release();
}

void Engine::Update()
{
	Input_Manager::Update();
	Scene_Manager::Update();
}