#include "Engine.h"
using namespace std;

Engine::Engine()
{
	Input_Manager::Initialize();
	Scene_Manager::CreateScene(make_shared<Test_Scene>(), "Test_Scene");
	Scene_Manager::Set_StartScene("Test_Scene");
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