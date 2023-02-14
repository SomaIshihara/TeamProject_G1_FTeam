/*==========================================================================================================================================================

//ゲームメニューの選択処理[select_game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "select_game.h"

//マクロ定義
#define SELECTGAMEMODE_NUM_TEX		(1)								/*ゲーム選択のテクスチャ数*/

//グローバル変数宣言
SelectGameMode				g_SelectGameMode;				/*選択されたゲームモード*/
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectGame = NULL;			/*頂点バッファへのポインタ*/
LPDIRECT3DTEXTURE9			g_pTextureSelectGame[SELECTGAMEMODE_NUM_TEX] = {};	/*テクスチャのポインタ*/

//ゲーム選択のテクスチャパス
const char *c_apSelectGameModeTex[SELECTGAMEMODE_NUM_TEX] = {
	"data\\TEXTURE\\selectgame.png",		/*ゲーム選択背景*/
};

//------------------------------------------------
//				ゲーム選択の初期化処理
//------------------------------------------------
void InitSelectGame(void)
{
	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * SELECTGAMEMODE_NUM_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelectGame, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelectGame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntSelect = 0; nCntSelect < SELECTGAMEMODE_NUM_TEX; nCntSelect++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apSelectGameModeTex[nCntSelect], &g_pTextureSelectGame[nCntSelect]);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(			0.0f, 0.0f, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(	SCREEN_WIDTH, 0.0f, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(			0.0f, SCREEN_HEIGHT, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(	SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
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
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffSelectGame->Unlock();
}

//------------------------------------------------
//				ゲーム選択の終了処理
//------------------------------------------------
void UninitSelectGame(void)
{
	//テクスチャ破棄
	for (int nCntTex = 0; nCntTex < SELECTGAMEMODE_NUM_TEX; nCntTex++)
	{
		if (g_pTextureSelectGame[nCntTex] != NULL)
		{
			g_pTextureSelectGame[nCntTex]->Release();
			g_pTextureSelectGame[nCntTex] = NULL;
		}
	}

	//頂点バッファ破棄
	if (g_pVtxBuffSelectGame != NULL)
	{
		g_pVtxBuffSelectGame->Release();
		g_pVtxBuffSelectGame = NULL;
	}
}

//------------------------------------------------
//				ゲーム選択の更新処理
//------------------------------------------------
void UpdateSelectGame(void)
{
	//ゲームモード選択
	DecideGameMode();
}

//ゲームモード選択
void DecideGameMode(void)
{
	//Enterが押された　もしくは　ゲームパッドのSTART or A ボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		//チュートリアル画面に遷移
		SetFade(MODE_GAME);

		//タイトル決定音再生
		PlaySound(SOUND_LABEL_SE_TITLE_DECIDE);
	}
}

//------------------------------------------------
//				ゲーム選択の描画処理
//------------------------------------------------
void DrawSelectGame(void)
{
	//デバイスのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectGame, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntSelect = 0; nCntSelect < SELECTGAMEMODE_NUM_TEX; nCntSelect++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelectGame[nCntSelect]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelect * VTX_MAX, 2);
	}
}