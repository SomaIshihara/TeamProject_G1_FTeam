//==========================================
//
//プレイヤー選択プログラム[SelPlayer.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "SelPlayer.h"
#include "conversioninput.h"
#include "model.h"

//マクロ
#define FRAME_USE_TEXTURE	(2)			//タイプ枠で使うテクスチャ数
#define STR_USE_TEXTURE		(3)			//タイプ文字で使うテクスチャ数
#define FRAME_SIZE_WIDTH	(312.0f)	//枠のサイズ（幅）
#define FRAME_SIZE_HEIGHT	(108.0f)	//枠のサイズ（高さ）
#define STR_OFFSET_WIDTH	(37.0f)		//枠と文字の位置ずらし（幅）
#define STR_OFFSET_HEIGHT	(29.0f)		//枠と文字の位置ずらし（高さ）
#define STR_SIZE_WIDTH		(238.0f)	//文字のサイズ（幅）
#define STR_SIZE_HEIGHT		(50.0f)		//文字のサイズ（高さ）

//グローバル
PLAYERTYPE g_playerType[MAX_USE_GAMEPAD];
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeFrame;					//タイプ枠の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTypeFrame[FRAME_USE_TEXTURE];		//タイプ枠のテクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeStr;						//タイプ文字の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTypeStr[STR_USE_TEXTURE];			//タイプ文字のテクスチャポインタ

//タイプ枠のファイルパス
const char *c_apFilePathTypeFrame[FRAME_USE_TEXTURE] =
{
	"SelPlayer_Frame_01.png",
	"SelPlayer_Frame_02.png"
};

//タイプ枠の位置
const D3DXVECTOR3 c_aPosTypeFrame[MAX_USE_GAMEPAD] =
{
	D3DXVECTOR3(2.5f,550.0f,0.0f),
	D3DXVECTOR3(323.5f,550.0f,0.0f),
	D3DXVECTOR3(634.5f,550.0f,0.0f),
	D3DXVECTOR3(955.5f,550.0f,0.0f)
};

//タイプ枠背景の色
const D3DXCOLOR c_aColTypeFrameBG[MAX_USE_GAMEPAD] =
{
	D3DXCOLOR(1.0f,0.5f,0.5f,1.0f),
	D3DXCOLOR(0.24f,0.72f,0.92f,1.0f),
	D3DXCOLOR(0.72f,0.9f,0.73f,1.0f),
	D3DXCOLOR(1.0f,0.95f,0.67f,1.0f)
};

//タイプ文字のファイルパス
const char *c_apFilePathTypeStr[STR_USE_TEXTURE] =
{
	"SelPlayer_Str_01.png",
	"SelPlayer_Str_02.png",
	"SelPlayer_Str_03.png"
};

//========================
//初期化処理
//========================
void InitSelPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;	//設定用ポインタ

	//タイプ枠
	//テクスチャ読み込み
	for (int nCntTypeFrame = 0; nCntTypeFrame < FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeFrame],
			&g_pTextureTypeFrame[nCntTypeFrame]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD * FRAME_USE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeFrame,
		NULL);

	//タイプ文字
	//テクスチャ読み込み
	for (int nCntTypeStr = 0; nCntTypeStr < STR_USE_TEXTURE; nCntTypeStr++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeStr],
			&g_pTextureTypeFrame[nCntTypeStr]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeFrame,
		NULL);


	//頂点設定

	//タイプ枠
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * FRAME_USE_TEXTURE; nCntTypeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x + FRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y + FRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x + FRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y + FRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		//もし偶数（背景）なら色変える
		if (nCntTypeFrame / 2 == 0)
		{
			pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
			pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
			pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
			pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffTypeFrame->Unlock();


	//文字
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeStr->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + STR_OFFSET_WIDTH;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + STR_OFFSET_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + FRAME_SIZE_WIDTH - STR_OFFSET_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + STR_OFFSET_HEIGHT;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + STR_OFFSET_WIDTH;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + FRAME_SIZE_HEIGHT - STR_OFFSET_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + FRAME_SIZE_WIDTH - STR_OFFSET_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + FRAME_SIZE_HEIGHT - STR_OFFSET_HEIGHT;
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
	g_pVtxbuffTypeStr->Unlock();
}

//========================
//終了処理
//========================
void UninitSelPlayer(void)
{
	//ゲージ枠
	//テクスチャの破棄
	for (int nCntTypeFrame = 0; nCntTypeFrame < FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		if (g_pTextureTypeFrame[nCntTypeFrame] != NULL)
		{
			g_pTextureTypeFrame[nCntTypeFrame]->Release();
			g_pTextureTypeFrame[nCntTypeFrame] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffTypeFrame != NULL)
	{
		g_pVtxbuffTypeFrame->Release();
		g_pVtxbuffTypeFrame = NULL;
	}


	//アイコン
	//テクスチャの破棄
	for (int nCntTypeStr = 0; nCntTypeStr < FRAME_USE_TEXTURE; nCntTypeStr++)
	{
		if (g_pTextureTypeStr[nCntTypeStr] != NULL)
		{
			g_pTextureTypeStr[nCntTypeStr]->Release();
			g_pTextureTypeStr[nCntTypeStr] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffTypeStr != NULL)
	{
		g_pVtxbuffTypeStr->Release();
		g_pVtxbuffTypeStr = NULL;
	}
}

//========================
//更新処理
//========================
void UpdateSelPlayer(void)
{
	
}

//========================
//描画処理
//========================
void DrawSelPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

												//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//タイプ枠
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTypeFrame, 0, sizeof(VERTEX_2D));

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		//もし偶数（背景）なら加算合成にする
		if (nCntTypeFrame / 2 == 0)
		{//アルファブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTypeFrame[nCntTypeFrame % FRAME_USE_TEXTURE]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);

		//元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}


	//アイコン
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTypeStr, 0, sizeof(VERTEX_2D));

	for (int nCntTypeStr = 0; nCntTypeStr < MAX_USE_GAMEPAD; nCntTypeStr++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTypeStr[g_playerType[nCntTypeStr]]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeStr, 2);
	}
}