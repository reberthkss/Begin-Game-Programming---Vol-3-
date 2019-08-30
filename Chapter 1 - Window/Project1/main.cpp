#include <windows.h>
#include <iostream>

using namespace std;


const string ProgramTitle = "Titulo da Janela";

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	string text = "Hello Windows!";

	switch (message) {
	case WM_PAINT: {
		RECT rt;
		GetClientRect(hWnd, &rt);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawText(hdc, text.c_str(), text.length(), &rt, DT_CENTER);
		for (int n = 0; n < 3000; n++){
		int x = rand() % (rt.right - rt.left);
		int y = rand() % (rt.bottom - rt.top);
		COLORREF c = RGB(rand() % 256, rand() % 256, rand() % 256);
		SetPixel(hdc, x, y, c);
}
EndPaint(hWnd, &ps);
	}
				   break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ProgramTitle.c_str();
	wc.hIconSm = NULL;
	return RegisterClassEx(&wc);
}
bool InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd = CreateWindow(
		ProgramTitle.c_str(),
		ProgramTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800,600,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hWnd == 0) return 0;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}