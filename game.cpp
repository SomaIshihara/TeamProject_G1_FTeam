/*==========================================================================================================================================================

ゲーム画面処理[game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "color.h"
#include "player.h"
#include "wall.h"
#include "game.h"
#include "fade.h"
//#include "pause.h"
//#include "sound.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;			// 頂点バッファへのポインタ
bool					g_bPause = false;				// ポーズ

//------------------------------------------------
//				ゲームの初期化処理
//------------------------------------------------
void InitGame(void)
{
	InitPlayer();		// プレイヤーの初期化処理
	InitCamera();		// カメラの初期化処理
	InitWall();			// 壁の初期化処理

	g_bPause = false;	// ポーズの初期化

						//ゲームBGM開始
	//PlaySound(SOUND_LABEL_GAMEBGM);
}

//------------------------------------------------
//				ゲームの終了処理
//------------------------------------------------
void UninitGame(void)
{
	/*======================================================================
	各種オブジェクトの終了処理
	========================================================================*/

	/*エフェクトなどの板ポリゴンの終了処理*/
	UninitWall();		// 壁の終了処理
	UninitCamera();		// カメラの終了処理
	UninitPlayer();		// プレイヤーの終了処理

						//ゲームBGM停止
	//StopSound(SOUND_LABEL_GAMEBGM);
}

//------------------------------------------------
//				ゲームの更新処理
//------------------------------------------------
void UpdateGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();

	//ポーズがOFF
	if (g_bPause == false)
	{
			UpdatePlayer();		//プレイヤーの更新処理
			UpdateCamera();		//カメラの更新処理
			UpdateWall();			//壁の更新処理
	}

	else
	{
		//ポーズ画面の更新処理
		//UpdatePause();

		//ポーズを解除するときにメニューをコンティニューに設定
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			//SetPause(PAUSE_MENU_CONTINUE);
		}

		//if (*GetPause() == PAUSE_MENU_CONTINUE)
		//{
		//	if (GetKeyboardTrigger(DIK_RETURN) == true)
		//	{
		//		g_bPause = false;
		//	}
		//}
	}

	//ポーズ状態切り替え
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	//Enterが押された　もしくは　ゲームパッドのSTART or A ボタンが押された
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//タイトル画面に遷移
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//				ゲームの描画処理
//------------------------------------------------
void DrawGame(void)
{
	/*======================================================================
	各種オブジェクトの描画処理
	========================================================================*/
	//ゲーム内オブジェクトの描画処理
	SetCamera();		// カメラの設定処理

	DrawWall();			// 壁の描画処理
	DrawPlayer();		// プレイヤーの描画処理
}