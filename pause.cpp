//==========================================
//
//ポーズプログラム[meshfield.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "player.h"
#include "pause.h"
#include "input.h"
#include "color.h"

//マクロ定義
#define PAUSE_TEX_NAME		"data/TEXTURE/tutorial.png"		//ポーズ画像のファイル名

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePause = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//頂点バッファへのポインタ

//=================================
//ポーズの初期化処理
//=================================
void InitPause(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//チュートリアル情報の初期化

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEX_NAME, &g_pTexturePause);

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, NIL_F, NIL_F);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(NIL_F, SCREEN_HEIGHT, NIL_F);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);

	//rhwの設定
	pVtx[VTX_LE_UP].rhw = RHW;
	pVtx[VTX_RI_UP].rhw = RHW;
	pVtx[VTX_LE_DO].rhw = RHW;
	pVtx[VTX_RI_DO].rhw = RHW;

	//頂点カラーの設定
	pVtx[VTX_LE_UP].col = RGBA_WHITE;
	pVtx[VTX_RI_UP].col = RGBA_WHITE;
	pVtx[VTX_LE_DO].col = RGBA_WHITE;
	pVtx[VTX_RI_DO].col = RGBA_WHITE;

	//テクスチャの座標
	pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}

//=================================
//ポーズの終了処理
//=================================
void UninitPause(void) 
{
	//テクスチャの破棄
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=================================
//ポーズの更新処理
//=================================
void UpdatePause(void)
{

}

//=================================
//ポーズの描画処理
//=================================
void DrawPause(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
