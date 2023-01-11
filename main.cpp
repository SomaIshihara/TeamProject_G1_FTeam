/*==========================================================================================================================================================

メインの処理[main.cpp]
Author:大宮愛羅

============================================================================================================================================================*/

#include "main.h"
//#include "input.h"
//#include "camera.h"
//#include "light.h"
//#include "ground.h"
//#include "Player.h"
//#include "shadow.h"
//#include "bullet.h"
#include "wall.h"
#include <Windows.h>
//#include "block.h"
//#include "billboard.h"
//#include "meshfield.h"
//#include "title.h"
//#include "game.h"
//#include "result.h"
//#include "fade.h"
//#include "sound.h"
//#include "tutorial.h"



//マクロ定義
#define CLASS_NAME  "WindowClass"			//ウインドウクラスの名前
#define WINDOW_NAME "C・フォードの六重塔"		//ウインドウの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;
int g_nCountFPS = 0;
MODE g_mode = MODE_NONE;	//現在のモード

							//===================================================
							//メイン関数
							//===================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdshow)
{
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//メモリサイズ
		CS_CLASSDC,						//スタイル
		WindowProc,						//ウインドウプロシージャ                                
		0,								//0にする
		0,								//0にする
		hInstance,						//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		//クライアント領域の背景色
		NULL,							//メニューバー
		CLASS_NAME,						//クラスの名前
		LoadIcon(NULL,IDI_APPLICATION),
	};
	HWND hWnd;	//ウインドウハンドル
	MSG  msg;	//メッセージを格納する変数

				//ウインドウクラスの登録
	RegisterClassEx(&wcex);
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//ウインドウ生成
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能の設定
	timeBeginPeriod(1);

	//描画処理関連の変数の初期化
	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得(保存)

										//FPS関連の変数を初期化
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウ表示
	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if ((PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0))
		{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
			if (msg.message == WM_QUIT)
			{
				break;
			}

			else
			{
				//メッセージ設定
				TranslateMessage(&msg);	//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime(); //現在時刻を取得(保存)

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime; //処理開始の時刻[現在時刻]を保存

												//更新処理
				Update();

				//描画処理
				Draw();

				//フレームカウントを加算
				dwFrameCount++;
			}
		}
	}

	//終了処理
	Uninit();

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;

	//分解能を戻す
	timeEndPeriod(1);
}

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: //ウインドウ破棄のメッセージ
					 //WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]キーが押された
							//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//既定の処理を返す
}

//メイン初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;



	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//ディスプレイのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//DirectX3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice,
		18, 0, 0, 0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&g_pFont);

	/*=====================================================================
	各種オブジェクトの初期化処理
	=======================================================================*/
	//レンダーステート
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//キーボードの初期化
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//メッシュフィールド
	//InitMeshfield();		

	//InitBillboard();

	//フェード
	//InitFade(g_mode);

	//サウンド
	//InitSound(hWnd);

	// モードをタイトルに設定
	SetMode(MODE_TITLE);

	return S_OK;
}

//メイン終了処理
void Uninit(void)
{
	//キーボード
	//UninitKeyboard();

	//メッシュフィールド
	//UninitMeshfield();	

	//UninitBillboard();

	//フェード
	//UninitFade();

	//タイトル画面の終了処理
	//UninitTitle();

	//チュートリアル画面の終了処理
	//UninitTutorial();

	//ゲーム画面の終了処理
	//UninitGame();

	//リザルト画面の終了処理
	//UninitResult();

	//サウンド
	//UninitSound();

	//DirectX3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//DirectX3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//メイン更新処理
void Update(void)
{
	//キーボードの更新処理
	//UpdateKeyboard();

	//メッシュフィールド
	//UpdateMeshfield();

	//UpdateBillboard();

	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		//UpdateTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		//UpdateTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		//UpdateGame();
		break;

	case MODE_GAMERESULT:	//リザルト画面
		//UpdateResult();
		break;
	}

	//フェード
	//UpdateFade();
}

//メイン描画処理
void Draw(void)
{
	//画面クリア(バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合 			

	 //DrawBillboard();

		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面
			//DrawTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			//DrawTutorial();
			break;

		case MODE_GAME:			//ゲーム画面
			//DrawGame();
			break;

		case MODE_GAMERESULT:	//リザルト画面
			//DrawResult();
			break;
		}

		//フェード
		//DrawFade();

		//FPSの表示
		DrawFPS();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファ
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//FPSの表示
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの表示
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//画面のモード設定
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		//UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		//UninitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		//UninitGame();
		break;

	case MODE_GAMERESULT:	//リザルト画面
		//UninitResult();
		break;
	}

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面
		//InitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		//InitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		//InitGame();
		break;

	case MODE_GAMERESULT:	//リザルト画面
		//InitResult();
		break;
	}

	g_mode = mode;			//現在の画面(モード)を切り替える
}

//画面モードの取得
MODE GetMode(void)
{
	return g_mode;
}