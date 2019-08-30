#include "DirectX.h"
using namespace std;
const string AppTittle = "TRANSPARENTE";
const int SCREENW = 1100;
const int SCREENH = 736;

SPRITE sprite1, sprite2, sprite3;
LPDIRECT3DTEXTURE9 sprite = NULL;
LPDIRECT3DSURFACE9 surface = NULL;
LPD3DXSPRITE spriteobj = NULL;


bool Game_Init(HWND hwnd) {

	if (!Direct3D_Init(hwnd, SCREENW, SCREENH, false)) {
		MessageBox(0, "Erro ao iniciar o direct3d", "Erro", 0);
		return false;
	}


	if (!DirectInput_Init(hwnd) {
		MessageBox(0, "Erro ao iniciar os controles", "ERRO", 0);
	}

	/*
	CARREGAR A IMAGEM PARA O OBJETO 
	sprite = LoadTexture("<NOME DA IMAGEM AQUI>.<EXTENSAO DA IMAGEM AQUI>");

	surface = LoadSurface("<NOME DA IMAGEM AQUI>.<EXTENSAO DA IMAGEM AQUI>");

	DEPOIS QUE CARREGAR A IMAGEM, DEFINIR AS PROPRIEDADES DO SPRITE:
			

			--------------------------------------------SPRITE SHEET (SPRITE1 vide linha 7)------------------------
		SPRITE1.x = 50;
		SPRITE1.y = 200;
		SPRITE1.width = asteroid1.height = 160;
		SPRITE1.columns = 1;
		SPRITE1.startframe = 0;
		SPRITE1.endframe = 1;
		SPRITE1.velx = -0.1f;
		SPRITE1.vely = +0.1f;
		SPRITE1.starttime = 0;
		SPRITE1.delay = 0;
		   -----------------------------------------------SIMPLE SPRITE(SPRITE2 vide linha 7)------------------------

				SPRITE2.x = SCREENH/1.2;
				SPRITE2.y = SCREENW/5;
				SPRITE.width = ship.height = 91;

	*/

	return true;
}

void Game_Run(HWND hwnd) {
	if (!d3ddev) return;

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);




	/*

	USAR ESSA FUNÇÃO DEPOIS DE DEFINIR TODOS OS PARAMETROS DO SPRITE

	Sprite_Animate(asteroid1.frame, asteroid1.startframe, asteroid1.endframe, asteroid1.direction, asteroid1.starttime, asteroid1.delay);

	*/
	if (d3ddev->BeginScene()) {

		//->Verificar as teclas pressionadas --> DirectInput_Update();

		/*
		
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	
		
		
		
		
		FUNÇÕES PARA O JOGO RODAR
		
		
		
		
		DrawSurface(<variavel da tela a ser desenhada>,<variavel que define a posição X do objeto aqui>,<variavel que define a posição Y do objeto aqui>,<Variavel correspondente ao Objeto que vai ser desenhado na tela aqui>);
		OU
		spriteobj->Draw(<variavel do sprite a ser impresso>,<variavel que define o tamanho do sprite a ser impresso>,<variavel que fala o ponto central do sprite>,<variavel que dáa posição onde o sprite vai ser impresso>,<variavel que dá a cor>);
		OU

		Sprite_Animate(<variavel correspondente ao frame>,<variavel que fala a partir de qual frame começa a animação>,<variavel que fala quantos frames tem um sprite>, <"Direção" do prite>, <variavel que começa a contar o tempo>,<variavel que fala até aonde será contado>);
		Sprite_Draw_Frame(<sprite que vai ser animado>, <posição da tela - largura>,<posição da tela - altura>, <variavel do frame>, <tamanho de cada frame - largura>, <tamanho de cada frame - altura>,<numero de colunas do sprite>);
		spriteobj->End();
		*/

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	if(Key_Down(VK_ESCAPE)) gameover = true;


	}

void Game_End() {

	/*
	Liberação da memoria do objeto criado --> sprite->Release();
	Liberação da memoria do objeto criado --> surface->Release();
	*/
	Direct3D_Shutdown();
}



