/*==========================================================================================================================================================

リザルト画面処理[result.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "fade.h"
#include "file.h"
#include "model.h"
#include "resultPlayer.h"
#include "resultCamera.h"
#include "VictoryStand.h"
#include "resultCylinder.h"
#include "conversioninput.h"

#define NUM_RESULT			(1)		//リザルト画像の種類

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;			//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureResult[NUM_RESULT] = {};	//テクスチャのポインタ

//************************************************
//				リザルトの初期化処理
//************************************************
void InitResult(void)
{
	//===============================================
	//リザルト用オブジェクトの初期化処理関数呼び出し
	//===============================================
	InitResultObject();
}

//------------------------------------------------
//		リザルト用オブジェクトの初期化処理
//------------------------------------------------
void InitResultObject(void)
{
	InitFile();				//ファイル初期化
	LoadModelViewerFile("data\\model.txt");	// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）
	InitResultCylinder();	//背景の初期化処理
	InitVictoryStand();		//表彰台の初期化処理
	InitAnimalModel();		//動物モデル初期化
	InitPlayer_RESULT();	//プレイヤーの初期化処理
	InitResultCamera();		//カメラの初期化処理
}

//************************************************
//				リザルトの終了処理
//************************************************
void UninitResult(void)
{
	//テクスチャ(２枚)の破棄
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//===============================================
	//リザルト用オブジェクトの終了処理関数呼び出し
	//===============================================
	UninitResultObject();
}

//------------------------------------------------
//		リザルト用オブジェクトの終了処理
//------------------------------------------------
void UninitResultObject(void)
{
	UninitResultCylinder();		//背景の終了処理
	UninitModel();				//動物モデル終了処理
	UninitPlayer_RESULT();		//プレイヤーの終了処理
	UninitResultCamera();		//カメラの終了処理
	UninitVictoryStand();		//表彰台の終了処理
}

//************************************************
//				リザルトの更新処理
//************************************************
void UpdateResult(void)
{
	//===============================================
	//リザルト用オブジェクトの更新処理関数呼び出し
	//===============================================
	UpdateResultObject();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//		リザルト用オブジェクトの更新処理
//------------------------------------------------
void UpdateResultObject(void)
{
	UpdatePlayer_RESULT();		//プレイヤーの更新処理
	UpdateResultCamera();		//カメラの更新処理
	UpdateVictoryStand();		//表彰台の更新処理
	UpdateResultCylinder();		//背景の更新処理
}

//************************************************
//				リザルトの描画処理
//MEMO:2DUIを描画する場合はαテストを忘れずに！
//************************************************
void DrawResult(void)
{
	//===============================================
	//リザルト用オブジェクトの描画処理関数呼び出し
	//===============================================
	DrawResultObject();
}

//------------------------------------------------
//		リザルト用オブジェクトの描画処理
//------------------------------------------------
void DrawResultObject(void)
{
	SetResultCamera();			//カメラの設定処理
	DrawResultCylinder();		//背景の描画処理
	DrawPlayer_RESULT();		//プレイヤーの描画処理
	DrawVictoryStand();			//表彰台の描画処理
}