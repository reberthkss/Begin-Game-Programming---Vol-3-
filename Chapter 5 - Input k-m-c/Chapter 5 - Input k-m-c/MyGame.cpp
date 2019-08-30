#include "MyDirectX.h"


using namespace std;

const string AppTittle = "Bomb Catcher By: Dialogy - Reberth - UNIFESP SJC";
const int SCREENW = 1920;
const int SCREENH = 1080;

LPDIRECT3DSURFACE9 bomb_surf = NULL;
LPDIRECT3DSURFACE9 bucket_surf = NULL;

struct BOMB {
	float x, y;

	void reset() {
		x = (float)(rand() % (SCREENW - 128));
		y = 0;
	}
};

BOMB bomb;
BOMB bomb2;

struct BUCKET {
	float x, y;
};

BUCKET bucket;

int score = 0;
int vibrating = 0;


bool Game_Init(HWND hwnd) {
	Direct3D_Init(hwnd, SCREENW, SCREENH, false);
	DirectInput_Init(hwnd);
	bomb_surf = LoadSurface("bomb.png");
	if (!bomb_surf) {
		MessageBox(hwnd, "Error loading bomb", "Error", 0);

		return false;
	}

	bucket_surf = LoadSurface("bucket.png");
	if (!bucket_surf) {
		MessageBox(hwnd, "Error loading bucket", "Error", 0);
		return false;
	}

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	srand((unsigned int)time(NULL));
	bomb.reset();
	bomb2.reset();


	bucket.x = 500;
	bucket.y = 800;

	return true;
}

void Game_Run(HWND hwnd) {
	if (!d3ddev) return;

	DirectInput_Update();

	bomb.y += 2.0f;
	bomb2.y += 2.0f;


	if (bomb.y > SCREENH) {
		MessageBox(0, "A bomba explodiu!!!", "SE FUDEU", 0);
		gameover = true;
	}	if (bomb2.y > SCREENH) {
		MessageBox(0, "A bomba explodiu!!!", "SE FUDEU", 0);
		gameover = true;
	}

	int mx = Mouse_X();
	if (mx < 0) { bucket.x -= 7.0f; }
	else {
		if (mx > 0) { bucket.x += 7.0f; }
	}


	if (Key_Down(DIK_LEFT)) { bucket.x -= 7.0f; }
	else { if (Key_Down(DIK_RIGHT)) {bucket.x += 7.0f; }
}


	if (XInput_Controller_Found()) {
		if (controllers[0].sThumbLX < -5000) {
			bucket.x -= 7.0f;
		}
		else {
			if (controllers[0].sThumbLX > 5000) {
				bucket.x += 7.0f;
			}
		}

		if (controllers[0].bLeftTrigger > 128) {
			bucket.x -= 7.0f;
		}
		else {
			if (controllers[0].bRightTrigger > 128) {
				bucket.x += 7.0f;
			}
		}

		if (controllers[0].wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			bucket.x -= 7.0f;
		}
		else {
			if (controllers[0].wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				bucket.x += 7.0f;
			}
		}

		if (controllers[0].wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			bucket.x -= 7.0f;
		}
		else {
			if (controllers[0].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				bucket.x += 7.0f;
			}
		}
	}
	if (vibrating > 0) {
		vibrating++;
		if (vibrating > 20) {
			XInput_Vibrate(0, 0);
			vibrating = 0;
		}
	}

	if (bucket.x < 0) { bucket.x = 0; }
	if (bucket.x >= SCREENW-255) { bucket.x = SCREENW-260; }
	int cx = bomb.x+64;
	int cy = bomb.y +64;
	int cx2 = bomb2.x + 64;
	int cy2 = bomb2.y + 64;
	if (cx > bucket.x && cx<bucket.x+200 && cy>bucket.y && cy < bucket.y+128) {
		score++;
		char s[255];

		sprintf_s(s, "%s [SCORE %i]", AppTittle.c_str(), score);

		SetWindowText(hwnd, s);


		XInput_Vibrate(0, 65000);
		vibrating = 1;

		bomb.reset();
		
	}
	if (cx2 > bucket.x && cx2<bucket.x + 200 && cy2>bucket.y && cy2 < bucket.y + 128) {
		score++;
		char s[255];

		sprintf_s(s, "%s [SCORE %i]", AppTittle.c_str(), score);

		SetWindowText(hwnd, s);


		XInput_Vibrate(0, 65000);
		vibrating = 1;
		bomb2.reset();
	}

	d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

	if (d3ddev->BeginScene()) {
		DrawSurface(backbuffer, bomb.x, bomb.y, bomb_surf);
		DrawSurface(backbuffer, bomb2.x, bomb2.y, bomb_surf);
		DrawSurface(backbuffer, bucket.x, bucket.y, bucket_surf);


		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	if (Key_Down(DIK_ESCAPE)) {
		gameover = true;
	}

	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK) {
		gameover = true;
	}
	
}

void Game_End() {
	if (bomb_surf) bomb_surf->Release();
	if (bucket_surf) bucket_surf->Release();
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}
