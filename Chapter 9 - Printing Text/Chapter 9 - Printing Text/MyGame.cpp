#include "DirectX.h"
#include <sstream>

using namespace std;
const string AppTittle = "TRANSPARENTE";
const int SCREENW = 1100;
const int SCREENH = 736;

LPD3DXFONT fontArial24 = NULL;
LPD3DXFONT fontGaramond36 = NULL;
LPD3DXFONT fontTimesNewRoman40 = NULL;
LPD3DXFONT font;
LPDIRECT3DTEXTURE9 sprite = NULL;
LPDIRECT3DSURFACE9 gameworld = NULL;
LPD3DXSPRITE spriteobj = NULL;
LPDIRECT3DSURFACE9 scrollbuffer = NULL;
LPDIRECT3DSURFACE9 tiles = NULL;

D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

const int TILEWIDTH = 64;
const int TILEHEIGHT = 64;
const int MAPWIDTH = 16;
const int MAPHEIGHT = 24;


const int WINDOWWIDTH = (SCREENW / TILEWIDTH) * TILEWIDTH;
const int WINDOWHEIGHT = (SCREENH / TILEHEIGHT) * TILEHEIGHT;

/*
const int GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
const int GAMEWORLDHEIGHT = TILEHEIGHT * MAPHEIGHT;
*/
int ScrollX, ScrollY;
int SpeedX, SpeedY;
long start;


int MAPDATA[MAPWIDTH * MAPHEIGHT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
,
17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
,
33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48
,
49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64
,
65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80
,
81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96
,
97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112
,
113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128
,
129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144
,
145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160
,
161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176
,
177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192
,
193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208
,
209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224
,
225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240
,
241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256
,
257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272
,
273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288
,
289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304
,
305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320
,
321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336
,
337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352
,
353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368
,
369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384
};


void DrawTile(LPDIRECT3DSURFACE9 source,
	int tilenum,
	int width,
	int height,
	int columns,
	LPDIRECT3DSURFACE9 dest,
	int destx,
	int desty) {
	RECT r1;
	r1.left = (tilenum % columns) * width;
	r1.top = (tilenum / columns) * height;
	r1.right = r1.left + width;
	r1.bottom = r1.top + height;

	RECT r2 = { destx,desty,destx + width,desty + height };

	d3ddev->StretchRect(source, &r1, dest, &r2, D3DTEXF_NONE);
}

void DrawTiles() {
	int tilex, tiley;
	int columns, rows;
	int x, y;
	int tilenum;

	tilex = ScrollX / TILEWIDTH;
	tiley = ScrollY / TILEHEIGHT;

	columns = WINDOWWIDTH / TILEWIDTH;
	rows = WINDOWHEIGHT / TILEHEIGHT;

	for (y = 0; y <=rows ; y++) {
		for ( x= 0; x < columns; x++) {
			tilenum = MAPDATA[((tiley + y) * MAPWIDTH + (tilex + x))];
			DrawTile(tiles, tilenum, TILEWIDTH, TILEHEIGHT, 16, scrollbuffer, x * TILEWIDTH, y * TILEHEIGHT);
		}
	}
	
}

void DrawScrollWindow(bool scaled = false) {

	int partialx = ScrollX % TILEWIDTH;
	int partialy = ScrollY % TILEHEIGHT;

	RECT r1 = { partialx,partialy,partialx + WINDOWWIDTH - 1,partialy + WINDOWHEIGHT - 1 };

	RECT r2;

	if (scaled) {
		RECT r = { 0,0,WINDOWWIDTH - 1,WINDOWHEIGHT - 1 };
		r2 = r;
	}
	else {
		RECT r = { 0,0,SCREENW - 1,SCREENH - 1 };
		r2 = r;
	}

	d3ddev->StretchRect(scrollbuffer,&r1, backbuffer, &r2, D3DTEXF_NONE);
}
/*
void BuildGameWorld() {
	HRESULT result;
	int x, y;
	LPDIRECT3DSURFACE9 tiles;
	tiles = LoadSurface("dasdada.bmp");

	result = d3ddev->CreateOffscreenPlainSurface(
		GAMEWORLDWIDTH,
		GAMEWORLDHEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&gameworld,
		NULL);
		
	if (result != S_OK) {
		MessageBox(NULL, "Error creating surface", "ERROR", 0);
		return;
	}

	for (y = 0; y < MAPHEIGHT; y++) {
		for (x = 0; x < MAPWIDTH; x++) {
			DrawTile(tiles, MAPDATA[y * MAPWIDTH + x], 64, 64, 16, gameworld, x * 64, y * 64);
		}
	}

	tiles->Release();

}
*/


bool Game_Init(HWND hwnd) {


	if (!Direct3D_Init(hwnd, SCREENW, SCREENH, false)) {
		MessageBox(0, "Erro ao iniciar o direct3d", "Erro", 0);
		return false;
	}


	if (!DirectInput_Init(hwnd)) {
		MessageBox(0, "Erro ao iniciar os controles", "ERRO", 0);
	}
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	

	/*
	CARREGAR A IMAGEM PARA O OBJETO
	sprite = LoadTexture("<NOME DA IMAGEM AQUI>.<EXTENSAO DA IMAGEM AQUI>");

	surface = LoadSurface("<NOME DA IMAGEM AQUI>.<EXTENSAO DA IMAGEM AQUI>");
	*/
	fontArial24 = MakeFont("Arial", 24);
	tiles = LoadSurface("dasdada.bmp");
	//fontGaramond36 = MakeFont("Garamond", 36);
	//fontTimesNewRoman40 = MakeFont("Times New Roman", 40);

	const int SCROLLBUFFERWIDTH = SCREENW + TILEWIDTH * 2;
	const int SCROLLBUFFERHEIGHT = SCREENH + TILEHEIGHT * 2;

	HRESULT result = d3ddev->CreateOffscreenPlainSurface(SCROLLBUFFERWIDTH, SCROLLBUFFERHEIGHT, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &scrollbuffer, NULL);

	//BuildGameWorld();

	start = GetTickCount();


	return true;

}
/*
void ScrollScreen() {

	ScrollX += SpeedX;
	if (ScrollX < 0) {
		ScrollX = 0;
		SpeedX = 0;
	}
	else {
		if (ScrollX > GAMEWORLDWIDTH - SCREENW) {
			ScrollX = GAMEWORLDWIDTH - SCREENW;
			SpeedX = 0;
		}
	}

	ScrollY += SpeedY;
	if (ScrollY < 0) {
		ScrollY = 0;
		SpeedY = 0;
		}
	else {
		if (ScrollY > GAMEWORLDHEIGHT - SCREENH) {
			ScrollY = GAMEWORLDHEIGHT - SCREENH;
			SpeedY = 0;
			}
		}
	RECT r1 = { ScrollX,ScrollY,ScrollX + SCREENW - 1,ScrollY + SCREENH - 1 };
	RECT r2 = { 0,0,SCREENW - 1,SCREENH - 1 };

	d3ddev->StretchRect(gameworld, &r1, backbuffer, &r2, D3DTEXF_NONE);

	}
	*/

void UpdateScrollPosition() {
	const int GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
	const int GAMEWORDHEIGHT = TILEHEIGHT * MAPHEIGHT;

	ScrollX += SpeedX;
	if (ScrollX < 0) {
		ScrollX = 0;
		SpeedX = 0;
	}
	else {
		if (ScrollX > GAMEWORLDWIDTH - SCREENW) {
			ScrollX = GAMEWORLDWIDTH - SCREENW;
			SpeedX = 0;
		}
	}

	ScrollY += SpeedY;
	if (ScrollY < 0) {
		ScrollY = 0;
		SpeedY = 0;
	}
	else {
		if (ScrollY > GAMEWORDHEIGHT - SCREENH) {
			ScrollY = GAMEWORDHEIGHT - SCREENH;
			SpeedY = 0;
		}
	}

}


void Game_Run(HWND hwnd) {
	if (!d3ddev) return;
	DirectInput_Update();
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	if (Key_Down(DIK_UP)) {
		ScrollY -= 5;
	}

	if (Key_Down(DIK_DOWN)) {
		ScrollY += 5;
	}
	
	if (Key_Down(DIK_LEFT)) {
		ScrollX -= 5;
	}
	
	if (Key_Down(DIK_RIGHT)) {
		ScrollX += 5;
	}

	if (GetTickCount() - start >= 30) {
		start = GetTickCount();
		UpdateScrollPosition();


		if (d3ddev->BeginScene()) {
			//ScrollScreen();

			DrawTiles();
			DrawScrollWindow();

			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			ostringstream oss;
			oss << "Scroll Position = " << ScrollX << "," << ScrollY;
			FontPrint(fontArial24, 0,0,oss.str());
			//FontPrint(fontGaramond36, 60, 100, "ESSA É A FONTE GARAMOND 36", D3DCOLOR_XRGB(255, 0, 255));
			//FontPrint(fontTimesNewRoman40, 60, 200, "ESSA É A FONTE TIMES NEW ROMAN 40", D3DCOLOR_XRGB(0, 255, 0));

			//string text = "Mensaagem que vou printar";
			//text += "escrito na tela";
			//fontTimesNewRoman40->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_WORDBREAK, white);

			spriteobj->End();
			d3ddev->EndScene();
			d3ddev->Present(NULL, NULL, NULL, NULL);
		}
	}

	if(Key_Down(VK_ESCAPE)) gameover = true;


	}

void Game_End() {

	if (fontArial24) fontArial24->Release();
	//if (fontGaramond36) fontGaramond36->Release();
	//if (fontTimesNewRoman40) fontTimesNewRoman40->Release();
	if (scrollbuffer) scrollbuffer->Release();
	if (tiles) tiles->Release();
	/*
	Liberação da memoria do objeto criado --> sprite->Release();
	Liberação da memoria do objeto criado --> surface->Release();
	*/
	Direct3D_Shutdown();
}



