#include "Engine.h"

Engine::Engine()
{
	Input_Manager::Initialize();
}

Engine::~Engine()
{
	DxSystem::Release();
}

void Engine::Update()
{
	Input_Manager::Update();
	Scene_Manager::Update();
	Camera_Manager::Update();
	DxSystem::Clear();
	Camera_Manager::Render();
	DxSystem::Flip(1);
}