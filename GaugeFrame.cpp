//==========================================
//
//ゲージ類の枠関係プログラム[GaugeFrame.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "GaugeFrame.h"
#include "pvp_game.h"
#include "input.h"

//マクロ
#define GAUGE_FRAME_SIZE_WIDTH	(232)	//プレイヤーのポリゴンサイズ（幅）
#define GAUGE_FRAME_SIZE_HEIGHT	(140)	//プレイヤーのポリゴンサイズ（高さ）

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGaugeFrame;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureGaugeFrame[MAX_USE_GAMEPAD];				//テクスチャポインタ

//ファイルパス
const char* c_apFilePathGaugeFrame[MAX_USE_GAMEPAD] =
{
	"data\\TEXTURE\\PlayerFrame_01.png",
	"data\\TEXTURE\\PlayerFrame_02.png",
	"data\\TEXTURE\\PlayerFrame_03.png",
	"data\\TEXTURE\\PlayerFrame_04.png"
};

const D3DXVECTOR3		g_PosGaugeFrame[MAX_USE_GAMEPAD] =				//ゲージの位置
{//参考:X = gaugeのX - 17.0f, Y = gaugeのY - 80.0f
	D3DXVECTOR3(83.0f,570.0f,0.0f),
	D3DXVECTOR3(383.0f,570.0f,0.0f),
	D3DXVECTOR3(683.0f,570.0f,0.0f),
	D3DXVECTOR3(983.0f,570.0f,0.0f),
};

//========================
//プレイヤー初期化処理
//========================
void InitGaugeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathGaugeFrame[nCntGaugeFrame],
			&g_pTextureGaugeFrame[nCntGaugeFrame]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffGaugeFrame,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffGaugeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_PosGaugeFrame[nCntGaugeFrame].x;
		pVtx[0].pos.y = g_PosGaugeFrame[nCntGaugeFrame].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_PosGaugeFrame[nCntGaugeFrame].x + GAUGE_FRAME_SIZE_WIDTH;
		pVtx[1].pos.y = g_PosGaugeFrame[nCntGaugeFrame].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_PosGaugeFrame[nCntGaugeFrame].x;
		pVtx[2].pos.y = g_PosGaugeFrame[nCntGaugeFrame].y + GAUGE_FRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_PosGaugeFrame[nCntGaugeFrame].x + GAUGE_FRAME_SIZE_WIDTH;
		pVtx[3].pos.y = g_PosGaugeFrame[nCntGaugeFrame].y + GAUGE_FRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffGaugeFrame->Unlock();
}

//========================
//プレイヤー終了処理
//========================
void UninitGaugeFrame(void)
{
	//テクスチャの破棄
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		if (g_pTextureGaugeFrame[nCntGaugeFrame] != NULL)
		{
			g_pTextureGaugeFrame[nCntGaugeFrame]->Release();
			g_pTextureGaugeFrame[nCntGaugeFrame] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffGaugeFrame != NULL)
	{
		g_pVtxbuffGaugeFrame->Release();
		g_pVtxbuffGaugeFrame = NULL;
	}
}

//========================
//プレイヤー更新処理
//========================
void UpdateGaugeFrame(void)
{
	//無
}

//========================
//プレイヤー描画処理
//========================
void DrawGaugeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffGaugeFrame, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureGaugeFrame[nCntGaugeFrame]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGaugeFrame, 2);
	}
}