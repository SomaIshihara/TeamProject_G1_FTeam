/*==========================================================================================================================================================

ゲージの処理[gauge.cpp]
Author:大宮愛羅

============================================================================================================================================================*/

#include "main.h"
#include "Gauge.h"
#include "player.h"
#include "input.h"

//マクロ定義
#define NUM_GAUGE		(3)		//ゲージの数
#define DISPLAY_ADD		(4)		//表示する分だけ加算 
#define TOP_NUM			(12)	//頂点数

//初期化関連
#define MIN_POS		(0.0f)
#define MIN_COL		(0.0f)
#define MIN_GA_WID	(200)
#define MIN_WIDTH	(200)
#define MIN_HEIGHT	(200)

//位置関連

//rhw関連
#define MAX_RHW		(1.0f)	//rhwの最大値

//色関連
#define USE_RED		(1.0f)	//赤色を使用する
#define	NUSE_RED	(0.0f)	//赤色を使用しない
#define USE_GREEN	(1.0f)	//緑色を使用する
#define NUSE_GREEN	(0.0f)	//緑色を使用しない
#define USE_BLUE	(1.0f)	//青色を使用する
#define	NUSE_BLUE	(0.0f)	//青色を使用しない
#define	USE_ALPHA	(1.0f)	//アルファー

//テクスチャ関連
#define	MAX_TEX		(1.0f)	//テクスチャの最大値
#define MIN_TEX		(0.0f)	//テクスチャの最小値
#define TEX_ZERO	(0)		//テクスチャの読み込み(0)
#define TEX_ONE		(1)		//テクスチャの読み込み(1)
#define TEX_TWO		(2)		//テクスチャの読み込み(2)

//更新関連
#define COUNT_ONE	(1)		//カウントが1
#define MAX_NUM		(100)	//割られる数(最大値)
#define UNHALF_NUM  (75)	//割る数(最大値-25)
#define HALF_NUM	(50)	//割る数(最大値の半分)
#define QUAR_NUM	(25)	//割る数(最大値の4分の1)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGauge = NULL;					//バッファへのポインタ
LPDIRECT3DTEXTURE9      g_pTextureGauge[NUM_GAUGE] = {};		//テクスチャへのポインタ
Gauge g_Gauge[NUM_GAUGE];										//ゲージ情報

//初期化処理
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ取得

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,		//==============
		"data\\TEXTURE\\GaugeBg.png",		//ゲージ背景
		&g_pTextureGauge[TEX_ZERO]);		//===========

	D3DXCreateTextureFromFile(pDevice,		//==============
		"data\\TEXTURE\\Gauge.png",			//ゲージ本体
		&g_pTextureGauge[TEX_ONE]);			//===========

	D3DXCreateTextureFromFile(pDevice,		//==============
		"data\\TEXTURE\\GaugeFrame.png",	//ゲージの枠
		&g_pTextureGauge[TEX_TWO]);			//===========
	
	//ゲージ情報の初期化処理
	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{
		g_Gauge[nCntGauge].pos = D3DXVECTOR3(MIN_POS, MIN_POS, MIN_POS);
		g_Gauge[nCntGauge].col = D3DXCOLOR(MIN_COL, MIN_COL, MIN_COL, MIN_COL);
		g_Gauge[nCntGauge].fGaugeWidth = MIN_GA_WID;
		g_Gauge[nCntGauge].fWidth = MIN_WIDTH;
		g_Gauge[nCntGauge].fHeight = MIN_HEIGHT;
		g_Gauge[nCntGauge].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * TOP_NUM * NUM_GAUGE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++, pVtx + DISPLAY_ADD)
	{
		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(500.0f, 650.0f, 0.0f);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(50.0f, 710.0f, 0.0f);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(500.0f, 710.0f, 0.0f);
						
		//rhwの設定
		pVtx[VTX_LE_UP].rhw = MAX_RHW;
		pVtx[VTX_RI_UP].rhw = MAX_RHW;
		pVtx[VTX_LE_DO].rhw = MAX_RHW;
		pVtx[VTX_RI_DO].rhw = MAX_RHW;
						
		//頂点カラー設定
		pVtx[VTX_LE_UP].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
				
		//テクスチャ情報の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);				
	}
	//頂点バッファをロックする
	g_pVtxBuffGauge->Unlock();
}

//終了処理
void UninitGauge(void)
{
	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{//ゲージ分
		//テクスチャの破棄
		if (g_pTextureGauge[nCntGauge] != NULL)
		{
			g_pTextureGauge[nCntGauge]->Release();
			g_pTextureGauge[nCntGauge] = NULL;
		}
	}	

	//頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//更新処理
void UpdateGauge(void)
{
	Player *pPlayer = GetPlayer();		//プレイヤー情報の取得

	int Parcent;						//ゲージ量を格納する変数
	int MaxGauge = 100;					//最大ゲージ量		

	for (int nCntGauge = 0; nCntGauge < MAX_USE_GAMEPAD; nCntGauge++,pPlayer++)
	{
		//計算(今のゲージ量　/　最大ゲージ量)
		Parcent = pPlayer->moveGauge / MaxGauge;

		if (nCntGauge == COUNT_ONE)
		{
			if (Parcent > 90)
			{//赤色
				
			}

			else if (Parcent > 75)
			{//オレンジ色

			}

			else if (Parcent > 50)
			{//青

			}

			else if(Parcent > 25)
			{//水色

			}

			else
			{//白色

			}
		}
	}
}

//描画処理
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGauge[nCntGauge]);

		//ポリゴンの描画	
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
	}
}