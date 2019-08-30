#include "DirectX.h"
#include <iostream>
//#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)
using namespace std;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPD3DXSPRITE spriteobj = NULL;

LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyboard;
char keys[256];
XINPUT_GAMEPAD controllers[4];




// SPRITES
// ---- Carregar Textura
// ------- Colocar no Game_Init para iniciar os objetos
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor) {
	LPDIRECT3DTEXTURE9 texture = NULL;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK) return NULL;

	D3DXCreateTextureFromFileEx(
		d3ddev,
		filename.c_str(),
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor,
		&info,
		NULL,
		&texture);

	if (result != D3D_OK) return NULL;

	return texture;
}

//SPRITES 
// ---- Obter as dimensoes da imagem
// ------Colocar no Game_Init

D3DXVECTOR2 GetBitmapSize(string filename)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result == D3D_OK) {
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	}
	else {
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	}

	return size;
}

//SPRITES
// ---- Desenhar sprites na tela
// ------ Colocar no d3ddev->BeginScene() do game_run
// -------- É importante definir a posição que o objeto vai ficar. exemplo pos1 (10, 20 ,30)
/*
HRESULT Draw(LPDIRECT3DTEXTURE9* <variavel do objeto sprite>,
	CONST RECT* <variavel que vai definir o tamanho do objeto>,
	CONST D3DXECTOR3* <variavel que vai definir se o objeto vai estar no centro>,
	CONST D3DXVECTOR3* <variavel que vai definir a posição do objeto>,
	D3DCOLOR <Variavel que vai definir a cor de fundo do objeto>);
	*/

//DIRECT3D
// ----- Iniciar o DIRECT3D
// ------- Colocar no Game_Init
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d) return false;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	if (!d3ddev) return false;

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	 D3DXCreateSprite(d3ddev, &spriteobj);

	return 1;
}

//DIRECT3D
// ----  Finalizar o DIRECT3D
// ------ Usar no Game_End
void Direct3D_Shutdown() {
if (spriteobj) spriteobj->Release();
	// Liberar memoria da superficie -->if(surface) surface->Release();
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

//SURFACE
// ------ Função que imprime a imagem na tela
// --------- Usar no d3ddev->BeginScene do do Game_run depois que definir todas as propriedades do objeto
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source) {
	D3DSURFACE_DESC desc;

	source->GetDesc(&desc);

	RECT source_rect = {
		0,
		0,
	(long)desc.Width,
	(long)desc.Height };

	RECT dest_rect = {
		(long)x,
		(long)y,
		(long)x + desc.Width,
		(long)y + desc.Height };

	d3ddev->StretchRect(source,//Manda
		&source_rect,//nova posição
		dest,//Recebe
		&dest_rect,//Antiga posição
		D3DTEXF_NONE);
}

//SURFACE
// -------- Função que carrega uma imagem que será demonstrada na superficie
// ---------- Usar no Game_init para carregar o objeto a ser manipulado
LPDIRECT3DSURFACE9 LoadSurface(string filename) {
	LPDIRECT3DSURFACE9 image = NULL;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(),&info);

	if (result != D3D_OK) return NULL;

	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);

	if (result != D3D_OK) return NULL;


	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		filename.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		NULL);


	if (result != D3D_OK) return NULL;

	return image;
}

//SPRITE
// ---- Usar para animar uma sprite
// ------

void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns) {
	D3DXVECTOR3 position((float)destx, (float)desty, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;

	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	spriteobj->Draw(texture, &rect, NULL, &position, white);
}

//SPRITE 
// ---- Usar para animar uma sprite
// ------COMO USAR::
/*

	TEM QUE USAR O CONJUNTO SPRITE_ANIMATE+SPRITE_DRAW_FRAME.
	----- A ORDEM É: SPRITE_ANIAMTE e depois SPRITE_DRAW_FRAME
	--------LOGICA: DESLIZAR O SPRITE(SPRITE_ANIMATE) ATÉ O PROXIMO SPRITE (SPRITE_DRAW_FRAME)
	---------QUANDO HOUVER UMA MATRIZ DE SPRITES E QUISER CARREGAR A SEGUNDA LINHA DE SPRITES:
	Sprite_Animate(<número do frame em que começa a segunda linha ([1][0])>,<número do frame em que começa a segunda linha ([1][0])>,<direção>,<contador>,<velocidade da animação>);
	Sprite_Draw_Frame(<sprite que sera animado>,<posicao da tela em que sera impresso - largura>,<largura da tela em que será impresso - altura>,<numero do frame  em que começa a segunda linha([1][0])>,<largura do frame>,<altura do frame>,<numero de colunas>);

*/






void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay) {
	if ((int)GetTickCount() > starttime + delay) {
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) frame = startframe;
		if (frame < startframe) frame = endframe;
	}
}


/*
//CONTROLES
// ------ Função que vai iniciar o DIRECTINPUT (teclado,mouse e controle)
// ----------- Colocar no Game_Init 
bool DirectInput_Init(HWND hwnd) {
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**) &dinput,
		NULL);

	dinput->CreateDevice(
		GUID_SysKeyboard,
		&dikeyboard,
		NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();

	dinput->CreateDevice(
		GUID_SysMouse,
		&dimouse,
		NULL);
	dimouse->SetDataFormat(&c_dfDIMouse);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	dimouse->Acquire();

	d3ddev->ShowCursor(false);

	return true;
}



//CONTROLES 
// -------- Função que vai verificar se uma nova tecla foi pressionada  (tanto para teclado,mouse ou controle)
// ------------ Colocar no começo do Game_Run
void DirectInput_Update() {
	dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID) &mouse_state);
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID) &keys);

	for (int i = 0; i < 4; i++) {
		ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));

		XINPUT_STATE state;

		DWORD result = XInputGetState(i, &state);

		if (result == 0) controllers[i] = state.Gamepad;
	}
}

//CONTROLES
// -------- Função  dos mouses

int Mouse_X() {
	return mouse_state.lX;
}

int Mouse_Y() {
	return mouse_state.lY;
}

int Mouse_Button(int button) {
	return mouse_state.rgbButtons[button] & 0x80;
}

//CONTROLES
// -------- FUNÇÃO DO TECLADO
int Key_Down(int key) {
	return (keys[key] & 0x80);
}

//CONTROLES
// -------- Função que desliga os dispositivos(teclado,mouse e controle)
void DirectInput_Shutdown() {
	if (dikeyboard) {
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
	if (dimouse) {
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}


//CONTROLES
// --------- Função que ve se tem um controle conectado
bool XInput_Controller_Found() {
	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
	if (caps.Type != 0) return false;

	return true;
}

//CONTROLES
// --------- FUNÇÃO QUE FAZ O CONTROLE VIBRAR 

void XInput_Vibrate(int contNum, int amount) {
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = amount;
	vibration.wRightMotorSpeed = amount;
	XInputSetState(contNum, &vibration);
}


*/




