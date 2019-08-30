#include "DirectX.h"

using namespace std;
const string AppTittle = "TRANSPARENTE";
const int SCREENW = 1920;
const int SCREENH = 1080;

SPRITE ship, asteroid1, asteroid2;
LPDIRECT3DTEXTURE9 imgShip = NULL;
LPDIRECT3DTEXTURE9 imgAsteroid=NULL;
LPD3DXSPRITE spriteobj = NULL;
D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
float r = 0.0f;


bool Game_Init(HWND hwnd) {

	//MessageBox(0, "Tentou iniciar o DIRECT3D", "BREAKPOINT", 0);
	Direct3D_Init(hwnd, SCREENW, SCREENH, false);
	
	//MessageBox(0, "Iniciou o DIRECT3D", "BREAKPOINT", 0);

	//MessageBox(0, "Tentou iniciar o DIRECTINPUT", "BREAKPOINT", 0);

	DirectInput_Init(hwnd);
	//MessageBox(0, "Iniciou o DIRECTINPUT", "BREAKPOINT", 0);


		imgAsteroid = LoadTexture("asteroid.png");
		imgShip = LoadTexture("spaceship2.png");

		ship.x = SCREENH/1.2;
		ship.y = SCREENW/5;
		ship.width = ship.height = 91;

		asteroid1.x = 50;
		asteroid1.y = 200;
		asteroid1.width = asteroid1.height = 160;
		asteroid1.columns = 1;
		asteroid1.startframe = 0;
		asteroid1.endframe = 1;
		asteroid1.velx = -0.1f;
		asteroid1.vely = +0.1f;
		asteroid1.starttime = 0;
		asteroid1.delay = 0;

		asteroid2.x = 900;
		asteroid2.y = 200;
		asteroid2.width = asteroid2.height = 160;
		asteroid2.columns = 1;
		asteroid2.startframe = 0;
		asteroid2.endframe = 1;
		asteroid2.velx = +0.1f;
		asteroid2.vely = -0.08f;
		asteroid2.starttime = 0;
		asteroid2.delay = 0;


		return true;
	}


void Game_Run(HWND hwnd){
	if (!d3ddev) return;

	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	

	if (Key_Down(DIK_UP)) {
		ship.y -= 0.3f;
		if (ship.y < 0) ship.y = 0;
	}

	if (Key_Down(DIK_DOWN)) {
		ship.y += 0.3f;
		if (ship.y > SCREENH - ship.height) {
			ship.y = SCREENH - ship.height;
		}
	}
	
	if (Key_Down(DIK_RIGHT)) {
		ship.x += 0.3f;
		if (ship.x > SCREENW - ship.width) {
			ship.x = SCREENW - ship.width;
		}
	}

	if (Key_Down(DIK_LEFT)) {
		ship.x -= 0.3f;
		if (ship.x < 0) ship.x = 0;
	}

	asteroid1.x += asteroid1.velx;
	asteroid1.y += asteroid1.vely;

	if (asteroid1.x<0 || asteroid1.x>SCREENW - asteroid1.width) {
		asteroid1.velx *= -1;
	}

	if (asteroid1.y<0 || asteroid1.y>SCREENH - asteroid1.height) {
		asteroid1.vely *= -1;
	}

	Sprite_Animate(asteroid1.frame, asteroid1.startframe, asteroid1.endframe, asteroid1.direction, asteroid1.starttime, asteroid1.delay);

	asteroid2.x += asteroid2.velx;
	asteroid2.y += asteroid2.vely;

	if (asteroid2.x<0 || asteroid2.x>SCREENW - asteroid2.width) {
		asteroid2.velx *= -1;
	}
	if (asteroid2.y<0 || asteroid2.y>SCREENH - asteroid2.height) {
		asteroid2.vely *= -1;
	}


	Sprite_Animate(asteroid2.frame, asteroid2.startframe, asteroid2.endframe, asteroid2.direction, asteroid2.starttime, asteroid2.delay);

	if (CollisionD(ship, asteroid1)) {
		asteroid1.velx *= -1;
	}



	if (CollisionD(ship, asteroid2)) {
		asteroid2.velx *= -1;
	}

	if (CollisionD(asteroid1, asteroid2)) {
		asteroid1.velx *= -1;
		asteroid2.velx *= -1;
	}



	if (d3ddev->BeginScene()) {
		
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		r = timeGetTime() / 600.0f;
	
		Sprite_Transform_Draw(imgShip, ship.x, ship.y, ship.width, ship.height, ship.startframe, ship.columns);
		Sprite_Transform_Draw(imgAsteroid, asteroid1.x, asteroid1.y, asteroid1.width, asteroid1.height, asteroid1.startframe, asteroid1.columns,r);
		Sprite_Transform_Draw(imgAsteroid, asteroid2.x, asteroid2.y, asteroid2.width, asteroid2.height, asteroid2.startframe, asteroid2.columns,r);

		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}


	if(Key_Down(VK_ESCAPE)) gameover = true;

	


	}

void Game_End() {

	if (imgShip) imgShip->Release();
	if (imgAsteroid) imgAsteroid->Release();
	Direct3D_Shutdown();
}



