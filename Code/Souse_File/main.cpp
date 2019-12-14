#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#include <time.h>
#include <WinSDKVer.h>
#include <SDKDDKVer.h>
#include "DxSystem.h"
#include "Scene.h"
#include <sstream>

//float	elapsed_time = 0; // �o�ߎ���

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_RBUTTONUP:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));

}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E����
	TCHAR szWindowClass[] = TEXT("Shader_Sample");
	WNDCLASS wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	RegisterClass(&wcex);

	HWND hWnd;
	hWnd = CreateWindow(szWindowClass,
		szWindowClass,
		WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 1280, 720,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// �f�o�C�X������
	if (FAILED(DxSystem::Initialize(hWnd, DxSystem::GetScreenWidth(), DxSystem::GetScreenHeight())))
	{
		return 0;
	}

	SceneManager* scenemanager = new SceneManager();
	scenemanager->ChangeScene(std::make_unique<SceneTest01>());

	//���C�����[�v
	MSG hMsg = { 0 };
	float Interval = 1.0f;
	DWORD before = GetTickCount64();
	int fps = 0;

	while (hMsg.message != WM_QUIT) {
		if (PeekMessage(&hMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&hMsg);
			DispatchMessage(&hMsg);
		}
		else
		{
			DxSystem::elapsed_time =
				(GetTickCount64() - before) * 0.001f;

			before = GetTickCount64();
			float mspf = 1000.0f / fps;

			Interval -= DxSystem::elapsed_time;
			fps++;
			if (Interval < 0) {
				std::ostringstream outs;
				outs.precision(6);
				outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
				SetWindowTextA(hWnd, outs.str().c_str());
				Interval += 1.0f;
				fps = 0;

			}






			// �X�V�E�`��
			scenemanager->Update();
			scenemanager->Render();
		}
	}
	SAFE_DELETE(scenemanager);
	DxSystem::Release();
	return 0;
}
