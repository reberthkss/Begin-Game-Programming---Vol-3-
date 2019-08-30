#include "DirectX.h"
using namespace std;
const string AppTittle = "TRANSPARENTE";
const int SCREENW = 1100;
const int SCREENH = 736;

LPDIRECT3DTEXTURE9 image_colorkey = NULL;
LPDIRECT3DTEXTURE9 image_alpha = NULL;
LPDIRECT3DTEXTURE9 image_notrans = NULL;

bool Game_Init(HWND hwnd) {
	if (!Direct3D_Init(hwnd, SCREENW, SCREENH, false)) {
		MessageBox(0, "Erro ao iniciar o direct3d", "Erro", 0);
		return false;
	}

	image_notrans = LoadTexture("foguete.jpg");
	if (!image_notrans) return false;
	image_colorkey = LoadTexture("foguete.bmp",D3DCOLOR_XRGB(255,0,255));
	if (!image_colorkey) return false;
	image_alpha = LoadTexture("foguete.tga");
	if (!image_alpha) return false;

	return true;
}

void Game_Run(HWND hwnd) {
	if (!d3ddev) return;

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	if (d3ddev->BeginScene()) {
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXVECTOR3 pos1(10, 10, 0);
		spriteobj->Draw(image_notrans, NULL, NULL, &pos1, D3DCOLOR_XRGB(255, 255, 255));

		D3DXVECTOR3 pos2(350, 10, 0);
		spriteobj->Draw(image_colorkey, NULL, NULL, &pos2, D3DCOLOR_XRGB(255, 255, 255));

		D3DXVECTOR3 pos3(700, 10, 0);
		spriteobj->Draw(image_alpha, NULL, NULL, &pos3, D3DCOLOR_XRGB(255, 255, 255));

		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	if(KEY_DOWN(VK_ESCAPE)) gameover = true;


	}

void Game_End() {
	image_notrans->Release();
	image_colorkey->Release();
	image_alpha->Release();
	Direct3D_Shutdown();
}



