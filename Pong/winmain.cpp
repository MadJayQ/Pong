#include "pch.h"
#include "Pong.h"


namespace
{
	std::unique_ptr<Pong> g_Pong;
};

//Function Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall wWinMain (_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	g_Pong.reset(new Pong());

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = "Pong_Class";
	wcex.lpszMenuName = nullptr;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);

	if (!RegisterClassEx(&wcex))
	{
		return 0;
	}

	HWND hwnd = CreateWindow("Pong_Class", "Pong", WINSTYLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(g_Pong.get()));
	g_Pong->Initialize(hwnd);



	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //If any messages exist on the message queue process them
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //Otherwise let's perform a tick.
		{
			//Game loop tick
			g_Pong->Tick();
		}
	}

	g_Pong.reset();

	CoUninitialize();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_minized = false;

	auto game = reinterpret_cast<Pong*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (msg == WM_PAINT)
	{
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	}
	if (msg == WM_ACTIVATEAPP)
	{
		if (game)
		{
			if (wParam)
			{
				game->OnActivate();
			}
			else
			{
				game->OnDeactivate();
			}
		}
	}
	if (msg == WM_KEYDOWN && msg != WM_PAINT)
	{
		g_Pong->keys[wParam] = true;
	}
	if (msg == WM_KEYUP && msg != WM_PAINT)
	{
		g_Pong->keys[wParam] = false;
	}
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}