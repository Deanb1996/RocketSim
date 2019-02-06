#include <windows.h>
#include "Scene.h"
#include "DXRender.h"
#include <chrono>

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

/// <summary>
/// Entry point to the program. Initializes everything and goes into a message processing loop.
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int WINAPI wWinMain(_In_ const HINSTANCE hInstance, _In_opt_ const HINSTANCE hPrevInstance, _In_ const LPWSTR lpCmdLine, _In_ const int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	//Create instance of AntTweakManager
	AntTweakManager antTweakManager;

	//Gives renderer pointer to window
	DXRender renderer;
	renderer.SetWindow(g_hWnd);

	//Initisialises DirectX device, and cleans up device if there's a failure
	if (FAILED(renderer.InitDXDevice(antTweakManager)))
	{
		renderer.CleanUpDevice();
		return 0;
	}

	//Initialises the scene with all required entities, cameras and lights
	Scene scene(renderer.Width(), renderer.Height(), antTweakManager);
	scene.InitScene();

	//Main message loop
	MSG msg = { 0 };
	auto previousTime = std::chrono::high_resolution_clock::now();
	auto startTime = std::chrono::high_resolution_clock::now();
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			//Calculates delta time
			auto currentTime = std::chrono::high_resolution_clock::now();
			const auto elapsedTimeTotal = currentTime - startTime;
			const auto elapsedTime = currentTime - previousTime;
			const float dt = (elapsedTime.count() / 1e+9);
			const float time = (elapsedTimeTotal.count() / 1e+9);

			//Update Render Loop
			scene.Update(dt);
			if (scene.ExitApplication())
			{
				renderer.CleanUpDevice();
				return 0;
			}
			if (FAILED(renderer.Render(scene.Entities(), scene.ActiveCamera(), time)))
			{
				renderer.CleanUpDevice();
				return 0;
			}
			previousTime = currentTime;
		}
	}
	
	//Cleanup device on exit
	renderer.CleanUpDevice();

	return static_cast<int>(msg.wParam);
}

/// <summary>
/// Register class and create window
/// </summary>
/// <param name="hInstance"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
HRESULT InitWindow(const HINSTANCE hInstance, const int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, nullptr);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"ACW";
	wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);
	if (!RegisterClassEx(&wcex))
		return static_cast<HRESULT>(0x80004005L);

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"ACW", L"ACW",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd)
		return static_cast<HRESULT>(0x80004005L);

	ShowWindow(g_hWnd, nCmdShow);

	return static_cast<HRESULT>(0L);
}

/// <summary>
/// Called every time the application receives a message
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	//Tells anttweak to handle events relevant to anttweak
	if (TwEventWin(hWnd, message, wParam, lParam))
	{
		return 0;
	}

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//DirectXTK input handling
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}