/*==========================================================================================================================================================

リザルト画面処理[result.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "resultPlayer.h"

#define NUM_RESULT			(1)		//リザルト画像の種類

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;			//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureResult[NUM_RESULT] = {};	//テクスチャのポインタ

//************************************************
//				リザルトの初期化処理
//************************************************
void InitResult(void)
{
	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void* *)&pVtx, 0);

	//各情報の初期化
	for (int nCntRuselt = 0; nCntRuselt < NUM_RESULT; nCntRuselt++, pVtx += VTX_MAX)
	{
		//テクスチャの読み込み


		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, 0.0f, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffResult->Unlock();

	//===============================================
	//リザルト用オブジェクトの初期化処理関数呼び出し
	//===============================================
	InitResultObject();
}

//================================================
//		リザルト用オブジェクトの初期化処理
//================================================
void InitResultObject(void)
{
	InitPlayer_RESULT();		//プレイヤーの初期化処理
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

//================================================
//		リザルト用オブジェクトの終了処理
//================================================
void UninitResultObject(void)
{
	UninitPlayer_RESULT();		//プレイヤーの終了処理
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
}

//================================================
//		リザルト用オブジェクトの更新処理
//================================================
void UpdateResultObject(void)
{
	UpdatePlayer_RESULT();		//プレイヤーの更新処理
}

//************************************************
//				リザルトの描画処理
//************************************************
void DrawResult(void)
{
	//デバイスのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//===============================================
	//リザルト用オブジェクトの描画処理関数呼び出し
	//===============================================
	DrawResultObject();
}

//================================================
//		リザルト用オブジェクトの描画処理
//================================================
void DrawResultObject(void)
{
	DrawPlayer_RESULT();		//プレイヤーの描画処理
}