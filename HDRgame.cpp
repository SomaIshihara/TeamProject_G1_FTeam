/*==========================================================================================================================================================

ゲーム画面処理[HDR_game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "HDRgame.h"
#include "input.h"
#include "file.h"
#include "color.h"
#include "fade.h"
#include "HDR_camera.h"
#include "camera_frame.h"
#include "HipDropRankUI.h"
#include "pause.h"
#include "hdr_player.h"
#include "model.h"
#include "light.h"
#include "sound.h"
#include "bg.h"
#include "block.h"
#include "meshfield.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "fence.h"
#include "rank.h"

//グローバル変数宣言
GAMESTATE g_gameState = GAMESTATE_NONE;
bool g_bPause_HDR = false;				// ポーズ
int  g_nUseContNum_HDR;					// 使用しているコントローラーの数
bool g_abUsePlayer_HDR[MAX_USE_GAMEPAD];// 使用しているプレイヤー（接続チェックに使用）
bool g_bDisconnectPlayer_HDR;			// 接続が切れたことを確認する
int  g_numGamePad_HDR;
int  g_nCounterGameState;
CHECKMODE	g_CheckMode_HDR;
NumCamera	g_NumCamera_HDR;
bool		g_bPhotoMode_HDR;			// フォトモード切替		true:ポーズ画面非表示	false:ボーズ画面表示

//------------------------------------------------
//				ゲームの初期化処理
//------------------------------------------------
void InitHDRGame(void)
{
	g_nUseContNum_HDR = SetUseController_HDR();			// コントローラーの使用設定
	InitFile();											// ファイルの初期化処理（モデルビューワーファイル読み込み前に行うこと！）
	LoadModelViewerFile("data\\model.txt");				// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）
	LoadModelOriginalFile("data\\originalmodel.txt");	// モデルオリジナルファイル読み込み
	g_NumCamera_HDR = NumCamera_FOUR_SIDE;				// 初期カメラの設定

	InitLight();					// ライト初期化処理
	InitModel();					// モデルの初期化処理（プレイヤーの前に行うこと！）
	InitPlayer_HDR();				// プレイヤーの初期化処理
	InitCameraFrame();				// 画面分割の枠初期化処理
	InitHDRCamera(g_NumCamera_HDR);	// カメラの初期化処理
	InitPause();					// ポーズ画面の初期化処理
	InitBlock();					// ブロックの初期化処理
	InitMeshDome();					// ドームの初期化処理
	InitMeshfield();				// フィールドの初期化処理
	InitMeshCylinder();				// メッシュシリンダー初期化処理
	InitFence();					// フェンスの初期化処理
	InitRank();						// ランキングの初期化処理
	InitHipDropRankUI();			// ヒップドロップランクUI初期化処理

	g_bPause_HDR = false;				// ポーズの初期化
	g_bDisconnectPlayer_HDR = false;	//正常にコントローラーが接続されている状態とする
	g_bPhotoMode_HDR = false;			// フォトモード初期化

	//ゲームBGM開始
	PlaySound(SOUND_LABEL_BGM_GAME_HDR, 0);
}

//------------------------------------------------
//				ゲームの終了処理
//------------------------------------------------
void UninitHDRGame(void)
{
	/*======================================================================
	各種オブジェクトの終了処理
	========================================================================*/

	UninitLight();			// ライト終了処理
	UninitHDRCamera();		// カメラの終了処理
	UninitPause();			// ポーズ画面の終了処理
	UninitPlayer_HDR();		// プレイヤーの終了処理
	UninitModel();			// モデルの終了処理
	UninitBlock();			// ブロックの終了処理
	UninitMeshfield();		// フィールドの終了処理
	UninitMeshDome();		// ドームの終了処理
	UninitMeshCylinder();	// メッシュシリンダー終了処理
	UninitFence();			// フェンスの終了処理
	UninitRank();			// ランキングの終了処理
	UninitHipDropRankUI();	// ヒップドロップランクUI終了処理

	UninitCameraFrame();	// 画面分割の枠終了処理

	//ゲームBGM停止
	StopSound(SOUND_LABEL_BGM_GAME_HDR);
}

//------------------------------------------------
//				ゲームの更新処理
//------------------------------------------------
void UpdateHDRGame(void)
{
	FADE fadeState = GetFade();

	//ポーズがOFF
	if (g_bPause_HDR == false)
	{
		UpdateHDRCamera();		// カメラの更新処理

		if (*GetHDR_Ready() == HDR_Ready_OK)
		{
			UpdateLight();			// ライトの更新処理
			UpdateBlock();			// ブロックの更新処理
			UpdatePlayer_HDR();		// ヒップドロップレース用プレイヤーの更新処理
			UpdateMeshCylinder();	// メッシュシリンダー更新処理
			UpdateHipDropRankUI();	// ヒップドロップランクUI更新処理

			ChangeNumCamera_HDR();	//カメラの数変更処理
		}

		//ポーズ取得
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			//ポーズ状態切り替え
			if (GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(nCntPause, XINPUT_GAMEPAD_START) == true)
			{
				//何番目のゲームパッドか保存する
				g_numGamePad_HDR = nCntPause;

				PlaySound(SOUND_LABEL_SE_PAUSE_DECISION, 0);

				//ポーズ状態にする
				g_bPause_HDR = true;

				//ポーズしたコントローラーをpause.cppに渡す
				SetPadPause(false, nCntPause);
			}
		}
	}

	else
	{
		//ポーズ画面の更新処理
		UpdatePause(MODE_RaceGAME);

		//フォトモードON
		if (g_bPhotoMode_HDR)
		{
			UpdateHDRCamera();

			//1フレームだけ更新する
			if (GetKeyboardTrigger(DIK_RIGHT))
			{
				UpdateLight();			// ライトの更新処理
				UpdateBlock();			// ブロックの更新処理
				UpdatePlayer_HDR();		// ヒップドロップレース用プレイヤーの更新処理
				UpdateMeshCylinder();	// メッシュシリンダー更新処理
				UpdateHipDropRankUI();	// ヒップドロップランクUI更新処理
			}
		}
	}

	//コントローラーの使用チェックはポーズ状態関係なく呼び出し
	CheckUseController_HDR(CHECKMODE_DISCONPAUSE);

	//フォトモード切替
	if (GetKeyboardTrigger(DIK_F9))
	{
		g_bPhotoMode_HDR = g_bPhotoMode_HDR ? false : true;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:					//通常状態
		break;
	case GAMESTATE_END:						//終了状態
		g_nCounterGameState--;

		FADE pFade;		//フェードへのポインタ
		pFade = GetFade();

		if (pFade == FADE_NONE)
		{
			if (g_nCounterGameState <= 0)
			{
				g_gameState = GAMESTATE_NONE;	//何もしていない状態に設定

				SetFade(MODE_RESULT);
			}
		}
		break;
	}
}

//------------------------------------------------
//				ゲームの描画処理
//------------------------------------------------
void DrawHDRGame(void)
{
	/*======================================================================
	各種オブジェクトの描画処理
	========================================================================*/
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		//ゲーム内オブジェクトの描画処理
		SetHDRCamera(nCntCamera);	// カメラの設定処理
		DrawCameraFrame();			// 画面分割の枠描画処理
		DrawBlock();				// ブロックの描画処理
		DrawPlayer_HDR();			// プレイヤーの描画処理
		DrawMeshfield();			// フィールドの描画処理
		DrawMeshCylinder();			// 背景の描画処理
		DrawMeshDome();				// ドームの描画処理
		DrawFence();				// フェンスの描画処理
		DrawRank();					// ランキングの描画処理
		DrawHipDropRankUI();		// ヒップドロップランクUI描画処理

		//ポーズがON
		if (g_bPause_HDR == true && g_bPhotoMode_HDR == false)
		{
			DrawPause();		//ポーズ画面描画処理
		}
	}
}

//------------------------------------------------
//		使用するコントローラーの設定
//		Author:石原颯馬
//		返り値:使用されている（接続されている）コントローラーの数
//		Memo:InitGameで呼び出し（GetUseControllerが呼び出される前に呼び出しすること）
//------------------------------------------------
int SetUseController_HDR(void)
{
	int nUseController = 0;
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		if (GetUseGamepad(nCntController) == true)
		{//使用されている
			g_abUsePlayer_HDR[nCntController] = true;
			nUseController++;	//コントローラーの数追加
		}
		else
		{//使用されていない
			g_abUsePlayer_HDR[nCntController] = false;
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
bool CheckUseController_HDR(CHECKMODE mode)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//対応コントローラー分繰り返す
		if (g_abUsePlayer_HDR[nCntPlayer] == true && g_abUsePlayer_HDR[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//切断を検知
			switch (mode)
			{
			case CHECKMODE_DISCONPAUSE:
				g_bPause_HDR = true;				//強制的にポーズ状態にする
				g_bDisconnectPlayer_HDR = true;		//切断されたことにする
				SetPadPause(true);
				return true;	//関数終了
				break;
			case CHECKMODE_DISCONNOPAUSE:
				g_bDisconnectPlayer_HDR = true;		//切断されたことにする
				return true;	//関数終了
				break;
			case CHECKMODE_REMOVE:
				g_abUsePlayer_HDR[nCntPlayer] = false;
				g_nUseContNum_HDR--;	//コントローラー使用数減らす
				break;
			default:
				break;
			}
		}
	}
	g_bDisconnectPlayer_HDR = false;	//for文が正常に終了したら問題ない
	return false;
}

//------------------------------------------------
//		コントローラーの使用有無取得処理
//		Author:石原颯馬
//		引数1:調べるパッドの番号
//------------------------------------------------
bool GetUseController_HDR(int nPadNum)
{
	return g_abUsePlayer_HDR[nPadNum];
}

//------------------------------------------------
//		コントローラーの使用個数取得処理
//		Author:石原颯馬
//------------------------------------------------
int GetUseControllerNum_HDR(void)
{
	return g_nUseContNum_HDR;
}

//------------------------------------------------
//		ポーズ状態の設定処理
//		Author:石原颯馬
//------------------------------------------------
void SetEnablePause_HDR(bool pause)
{
	g_bPause_HDR = pause;
}

//------------------------------------------------
//		カメラの数変更処理
//		Author:平澤詩苑
//------------------------------------------------
void ChangeNumCamera_HDR(void)
{
	if (GetKeyboardTrigger(DIK_F7))
	{
		//現在のカメラの種類を保存
		int nType = g_NumCamera_HDR;

		//次の種類に変更
		nType++;

		//変更に伴い、全種類の数を超えないようにする
		nType %= NumCamera_MAX;

		//変更したものを入れる
		g_NumCamera_HDR = (NumCamera)nType;

		//カメラの種類を設定
		Set_NumHDRCamera(g_NumCamera_HDR);
	}
}
//================================================================
//ゲームの状態の設定
//================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}