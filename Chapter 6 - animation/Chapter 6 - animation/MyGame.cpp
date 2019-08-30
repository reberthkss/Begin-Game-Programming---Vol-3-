#include "DirectX.h"
#include <iostream>
//#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code)&0x8000)?1:0)
using namespace std;
const string AppTittle = "TRANSPARENTE";
const int SCREENW = 800;
const int SCREENH = 600;

LPDIRECT3DTEXTURE9 cat = NULL;
int frame = 12;
int starttime = 0;


bool Game_Init(HWND hwnd) {
	//MessageBox(0, "Tentou iniciar o jogo", "BREAKPOINT", 0);
	if (!Direct3D_Init(hwnd, SCREENW, SCREENH, false)) {
		MessageBox(0, "Erro ao iniciar o direct3d", "Erro", 0);
		return false;
	}

	//MessageBox(0, "INICIOU O DIRECT3D", "BREAKPOINT", 0);

	//MessageBox(0, "Tentou DEFINIR O OBJETO", "BREAKPOINT", 0);
	cat = LoadTexture("catspritesx4.png");
	//MessageBox(0, "Iniciou o objeto", "BREAKPOINT", 0);
	if (!cat) return false;


	//MessageBox(0, "Jogo iniciado com sucesso", "BREAKPOINT", 0);
	return true;
}

void Game_Run(HWND hwnd) {
	if (!d3ddev) return;

	//MessageBox(0, "COMEÇO DO GAME_RUN", "BREAKPOINT", 0);


	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//MessageBox(0, "Limpou o dispositivo", "BREAKPOINT", 0);
	


	if (d3ddev->BeginScene()) {

		//MessageBox(0, "Inicio da cena", "BREAKPOINT", 0);
		
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//MessageBox(0, "Inicio do sprite begin", "BREAKPOINT", 0);
		Sprite_Animate(frame, 12, 17, 1, starttime, 200);
		Sprite_Draw_Frame(cat, 200, 200, frame, 76, 65, 6);
		
		spriteobj->End();
	

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//if(KEY_DOWN(VK_ESCAPE)) gameover = true;


	}

void Game_End() {

	
	 cat->Release();


	Direct3D_Shutdown();
}



