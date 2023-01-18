/*==========================================================================================================================================================

ゲーム画面処理[game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "color.h"
#include "player.h"
#include "model.h"
#include "wall.h"
#include "game.h"
#include "fade.h"
#include "bg.h"
#include "meshfield.h"
#include "light.h"
#include "meshcylinder.h"
#include "pause.h"
//#include "sound.h"

//プロト

int SetUsePlayer(void);
void CheckUsePlayer(void);

//グローバル変数宣言
bool g_bPause = false;				// ポーズ
bool g_abUsePlayer[MAX_USE_GAMEPAD];//使用しているプレイヤー（接続チェックに使用）
bool g_bDisconnectPlayer;			//接続が切れたことを確認する

//------------------------------------------------
//				ゲームの初期化処理
//------------------------------------------------
void InitGame(void)
{
	InitBg();			// 背景の初期化処理
	InitLight();		// ライト初期化処理
	InitMeshfield();	// ステージ初期化処理
	InitMeshCylinder();	// メッシュシリンダー初期化処理
	InitModel();		// モデルの初期化処理（プレイヤーの前に行うこと！）
	InitPlayer();		// プレイヤーの初期化処理
	InitCamera();		// カメラの初期化処理
	InitWall();			// 壁の初期化処理
	InitPause();		// ポーズ画面の初期化処理

	g_bPause = false;	// ポーズの初期化
	g_bDisconnectPlayer = false;	//正常にコントローラーが接続されている状態とする

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
	UninitBg();			// 背景の終了処理
	UninitLight();		// ライト終了処理
	UninitMeshfield();	// ステージ終了処理
	UninitMeshCylinder();	// メッシュシリンダー終了処理
	UninitWall();		// 壁の終了処理
	UninitCamera();		// カメラの終了処理
	UninitPlayer();		// プレイヤーの終了処理
	UninitPause();		// ポーズ画面の終了処理
	UninitModel();		// モデルの終了処理（ここは順番は問わない）

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
			UpdateBg();			// 背景の更新処理
			UpdateLight();		// ライトの更新処理
			UpdateMeshfield();	// ステージ更新処理
			UpdateMeshCylinder();	// メッシュシリンダー更新処理
			UpdatePlayer();		// プレイヤーの更新処理
			UpdateCamera();		// カメラの更新処理
			UpdateWall();		// 壁の更新処理
	}

	else
	{
		//ポーズ画面の更新処理
		UpdatePause();

		//ポーズを解除するときにメニューをコンティニューに設定
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			SetPause(PAUSE_CONTINUE);
		}

		if (*GetPause() == PAUSE_CONTINUE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_bPause = false;
			}
		}
	}

	//ポーズ状態切り替え
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
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

	DrawMeshfield();	// ステージの描画処理
	DrawMeshCylinder();	// メッシュシリンダーの描画処理
	DrawWall();			// 壁の描画処理
	DrawPlayer();		// プレイヤーの描画処理

	//ポーズがOFF
	if (g_bPause == true)
	{
		DrawPause();		//ポーズ画面描画処理
	}

}

//------------------------------------------------
//		使用するプレイヤーの設定
//		Author:石原颯馬
//		返り値:使用されている（接続されている）コントローラーの数
//		Memo:InitGameで呼び出し
//------------------------------------------------
int SetUsePlayer(void)
{
	int nUsePlayer = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (GetUseGamepad(nCntPlayer) == true)
		{//使用されている
			g_abUsePlayer[nCntPlayer] = true;
			nUsePlayer++;	//コントローラーの数追加
		}
		else
		{//使用されていない
			g_abUsePlayer[nCntPlayer] = false;
		}
	}
	return nUsePlayer;	//使用されているコントローラーの数を返す
}

//------------------------------------------------
//		使用しているプレイヤーの接続チェック
//		Author:石原颯馬
//		Memo:UpdateGame内でポーズ中でも呼び出しすること
//------------------------------------------------
void CheckUsePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//対応コントローラー分繰り返す
		if (g_abUsePlayer[nCntPlayer] == true && g_abUsePlayer[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//切断を検知
			g_bPause = true;				//強制的にポーズ状態にする
			g_bDisconnectPlayer = true;		//切断されたことにする
			return;	//関数終了
		}
	}
	g_bDisconnectPlayer = false;	//for文が正常に終了したら問題ない
}
