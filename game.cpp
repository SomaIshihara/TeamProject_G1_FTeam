/*==========================================================================================================================================================

ゲーム画面処理[game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "file.h"
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
#include "meshdome.h"
#include "Gauge.h"
#include "effect.h"
#include "score.h"
#include "bonus.h"
//#include "sound.h"

//グローバル変数宣言
bool g_bPause = false;				// ポーズ
int  g_nUseContNum;					// 使用しているコントローラーの数
bool g_abUsePlayer[MAX_USE_GAMEPAD];// 使用しているプレイヤー（接続チェックに使用）
bool g_bDisconnectPlayer;			// 接続が切れたことを確認する
int  g_numGamePad;
CHECKMODE g_CheckMode;

//------------------------------------------------
//				ゲームの初期化処理
//------------------------------------------------
void InitGame(void)
{
	g_nUseContNum = SetUseController();		// コントローラーの使用設定
	InitFile();								// ファイルの初期化処理（モデルビューワーファイル読み込み前に行うこと！）
	LoadModelViewerFile("data\\model.txt");	// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）

	InitBg();			// 背景の初期化処理
	InitLight();		// ライト初期化処理
	InitMeshfield();	// ステージ初期化処理
	InitMeshCylinder();	// メッシュシリンダー初期化処理
	//InitMeshDome();		// メッシュドーム初期化処理
	InitModel();		// モデルの初期化処理（プレイヤーの前に行うこと！）
	InitPlayer();		// プレイヤーの初期化処理
	InitBonus();		// ボーナスの初期化処理
	InitCamera();		// カメラの初期化処理
	InitWall();			// 壁の初期化処理
	InitGauge();		// ゲージの初期化処理
	InitPause();		// ポーズ画面の初期化処理
	InitEffect();		// エフェクトの初期化処理
	InitScore();		// スコアの初期化


	SetScore(0,4);		// スコアの設定処理

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
	//UninitMeshDome();	// メッシュドーム終了処理
	UninitWall();		// 壁の終了処理
	UninitCamera();		// カメラの終了処理
	UninitPlayer();		// プレイヤーの終了処理
	UninitBonus();		// ボーナスの終了処理
	UninitPause();		// ポーズ画面の終了処理
	UninitGauge();		// ゲージの終了処理
	UninitModel();		// モデルの終了処理
	UninitEffect();	// エフェクトの終了処理（ここは順番は問わない）
	UninitScore();		// スコアの終了処理

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
		//UpdateMeshDome();	// メッシュドーム更新処理
		UpdatePlayer();		// プレイヤーの更新処理
		UpdateBonus();		// ボーナスの更新処理
		UpdateCamera();		// カメラの更新処理
		UpdateWall();		// 壁の更新処理
		UpdateGauge();		// ゲージの更新処理
		UpdateEffect();		// エフェクトの更新処理
		UpdateScore();		//スコアの更新処理

		//ポーズ取得
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			//ポーズ状態切り替え
			if (GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(nCntPause, XINPUT_GAMEPAD_START) == true)
			{
				//何番目のゲームパッドか保存する
				g_numGamePad = nCntPause;

				//ポーズ状態にする
				g_bPause = true;

				//ポーズしたコントローラーをpause.cppに渡す
				SetPadPause(false, nCntPause);
			}
		}
	}

	else
	{
		//ポーズ画面の更新処理
		UpdatePause();
	}

	//コントローラーの使用チェックはポーズ状態関係なく呼び出し
	CheckUseController(CHECKMODE_DISCONPAUSE);
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
						//DrawMeshCylinder();	// メッシュシリンダーの描画処理
	//DrawMeshDome();		// メッシュドームの描画処理	
	DrawWall();			// 壁の描画処理
	DrawEffect();		// エフェクトの描画処理
	DrawPlayer();		// プレイヤーの描画処理
	DrawBonus();		// ボーナスの描画処理
	DrawGauge();		// ゲージの描画処理
	DrawScore();		// スコアの描画処理

						//ポーズがOFF
	if (g_bPause == true)
	{
		DrawPause();		//ポーズ画面描画処理
	}
}

//------------------------------------------------
//		使用するコントローラーの設定
//		Author:石原颯馬
//		返り値:使用されている（接続されている）コントローラーの数
//		Memo:InitGameで呼び出し（GetUseControllerが呼び出される前に呼び出しすること）
//------------------------------------------------
int SetUseController(void)
{
	int nUseController = 0;
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		if (GetUseGamepad(nCntController) == true)
		{//使用されている
			g_abUsePlayer[nCntController] = true;
			nUseController++;	//コントローラーの数追加
		}
		else
		{//使用されていない
			g_abUsePlayer[nCntController] = false;
		}
	}
	return nUseController;	//使用されているコントローラーの数を返す
}

//------------------------------------------------
//		使用しているコントローラーの接続チェック
//		Author:石原颯馬
//		引数1:チェックモード指定
//		CHECKMODE_DISCONNECT:切断されている場合強制ポーズ状態にするモード
//		CHECKMODE_REMOVE	:切断されている場合そのコントローラーを使用していない状態にする
//		Memo:UpdateGame内でポーズ中でも呼び出しすること
//------------------------------------------------
bool CheckUseController(CHECKMODE mode)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//対応コントローラー分繰り返す
		if (g_abUsePlayer[nCntPlayer] == true && g_abUsePlayer[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//切断を検知
			switch (mode)
			{
			case CHECKMODE_DISCONPAUSE:
				g_bPause = true;				//強制的にポーズ状態にする
				g_bDisconnectPlayer = true;		//切断されたことにする
				SetPadPause(true);
				return true;	//関数終了
				break;
			case CHECKMODE_DISCONNOPAUSE:
				g_bDisconnectPlayer = true;		//切断されたことにする
				return true;	//関数終了
				break;
			case CHECKMODE_REMOVE:
				g_abUsePlayer[nCntPlayer] = false;
				g_nUseContNum--;	//コントローラー使用数減らす
				break;
			default:
				break;
			}
		}
	}
	g_bDisconnectPlayer = false;	//for文が正常に終了したら問題ない
	return false;
}

//------------------------------------------------
//		コントローラーの使用有無取得処理
//		Author:石原颯馬
//		引数1:調べるパッドの番号
//------------------------------------------------
bool GetUseController(int nPadNum)
{
	return g_abUsePlayer[nPadNum];
}

//------------------------------------------------
//		コントローラーの使用個数取得処理
//		Author:石原颯馬
//------------------------------------------------
int GetUseControllerNum(void)
{
	return g_nUseContNum;
}

//------------------------------------------------
//		ポーズ状態の設定処理
//		Author:石原颯馬
//------------------------------------------------
void SetEnablePause(bool pause)
{
	g_bPause = pause;
}