#pragma once

#define WIN32_EXTRA_LEAN
#define DIRECT_INPUT_vERSION 0x800
#include <Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>
#include<XInput.h>
#include<ctime>
#include<iostream>
#include<iomanip>
#include<d3dx9core.h>
#include<d3dx9math.h>

//#define KEY_DOWN (vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)

using namespace std;

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")


extern const string AppTittle;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;

extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;


bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
D3DXVECTOR2 GetBitmapSize(string filename);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int framh, int columns);
void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);

extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];
extern LPD3DXSPRITE spriteobj;


bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
//int KEY_DOWN(int vk_code);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amout = 65535);
bool XInput_Controller_Found();

bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();
