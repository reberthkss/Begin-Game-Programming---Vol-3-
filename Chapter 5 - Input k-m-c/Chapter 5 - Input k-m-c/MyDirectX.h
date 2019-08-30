#pragma once

#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include<Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <XInput.h>
#include<ctime>
#include<iostream>
#include<iomanip>

using namespace std;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")


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

extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];


bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amount = 65535);
bool XInput_Controller_Found();
bool Game_Init(HWND hwnd);
void Game_Run(HWND hwnd);
void Game_End();

