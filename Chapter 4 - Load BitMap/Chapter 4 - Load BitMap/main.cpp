#include <Windows.h>
#include <iostream>
#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib");
#pragma comment(lib,"d3dx9.lib");
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
const int SCREENW = 1920;
const int SCREENH = 1080;



using namespace std;

const string windowstitle = "Chapter 4 - Drawing Bitmaps";




LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;

bool gameover = false;



bool Game_Init(HWND hwnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (d3d == NULL) {
		MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);

		return false;

	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow = hwnd;


	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	if (!d3ddev) {
		MessageBox(hwnd, "Error Creating Direct3D device", "Error", MB_OK);

		return false;
	}

	srand(time(NULL));

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	HRESULT result = d3ddev->CreateOffscreenPlainSurface(
		SCREENH,
		SCREENW,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&surface,
		NULL);



	if (!SUCCEEDED(result)) return false;

	result = D3DXLoadSurfaceFromFile(
		surface,
		NULL,
		NULL,
		"porno.jpg",
		NULL,
		D3DX_DEFAULT,
		0,
		NULL);

	if (!SUCCEEDED(result)) return false;

	return true;
}

void Game_Run(HWND hwnd) {

	if (!d3ddev) return;

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	if (d3ddev->BeginScene()) {

		RECT rect;

		rect.left = SCREENW / 2;
		rect.right = rect.left + SCREENW / 2;
		rect.top = SCREENH / 2;
		rect.bottom = rect.top + SCREENH / 2;

		d3ddev->StretchRect(surface, NULL, backbuffer, &rect, D3DTEXF_NONE);

		d3ddev->EndScene();

		d3ddev->Present(NULL, NULL, NULL, NULL);

	}


	if (KEY_DOWN(VK_ESCAPE)) {
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	}


}

void Game_End(HWND hwnd) {
	if (surface) surface->Release();
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
/*
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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowstitle.c_str();
	wc.hIconSm = NULL;

	return RegisterClassEx(&wc);
}
*/
/*
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	windows = CreateWindow(
		windowstitle.c_str(),
		windowstitle.c_str(),
		WS_OVERLAPPEDWINDOW,
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
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MSG msg;

	//if (!MyRegisterClass) return 0;
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowstitle.c_str();
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);
	//if (!InitInstance(hInstance, nCmdShow)) return 0;
	HWND windows = CreateWindow(
		windowstitle.c_str(),
		windowstitle.c_str(),
		WS_OVERLAPPEDWINDOW,
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

	if (!Game_Init(windows)) return 0;


	while (!gameover) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run(windows);
	}
	return msg.wParam;
}

