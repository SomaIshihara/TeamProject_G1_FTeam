/*==========================================================================================================================================================

//ゲームメニューの選択処理[select_game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "fade.h"
#include "conversioninput.h"
#include "sound.h"
#include "select_game.h"

//マクロ定義
#define SELECTGAME_INFO		"data\\CSV\\SelectGame.csv"						/*ゲーム選択の情報*/

//グローバル変数宣言
SelectGame					g_SelectGame[SelectGameMenu_MAX];				/*ゲーム選択の情報*/
SelectGameMenu				g_SelectGameMenu = SelectGameMenu_PVP;			/*選択されたゲームメニュー*/
SelectGameMode				g_SelectGameMode = SelectGameMode_PVP;			/*選択されたゲームモード*/
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectGame = NULL;					/*頂点バッファへのポインタ*/
LPDIRECT3DTEXTURE9			g_pTextureSelectGame[SelectGameMenu_MAX] = {};	/*テクスチャのポインタ*/

//ゲーム選択のテクスチャパス
const char *c_apSelectGameModeTex[SelectGameMenu_MAX] = {
	"data\\TEXTURE\\BG_selectgame.png",		/*ゲーム選択背景*/
	"data\\TEXTURE\\UI_PVP.png",			/*UI_PVP*/
	"data\\TEXTURE\\UI_HipDropRace.png",	/*UI_ヒップドロップレース*/
};

//------------------------------------------------
//				ゲーム選択の初期化処理
//------------------------------------------------
void InitSelectGame(void)
{
	//選択メニュー情報の初期化
	LoadSelectGame();
	g_SelectGameMenu = SelectGameMenu_PVP;
	g_SelectGameMode = SelectGameMode_PVP;

	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * SelectGameMenu_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelectGame, NULL);

	//テクスチャ読み込み
	for (int nCntTex = 0; nCntTex < SelectGameMenu_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apSelectGameModeTex[nCntTex], &g_pTextureSelectGame[nCntTex]);
	}

	//頂点情報設定
	SetVertexSelectGame();
}

//ゲーム選択の情報読み込み処理
void LoadSelectGame(void)
{
	FILE *pFile;
	char aData[MAX_CHAR];		//読み込んだ文字を格納

	pFile = fopen(SELECTGAME_INFO, "r");

	//ファイルを読み込めた
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//最初のセルタイトル読み込み

		for (int nCntSelect = 0; nCntSelect < SelectGameMenu_MAX; nCntSelect++)
		{
			fscanf(pFile, "%s", &aData[0]);	//一行読み込み

			g_SelectGame[nCntSelect].pos.x = fatof(strtok(&aData[0], ","));		// 表示位置初期化
			g_SelectGame[nCntSelect].pos.y = fatof(strtok(NULL, ","));			// 表示位置初期化

			g_SelectGame[nCntSelect].fWidth = fatof(strtok(NULL, ","));			// 幅初期化
			g_SelectGame[nCntSelect].fHeight = fatof(strtok(NULL, ","));		// 幅初期化
			g_SelectGame[nCntSelect].bChoice = false;							// 選択されていない
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//ゲーム選択メニューの頂点情報設定処理
void SetVertexSelectGame(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelectGame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntSelect = 0; nCntSelect < SelectGameMenu_MAX; nCntSelect++)
	{
		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x - g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y - g_SelectGame[nCntSelect].fHeight, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x + g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y - g_SelectGame[nCntSelect].fHeight, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x - g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y + g_SelectGame[nCntSelect].fHeight, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x + g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y + g_SelectGame[nCntSelect].fHeight, NIL_F);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//頂点カラーの設定
		//カウンターが選択されているメニュー番号 または 背景と同じ
		if (nCntSelect == g_SelectGameMenu || nCntSelect == SelectGameMenu_BG)
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}
		else
		{
			pVtx[VTX_LE_UP].col = XCOL_NO_SELECT;
			pVtx[VTX_RI_UP].col = XCOL_NO_SELECT;
			pVtx[VTX_LE_DO].col = XCOL_NO_SELECT;
			pVtx[VTX_RI_DO].col = XCOL_NO_SELECT;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += VTX_MAX;
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
	for (int nCntTex = 0; nCntTex < SelectGameMenu_MAX; nCntTex++)
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
	//ゲームモードの項目移動
	ChooseGameMode();

	//頂点情報設定
	SetVertexSelectGame();

	//ゲームモード選択
	DecideGameMode();
}

//ゲームモードを選ぶ
void ChooseGameMode(void)
{
	//右矢印が押された　もしくは　ゲームパッドの右十字キー　が押された　もしくは　左スティックが右に倒された
	if ((GetKeyboardTrigger(DIK_RIGHT) || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_RIGHT) || GetStick(0, INPUTTYPE_TRIGGER) == CONVSTICK_RIGHT ))
	{
		switch (g_SelectGameMenu)
		{
			//PVPが選択されている場合  もしくはバグで、何にも選択されていなかった場合
		case SelectGameMenu_PVP:
		default:
			g_SelectGameMenu = SelectGameMenu_HDR;	//選択されているメニューを「ヒップドロップレース」に
			break;

			//ヒップドロップレースが選択されている場合
		case SelectGameMenu_HDR:
			g_SelectGameMenu = SelectGameMenu_PVP;	//選択されているメニューを「PVP」に
			break;
		}
	}

	//左矢印が押された　もしくは　ゲームパッドの左十字キーが押された
	if ((GetKeyboardTrigger(DIK_LEFT) || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_LEFT) || GetStick(0, INPUTTYPE_TRIGGER) == CONVSTICK_LEFT))
	{
		switch (g_SelectGameMenu)
		{
			//PVPが選択されている場合  もしくはバグで、何にも選択されていなかった場合
		case SelectGameMenu_PVP:
		default:
			g_SelectGameMenu = SelectGameMenu_HDR;	//選択されているメニューを「ヒップドロップレース」に
			break;

			//ヒップドロップレースが選択されている場合
		case SelectGameMenu_HDR:
			g_SelectGameMenu = SelectGameMenu_PVP;	//選択されているメニューを「PVP」に
			break;
		}
	}
}

//ゲームモード選択
void DecideGameMode(void)
{
	//Enterが押された　もしくは　ゲームパッドのSTART or A ボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		switch (g_SelectGameMenu)
		{
			//PVPが選択されている場合  もしくはバグで、何にも選択されていなかった場合
		case SelectGameMenu_PVP:

			SetFade(MODE_PvPGAME);					//チュートリアル画面に遷移
			g_SelectGameMode = SelectGameMode_PVP;	//PVPゲームを選択

			break;

			//ヒップドロップレースが選択されている場合
		case SelectGameMenu_HDR:

			SetFade(MODE_RaceGAME);					//チュートリアル画面に遷移
			g_SelectGameMode = SelectGameMode_HDR;	//ヒップドロップレースゲームを選択

			break;
		}

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

	for (int nCntSelect = 0; nCntSelect < SelectGameMenu_MAX; nCntSelect++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelectGame[nCntSelect]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelect * VTX_MAX, 2);
	}
}