/*==========================================================================================================================================================

タイトル画面処理[title.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "title.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

#define NUM_TITLE_TEX	(3)									// タイトルに使う画像の数
#define TITLE_INFOFILE	"data/CSV/title.csv"				// タイトルの情報が入ったファイル名

#define TITLE_POS		(D3DXVECTOR3(NIL_F, NIL_F, NIL_F))	//タイトルの位置
#define TITLE_WIDTH		(510.0f)							//タイトルの幅
#define TITLE_HEIGHT	(215.0f)							//タイトルの高さ

//タイトルの情報構造体
typedef struct
{
	D3DXVECTOR2 pos;		//位置
	float		fWidth;		//幅
	float		fHeight;	//高さ
	TITLE		type;		//種類
}Title;

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;					//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureTitle[NUM_TITLE_TEX] = {};	//テクスチャのポインタ
Title						g_Title[NUM_TITLE_TEX];					//タイトルの情報
int							g_select;								//選択番号

//タイトル画面に使用するアイコンたちのパス
const char *c_apTitleTexName[NUM_TITLE_TEX] = {
	"data/TEXTURE/title.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
};

//------------------------------------------------
//				タイトルの初期化処理
//------------------------------------------------
void InitTitle(void)
{
	PlaySound(SOUND_LABEL_BGM_TITLE);

	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TITLE_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void* *)&pVtx, 0);

	//タイトルの情報読み込み
	LoadTitle();

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++, pVtx += VTX_MAX)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apTitleTexName[nCntTitle], &g_pTextureTitle[nCntTitle]);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x - g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y - g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x + g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y - g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x - g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y + g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x + g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y + g_Title[nCntTitle].fHeight, NIL_F);
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
			if (nCntTitle >= 1)
			{
				pVtx[VTX_LE_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[VTX_RI_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[VTX_LE_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[VTX_RI_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
			else
			{
				pVtx[VTX_LE_UP].col = XCOL_WHITE;
				pVtx[VTX_RI_UP].col = XCOL_WHITE;
				pVtx[VTX_LE_DO].col = XCOL_WHITE;
				pVtx[VTX_RI_DO].col = XCOL_WHITE;
			}
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		g_Title[nCntTitle].type = (TITLE)nCntTitle;
	}

	g_select = TITLE_EXIT;

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();
}

//タイトルの情報読み込み処理
void LoadTitle(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(TITLE_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//最初のセルタイトル読み込み

		for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
		{
			fscanf(pFile, "%s", &aData[0]);	//一行読み込み

			g_Title[nCntTitle].pos.x = fatof(strtok(&aData[0], ","));		// 表示位置初期化
			g_Title[nCntTitle].pos.y = fatof(strtok(NULL, ","));			// 表示位置初期化

			g_Title[nCntTitle].fWidth = fatof(strtok(NULL, ","));			// 幅初期化
			g_Title[nCntTitle].fHeight = fatof(strtok(NULL, ","));			// 幅初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//------------------------------------------------
//				タイトルの終了処理
//------------------------------------------------
void UninitTitle(void)
{
	//テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	StopSound(SOUND_LABEL_BGM_TITLE);
}

//------------------------------------------------
//				タイトルの更新処理
//------------------------------------------------
void UpdateTitle(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void* *)&pVtx, 0);

	
		//
		DirecUpTitle(0);

		//
		DirecDownTitle(0);
	
	switch (g_select)
	{
	case TITLE_START:
		
		pVtx += g_select * VTX_MAX;

		pVtx[VTX_LE_UP].col = XCOL_WHITE;
		pVtx[VTX_RI_UP].col = XCOL_WHITE;
		pVtx[VTX_LE_DO].col = XCOL_WHITE;
		pVtx[VTX_RI_DO].col = XCOL_WHITE;

		pVtx += VTX_MAX;

		pVtx[VTX_LE_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		break;

	case TITLE_EXIT:

		pVtx += g_select * VTX_MAX;

		pVtx[VTX_LE_UP].col = XCOL_WHITE;
		pVtx[VTX_RI_UP].col = XCOL_WHITE;
		pVtx[VTX_LE_DO].col = XCOL_WHITE;
		pVtx[VTX_RI_DO].col = XCOL_WHITE;

		pVtx -= VTX_MAX;

		pVtx[VTX_LE_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		break;
	}

	//Enterが押された　もしくは　ゲームパッドのSTART or A ボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		switch (g_select)
		{
		case TITLE_START:

			//ゲーム選択画面に遷移
			SetFade(MODE_SELECTGAME);

			break;

		case TITLE_EXIT:

			break;
		}
		
		//タイトル決定音再生
		PlaySound(SOUND_LABEL_SE_TITLE_DECIDE);
	}

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();
}

//------------------------------------------------
//				タイトルの描画処理
//------------------------------------------------
void DrawTitle(void)
{
	//デバイスのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * VTX_MAX, 2);
	}
}
//
void DirecUpTitle(int NumPad)
{
	if (GetKeyboardTrigger(DIK_UP) == true || GetGamepadTrigger(NumPad, XINPUT_GAMEPAD_DPAD_UP) == true)
	{
		switch (g_select)
		{
		case TITLE_START:

			g_select = TITLE_EXIT;

			break;

		case TITLE_EXIT:

			g_select = TITLE_START;

			break;
		}
	}
}
//
void DirecDownTitle(int NumPad)
{
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetGamepadTrigger(NumPad, XINPUT_GAMEPAD_DPAD_DOWN) == true)
	{
		switch (g_select)
		{
		case TITLE_START:

			g_select = TITLE_EXIT;

			break;

		case TITLE_EXIT:

			g_select = TITLE_START;

			break;
		}
	}
}