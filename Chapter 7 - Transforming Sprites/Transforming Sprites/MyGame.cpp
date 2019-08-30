#include "DirectX.h"
using namespace std;
const string AppTittle = "TRANSPARENTE";
const int SCREENW = 800;
const int SCREENH = 600;

LPDIRECT3DTEXTURE9 girassol = NULL;
LPDIRECT3DTEXTURE9 terra = NULL;
D3DCOLOR color=D3DCOLOR_XRGB(255,255,255);
int frame = 0, columns, width, height;
int startframe, endframe, starttime = 0, delay;
float scale = 0.000f;
float r = 0.0f;
float s = 1.0f;






bool Game_Init(HWND hwnd) {

	if (!Direct3D_Init(hwnd, SCREENW, SCREENH, false)) {
		MessageBox(0, "Erro ao iniciar o direct3d", "Erro", 0);
		return false;
	}
	/*
	INICIAR OS CONTROLES

	if (!DirectInput_Init) return {
		MessageBox(0,"Erro ao iniciar os controles", "ERRO", 0);
		return false;
	}*/

	/* EXEMPLO GIRASSOL
	girassol = LoadTexture("girassol.png",color);
	*/
	/* EXEMPLO TERRA*/
	terra = LoadTexture("terra.png");

	return true;
}

void Game_Run(HWND hwnd) {

	if (!d3ddev) return;

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	if (d3ddev->BeginScene()) {

		//MessageBox(0, "Começou a desenha", "BREAKPOINT", 0);
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	/*

	EXEMPLO DO GIRASSOL
		r = timeGetTime() / 600.0f;
		s += scale;
		if (s < 0.0001 || s>1.25f) scale *= -1;

		
		width = 360;
		height = 360;

		frame = 0;
		columns = 1;
		color = D3DCOLOR_XRGB(255, 255, 255);
		Sprite_Transform_Draw(girassol, 300, 300, width, height, frame, columns, r, s, color);
		*/
		/* 
		EXEMPLO DA TERRA */
		//s += scale;

		//if (s < 0.5f || s>6.0f) scale *= -1;

		columns = 16;
		width = height = 65;
		startframe = 0;
		endframe = 256;
		delay = 10;

		//MessageBox(0, "Tentou animar", "BREAKPOINT", 0);
		Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);
		//MessageBox(0, "Conseguiu Animar", "Breakpoint", 0);
		Sprite_Transform_Draw(terra, 200, 200, width, height, frame, columns, 0,s, color);
		//MessageBox(0, "Conseguiu transformar", "BREAKPOINT", 0);
		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}


	}

void Game_End() {


// exemplo do girassol--> girassol->Release();
	terra->Release();
	
	Direct3D_Shutdown();
}



