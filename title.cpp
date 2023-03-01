/*==========================================================================================================================================================

タイトル画面処理[title.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "title.h"
#include "light.h"
#include "camera.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "meshfield.h"
#include "meshdome.h"
#include "file.h"

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
	bool		bUseButtom;	//タイトル画面のボタン
}Title;

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;					//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureTitle[NUM_TITLE_TEX] = {};	//テクスチャのポインタ
Title						g_Title[NUM_TITLE_TEX];					//タイトルの情報
int							g_select;								//選択番号
NumCamera g_NumTitleCamera = NumCamera_ONLY;		//カメラ初期化の引数
int nCntButtonEffect1, nCntButtonEffect2;

//タイトル画面に使用するアイコンたちのパス
const char *c_apTitleTexName[NUM_TITLE_TEX] = {
	"data/TEXTURE/title000.png",
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

		//ボタンを押したかどうか
		g_Title[nCntTitle].bUseButtom = false;

		g_Title[nCntTitle].type = (TITLE)nCntTitle;
	}

	g_select = TITLE_START;

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();

	InitFile();

	LoadModelViewerFile("data\\model.txt");				// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）

	//ライトの初期化処理
	InitLight();

	//カメラの初期化処理
	InitCamera(g_NumTitleCamera);

	//メッシュドームの初期化処理
	InitMeshDome();

	//ステージの初期化処理
	InitMeshfield();
}

//タイトルの情報読み込み処理
void LoadTitle(void)
{
	FILE *pFile;
	char aData[MAX_CHAR];		//読み込んだ文字を格納

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

	UninitFile();

	//ライトの終了処理
	UninitLight();

	//カメラの終了処理
	UninitCamera();

	//メッシュドームの終了処理
	UninitMeshDome();

	//ステージの終了処理
	UninitMeshfield();

	StopSound(SOUND_LABEL_BGM_TITLE);
}

//------------------------------------------------
//				タイトルの更新処理
//------------------------------------------------
void UpdateTitle(void)
{
	//ボタン点滅のカウンタ初期化
	if (g_Title[TITLE_START].bUseButtom == false)
	{
		nCntButtonEffect1 = 120;
	}
	if (g_Title[TITLE_EXIT].bUseButtom == false)
	{
	    nCntButtonEffect2 = 120;
	}	

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

		if (nCntButtonEffect1 % 2 == 0 && g_Title[TITLE_START].bUseButtom == true)
		{//ボタンを押したときに2Fに一回
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

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

			//ボタンを押した
			g_Title[TITLE_START].bUseButtom = true;

			break;

		case TITLE_EXIT:

			EndProject();

			//ボタンを押した
			g_Title[TITLE_EXIT].bUseButtom = true;

			break;
		}
		
		//タイトル決定音再生
		PlaySound(SOUND_LABEL_SE_TITLE_DECIDE);
	}

	if (g_Title[TITLE_START].bUseButtom == true)
	{
		//カウンタを減らす
		nCntButtonEffect1--;

		if (nCntButtonEffect1 == 0)
		{//カウンタが0を下回った時
			g_Title[TITLE_START].bUseButtom = false;
		}
	}
	if (g_Title[TITLE_EXIT].bUseButtom == true)
	{
		//カウンタを減らす
		nCntButtonEffect2--;

		if (nCntButtonEffect2 == 0)
		{//カウンタが0を下回った時
			g_Title[TITLE_EXIT].bUseButtom = false;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();

	//ライトの更新処理
	UpdateLight();

	//カメラの更新処理
	CameraForTitle();
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

	SetCamera(0);		// １番目のカメラの設定処理	
	DrawMeshDome();		// メッシュドームの描画処理	
	DrawMeshfield();	// ステージの描画処理
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