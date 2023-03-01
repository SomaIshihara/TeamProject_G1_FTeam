//==========================================
//
//ランクプログラム[rank.cpp]
//Author:藤原龍輝
//
//==========================================
#include "rank.h"
#include "color.h"
#include "fade.h"

#define POS_XY (150)

//****************************//
//	   グローバル変数宣言     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureRank = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;	//頂点バッファへのポインタ
RANK g_Rank;							//ポーズの状態
Rank g_aRank[MAX_RANK];							//ポーズの情報

//=================================
//ランクの初期化処理
//=================================
void InitRank(void)
{

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * MAX_RANK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRank, NULL);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank.png", &g_pTextureRank);
	
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	//各情報読み込み
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		g_aRank[nCnt].pos = D3DXVECTOR3(160.0f + nCnt * 320, 600.0f, 0.0f);
		g_aRank[nCnt].buse = false;
	}

	g_Rank = RANK_1ST;

	//各情報読み込み
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++, pVtx += VTX_MAX)
	{
		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x - POS_XY, g_aRank[nCnt].pos.y - POS_XY, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x + POS_XY, g_aRank[nCnt].pos.y - POS_XY, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x - POS_XY, g_aRank[nCnt].pos.y + POS_XY, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x + POS_XY, g_aRank[nCnt].pos.y + POS_XY, NIL_F);

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
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(0.25f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffRank->Unlock();
}
//=================================
//ランクの終了処理
//=================================
void UninitRank(void)
{
	//テクスチャの破棄
	if (g_pTextureRank != NULL)
	{
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}
//=================================
//ランクの更新処理
//=================================
void UpdateRank(int nPlayer)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++, pVtx += VTX_MAX)
	{
		if (nCnt == nPlayer)
		{
			g_aRank[nCnt].buse = true;

			//テクスチャの座標
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f + g_Rank * 0.25f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(0.25f + g_Rank * 0.25f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f + g_Rank * 0.25f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(0.25f + g_Rank * 0.25f, 1.0f);

			g_Rank = (RANK)(g_Rank + 1);

		/*	if (g_Rank == RANK_MAX)
			{
				SetFade(MODE_RESULT);
			}*/
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffRank->Unlock();
}
//=================================
//ポーズの描画処理
//=================================
void DrawRank(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (g_aRank[nCnt].buse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRank);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * VTX_MAX, 2);
		}
	}
}