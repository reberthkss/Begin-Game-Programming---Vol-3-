#include <Windows.h>
#include <d3d9.h>
#include <time.h>
#include <iostream>

using namespace std;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

const string windowstitle = "Chapter 3 - Direct3D9";
const int SCREENW = 1920;
const int SCREENH = 1080;
HWND windows;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev= NULL;


bool gameover = false;

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)?1:0)


bool Game_Init(HWND window) {
	MessageBox(window, "Game_Init", "BREAKPOINT", MB_OK|MB_ICONINFORMATION);

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		MessageBox(window, "Error initializing Direct3D", "Error", MB_OK);
		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow = window;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	if (d3ddev == NULL) {
		MessageBox(window, "error creating direct3d device", "error", MB_OK);
		return 0;
	}

	return true;

}

void Game_Run(HWND hwnd) {

	if (!d3ddev) return;

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);

	if (d3ddev->BeginScene()) {
		d3ddev->EndScene();

		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}

void Game_End(HWND hwnd) {
	MessageBox(hwnd, "Program is about to end", "Game_End", MB_OK);

	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();

}


LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowstitle.c_str();
	wc.hIconSm = NULL;


	return RegisterClassEx(&wc);

}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	windows = CreateWindow(
		windowstitle.c_str(),
		windowstitle.c_str(),
		WS_EX_TOPMOST|WS_VISIBLE|WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREENW,
		SCREENH,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (windows == 0) return 0;

	ShowWindow(windows, nCmdShow);
	UpdateWindow(windows);

	return 1;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdlIne, int nCmdShow) {
	MSG msg;


	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) return 0;

	if (!Game_Init(windows)) return 0;

	while (!gameover) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run(windows);
	}

	Game_End(windows);

	return msg.wParam;
}