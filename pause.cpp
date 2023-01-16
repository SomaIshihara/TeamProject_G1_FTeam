//==========================================
//
//ポーズプログラム[meshfield.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "player.h"
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"

//マクロ定義
#define NUM_TEXTURE				(6)									//ポーズ画像のファイル名
#define NUM_BUTTON				(2)									//ボタンの数
#define COUNT_BUTTON			(15)								//ボタンのカウント
#define HALF_WIDTH				(640.0f)							//半分の横幅
#define HALF_HEIGHT				(360.0f)							//半分の高さ
#define UI_WIDTH				(200.0f)							//UIの横幅
#define UI_HEIGHT				(70.0f)								//UIの高さ
#define DIRE_WIDTH_UP			(40.0f)								//上や印の幅
#define DIRE_HEIGHT_UP			(40.0f)								//上や印の高さ
#define DIRE_WIDTH_DOWN			(-40.0f)							//下や印の幅
#define DIRE_HEIGHT_DOWN		(-40.0f)							//下や印の高さ
#define CENTER_POS_MENU			(D3DXVECTOR3(640.0f,360.0f,0.0f))	//メニューの中心の位置
#define CENTER_POS_UI			(D3DXVECTOR3(640.0f,440.0f,0.0f))	//UIの中心の位置
#define CENTER_POS_DIRE_UP		(D3DXVECTOR3(640.0f,340.0f,0.0f))	//や印の中心の位置
#define CENTER_POS_DIRE_DOWN	(D3DXVECTOR3(640.0f,540.0f,0.0f))	//や印の中心の位置

//ファイルパス
const char *c_pFileNamePause[] =
{
	"data/TEXTURE/pause.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
	"data/TEXTURE/quit.png",
	"data/TEXTURE/sankaku.png",
	"data/TEXTURE/sankaku.png"
};

//ポーズ画面のUIの幅
const D3DXVECTOR3 g_CenterPos[] = {
	CENTER_POS_MENU,								//ポーズ画面の位置
	CENTER_POS_UI,									//コンテニューの位置
	CENTER_POS_UI,									//リトライの位置
	CENTER_POS_UI,									//終了の位置
	CENTER_POS_DIRE_UP,
	CENTER_POS_DIRE_DOWN
};

//ポーズ画面のUIの幅
const float g_Width[] = {
	HALF_WIDTH,										//ポーズ画面の幅
	UI_WIDTH,										//コンテニューの幅
	UI_WIDTH,										//リトライの幅
	UI_WIDTH,										//終了の幅
	DIRE_WIDTH_UP,
	DIRE_WIDTH_DOWN
};

//ポーズ画面のUIの高さ
const float g_Height[] = {
	HALF_HEIGHT,									//ポーズ画面の幅
	UI_HEIGHT,										//コンテニューの幅
	UI_HEIGHT,										//リトライの幅
	UI_HEIGHT,										//終了の幅
	DIRE_HEIGHT_UP,
	DIRE_HEIGHT_DOWN
};

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_TEXTURE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//頂点バッファへのポインタ
PAUSE g_Pause;
Pause g_aPause[NUM_TEXTURE];
int g_nCnt[NUM_BUTTON];
bool g_ButtonUse[NUM_BUTTON];

//=================================
//ポーズの初期化処理
//=================================
void InitPause(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TEXTURE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_pFileNamePause[nCnt], &g_pTexturePause[nCnt]);
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//コンテニューに設定
	g_Pause = PAUSE_CONTINUE;

	//初期ボタンカウント設定
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		g_nCnt[nCnt] = COUNT_BUTTON;
	}

	//各情報読み込み
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++,pVtx +=4)
	{
		//位置の設定
		g_aPause[nCnt].pos = g_CenterPos[nCnt];

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x - g_Width[nCnt], g_aPause[nCnt].pos.y - g_Height[nCnt], NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x + g_Width[nCnt], g_aPause[nCnt].pos.y - g_Height[nCnt], NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x - g_Width[nCnt], g_aPause[nCnt].pos.y + g_Height[nCnt], NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x + g_Width[nCnt], g_aPause[nCnt].pos.y + g_Height[nCnt], NIL_F);

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
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}
//=================================
//ポーズの終了処理
//=================================
void UninitPause(void) 
{
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
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
	//ポーズの下方向選択処理
	SelectDownPause();
	
	//ポーズの上方向選択処理
	SelectUpPause();

	//色の変更のカウントダウン
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		if (g_ButtonUse[nCnt] == true)
		{
			g_nCnt[nCnt]--;
		}
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//選択項目の表示と矢印の色変更
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++, pVtx += 4)
	{
		if (g_Pause == nCnt)
		{
			//頂点カラーの設定
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}
		else if(nCnt != 0 && nCnt <= 3)
		{
			//頂点カラーの設定
			pVtx[VTX_LE_UP].col = XCOL_NONE;
			pVtx[VTX_RI_UP].col = XCOL_NONE;
			pVtx[VTX_LE_DO].col = XCOL_NONE;
			pVtx[VTX_RI_DO].col = XCOL_NONE;
		}
		else if (nCnt >= 4)
		{
			if (g_nCnt[1] <= 0 && nCnt == 4)
			{
				//白に設定
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//ボタンを使用していない状態にする
				g_ButtonUse[1] = false;

				//カウントの再設定
				g_nCnt[1] = COUNT_BUTTON;
			}
			else if (g_nCnt[0] <= 0 && nCnt == 5)
			{
				//白に設定
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//ボタンを使用していない状態にする
				g_ButtonUse[0] = false;

				//カウントの再設定
				g_nCnt[0] = COUNT_BUTTON;
			}
		}
	}
	
	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();

	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			break;

		case PAUSE_RETRY:
			//モード設定（ゲーム画面に遷移)
			SetFade(MODE_GAME);
			break;

		case PAUSE_QUIT:
			//タイトル画面に遷移
			SetFade(MODE_TITLE);
			break;
		}
	}
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

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
//=================================
//ポーズの下方向選択処理
//=================================
void SelectDownPause(void)
{
	//キーボードの下方向キー　か　ゲームパッドの　十字下ボタン　が押された
	if (GetKeyboardTrigger(DIK_DOWN) == true)
	{
		//ボタンを使用している状態にする
		g_ButtonUse[0] = true;

		//カウントの再設定
		g_nCnt[0] = COUNT_BUTTON;

		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += 20;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = XCOL_YELLOW;
		pVtx[VTX_RI_UP].col = XCOL_YELLOW;
		pVtx[VTX_LE_DO].col = XCOL_YELLOW;
		pVtx[VTX_RI_DO].col = XCOL_YELLOW;

		//頂点バッファをロックする
		g_pVtxBuffPause->Unlock();

		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			//リトライボタンに設定
			g_Pause = PAUSE_RETRY;

			break;

		case PAUSE_RETRY:
			
			//終了ボタンに設定
			g_Pause = PAUSE_QUIT;

			break;

		case PAUSE_QUIT:
			
			//続けるボタンに設定
			g_Pause = PAUSE_CONTINUE;

			break;
		}
	}
}
//=================================
//ポーズの上方向選択処理
//=================================
void SelectUpPause(void)
{
	//キーボードの上方向キー　か　ゲームパッドの　十字上ボタン　が押された
	if (GetKeyboardTrigger(DIK_UP) == true)
	{
		//ボタンを使用している状態にする
		g_ButtonUse[1] = true;

		//カウントの再設定
		g_nCnt[1] = COUNT_BUTTON;

		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += 16;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = XCOL_YELLOW;
		pVtx[VTX_RI_UP].col = XCOL_YELLOW;
		pVtx[VTX_LE_DO].col = XCOL_YELLOW;
		pVtx[VTX_RI_DO].col = XCOL_YELLOW;

		//頂点バッファをロックする
		g_pVtxBuffPause->Unlock();

		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			//終了ボタンに設定
			g_Pause = PAUSE_QUIT;

			break;

		case PAUSE_RETRY:

			//続けるボタンに設定
			g_Pause = PAUSE_CONTINUE;

			break;

		case PAUSE_QUIT:

			//リトライボタンに設定
			g_Pause = PAUSE_RETRY;

			break;
		}
	}
}
//=================================
//ポーズの設定処理
//=================================
void SetPause(PAUSE Pause)
{
	g_Pause = Pause;
}
//=================================
//ポーズの取得
//=================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}