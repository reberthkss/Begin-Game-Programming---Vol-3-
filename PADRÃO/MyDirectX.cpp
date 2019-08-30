#include "DirectX.h"
#include <iostream>

using namespace std;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;


LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyboard;
DIMOUSESTATE mouse_state
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
// ---------- JÁ ESTÁ PRESENTE NAS BIBLIOTECAS
/*

HRESULT Draw(LPDIRECT3DTEXTURE9* <variavel do objeto sprite>,
	CONST RECT* <variavel que vai definir o tamanho do objeto>,
	CONST D3DXECTOR3* <variavel que vai definir se o objeto vai estar no centro>,
	CONST D3DXVECTOR3* <variavel que vai definir a posição do objeto>,
	D3DCOLOR <Variavel que vai definir a cor de fundo do objeto>);

// -----------------<VARIAVEL QUE VAI DEFINIR SE O OBJETO VAI ESTAR NO CENTRO>
FUNÇÃO: center (define um pixel do objeto)

D3DXVECTOR2 center ((width*scaling)/180,(height*scaling)/180);

// -----------------<VARIAVEL QUE VAI DEFINIR O ANGULO DE ROTAÇÃO DO SPRITE(RADIANO)>
variavel rotation (recebe um angulo)

double toRadians(degrees){
		------ grau -> radiano
		return &rotation=degress*PI_over_180;
		}

// -----------------<VARIAVEL QUE VAI DEFINIR O ANGULO DE ROTAÇÃO DO SPRITE(GRAUS)>
variavel rotation (recebe um angulo)

double toDegress(radian){
		------ grau <- radiano
		return &rotation=degress*PI_under_180;
		}
		------- VARIAVEL PI_over_180
VARIAVEL: PI_over_180 (considerar angulos maiores que a metade da circunferencia)

		const double PI_over_180=PI/180.0f;
		------- VARIAVEL under
VARIAVEL: PI_under_180 (considerar angulos menores que a metade da circunferencia)

		const double PI_under_180=180/PI;



	*/
/*
//SPRITES
// ----
// ------ FUNÇÃO JÁ PRESENTE NA BIBLIOTECA DO D3DX9

FUNÇÃO: D3DXMatrixTransform2D(&mat,NULL,0,&scale,&center,rotation,&trans);


*/

//SPRITE
// ----
// -----

void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, int frame, int columns, float rotation, float scaling, D3DCOLOR color) {
	D3DXVECTOR2 scale(scaling, scaling);

	D3DXVECTOR2 trans(x, y);

	D3DXVECTOR2 center((float)(width * scaling) / 2, (float)(height * scaling) / 2);

	D3DXMATRIX mat;

	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, rotation, &trans);

	spriteobj->SetTransform(&mat);

	int fx = (frame % columns) * width;
	int fy = (frame / columns) * height;

	RECT scrRect = { fx,fy,fx + width,fy + height };

	spriteobj->Draw(image, &scrRect, NULL, NULL, color);
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
// ------

void Sprite_Animate(int& frame, int startframe, int endframe, int direction, int& starttime, int delay) {
	if ((int)GetTickCount() > starttime + delay) {
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) frame = startframe;
		if (frame < startframe) frame = endframe;
	}
}

//SPRITE
// ----
// ------
// -------- JÁ ESTÁ PRESENTE NA BIBLIOTECA DO D3DX9
/*
D3DXMatrixTransformation2D(
&mat,
NULL,
0,
&scale,
&center,
rotation,
&trans);
*/


//SPRITE
// ----
// ------ 

int Collision(SPRITE sprite1, SPRITE sprite2) {
	RECT rect1;
	rect1.left = (long)sprite1.x;
	rect1.top = (long)sprite1.y;
	rect.right = (long)sprite1.x + sprite1.width * sprite1.scaling;
	rect1.bottom = (long)sprite1.y + sprite1.height * sprite1.scaling;

	RECT rect2;

	rect2.left = (long)sprite2.x;
	rect2.top = (long)sprite2.y;
	rect2.right = (long)sprite2.x + sprite2.width * sprite2.scaling;
	rect2.bottom = (long)sprite2.y + sprite2.height * sprite2.scaling;

	RECT dest;

	return IntersectRect(&dest, &rect1, &rect2);

}
// SPRITE 
// ----
// ------ 

bool CollisionD(SPRITE sprite1, SPRITE sprite2) {
	double radius1, radius2;

	if (sprite1.width > sprite1.height) {
		radius1 = (sprite1.width * sprite1.scaling) / 2;
	}
	else {
		radius1 = (sprite1.height * sprite1.scaling) / 2;
	}

	double x1 = sprite1.x + radius1;
	double y1 = sprite1.y + radius1;
	D3DXVECTOR2 vector1(x1, y1);

	if (sprite2.width > sprite2.height) {
		radius2 = (sprite2.width * sprite2.scaling) / 2.0;
	}
	else {
		radius2 = (sprite2.height * sprite2.scaling) / 2.0;
	}

	double x2 = sprite2.x + radius2;
	double y2 = sprite2.y + radius2;
	D3DXVECTOR2 vector2(x2, y2);

	double deltax = vector1.x - vector2.x;
	double deltay = vector2.y - vector1.y;

	double dist = sqrt((deltax * deltax) + (deltay * deltay));


	return (dist < radius1 + radius2);
}

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
	// Liberar memoria do sprite --> if (spriteobj) spriteobj->Release();
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
	HRESULT result = D3DXGetImageInfoFromFile(
		filename.c_str(),
		&info);

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



//CONTROLES
// ------ Função que vai iniciar o DIRECTINPUT (teclado,mouse e controle)
// ----------- Colocar no Game_Init 
bool DirectInput_Init(HWND hwnd) {
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECT_INPUT_VERSION,
		IID_IDirectInput8,
		(void**)& dinput,
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
	dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)& mouse_state);
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)& keys);

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







// FONTES
// ----
// ------

LPD3DXFONT MakeFont(string name, int size) {
	LPD3DXFONT font = NULL; // cria afonte
	D3DXFONT_DESC desc = { // define as propriedades da fonte
		size,// tamanho/height
		0, // largura / width
		0, // peso /weight ????
		0, // mip leves ????
		false, // italico // itaic
		DEFAULT_CHARSET, // charset / ???
		OUT_TT_PRECIS, // output precision / ???
		CLIP_DEFAULT_PRECIS, // qualidade / quality
		DEFAULT_PITCH, //pitch and family / ??
	/*-> AQUIIIIIIII->>>>>>>> */"" // nome da fonte / font name};

	strcpy(desc.FaceName, name.c_str()); // copia o nome da fonte para a estrutura do desc, parametro "font name"

	D3DXCreateFontIndirect(d3ddev, &desc, &font); // FINALMENTE CRIA A FONTE

	return font;
}

void FontPrint(LPDXFONT font, int x, int y, string text, D3DCOLOR color) {
	RECT rect = { x,y,0,0 };
	font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color);

	font->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_LEFT, color);
}

