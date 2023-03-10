//==========================================
//
//プレイヤー選択のタイプ枠プログラム[TypeFrame.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "TypeFrame.h"
#include "input.h"
#include "conversioninput.h"
#include "fade.h"
#include "select_game.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "color.h"

//マクロ
#define FRAME_USE_TEXTURE	(3)				//タイプ枠で使うテクスチャ数
#define STR_USE_TEXTURE		(3)				//タイプ文字で使うテクスチャ数
#define FRAME_SIZE_WIDTH	(312.0f)		//枠のサイズ（幅）
#define FRAME_SIZE_HEIGHT	(108.0f)		//枠のサイズ（高さ）
#define STR_OFFSET_WIDTH	(37.0f)			//枠と文字の位置ずらし（幅）
#define STR_OFFSET_HEIGHT	(29.0f)			//枠と文字の位置ずらし（高さ）
#define STR_SIZE_WIDTH		(238.0f)		//文字のサイズ（幅）
#define STR_SIZE_HEIGHT		(50.0f)			//文字のサイズ（高さ）
#define INPUT_JUDGE			(STICK_MAX / 5)	//スティック入力をしたと判断するスティック移動量
#define MIN_PLAYER			(2)				//プレイヤーの最低人数

//プロトタイプ宣言
bool CheckMinPlayer(void);

//グローバル
PLAYERTYPE g_playerType[MAX_USE_GAMEPAD];

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeFrame;					//タイプ枠の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTypeFrame[FRAME_USE_TEXTURE];		//タイプ枠のテクスチャポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeStr;						//タイプ文字の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTypeStr[STR_USE_TEXTURE];			//タイプ文字のテクスチャポインタ
int g_nSelectNum = 0;

//タイプ枠のファイルパス
const char *c_apFilePathTypeFrame[FRAME_USE_TEXTURE] =
{
	"data\\TEXTURE\\SelPlayer_Frame_02.png",
	"data\\TEXTURE\\SelPlayer_Frame_01.png",
	"data\\TEXTURE\\SelPlayer_Frame_03.png"
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
	D3DXCOLOR(1.0f,0.7f,0.7f,1.0f),
	D3DXCOLOR(0.24f,0.72f,0.92f,1.0f),
	D3DXCOLOR(0.72f,0.9f,0.73f,1.0f),
	D3DXCOLOR(1.0f,0.95f,0.67f,1.0f)
};

//タイプ文字のファイルパス
const char *c_apFilePathTypeStr[STR_USE_TEXTURE] =
{
	"data\\TEXTURE\\SelPlayer_Str_01.png",
	"data\\TEXTURE\\SelPlayer_Str_02.png",
	"data\\TEXTURE\\SelPlayer_Str_03.png"
};

//========================
//初期化処理
//========================
void InitTypeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;	//設定用ポインタ

	//===============
	//変数初期化
	//===============
	for (int nCntType = 0; nCntType < MAX_USE_GAMEPAD; nCntType++)
	{
		g_playerType[nCntType] = PLAYERTYPE_PLAYER;
	}

	//===============
	//テクスチャ・バッファ生成
	//===============
	//タイプ枠
	//テクスチャ読み込み
	for (int nCntTypeFrame = 0; nCntTypeFrame < FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeFrame],
			&g_pTextureTypeFrame[nCntTypeFrame]);
	}

	//頂点バッファの生成(+1は選択中の枠用）
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_USE_GAMEPAD * (FRAME_USE_TEXTURE - 1) + 1),
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
			c_apFilePathTypeStr[nCntTypeStr],
			&g_pTextureTypeStr[nCntTypeStr]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeStr,
		NULL);


	//頂点設定

	//タイプ枠
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	//位置固定枠
	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * (FRAME_USE_TEXTURE - 1); nCntTypeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].x;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].x + FRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].x;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].y + FRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].x + FRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)].y + FRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		//もし偶数（背景）なら色変える
		if (nCntTypeFrame % 2 == 0)
		{
			pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)];
			pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)];
			pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)];
			pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame / (FRAME_USE_TEXTURE - 1)];
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

	//位置可変枠
	//頂点座標の設定
	pVtx[0].pos.x = c_aPosTypeFrame[0].x;
	pVtx[0].pos.y = c_aPosTypeFrame[0].y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_aPosTypeFrame[0].x + FRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_aPosTypeFrame[0].y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_aPosTypeFrame[0].x;
	pVtx[2].pos.y = c_aPosTypeFrame[0].y + FRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_aPosTypeFrame[0].x + FRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_aPosTypeFrame[0].y + FRAME_SIZE_HEIGHT;
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
void UninitTypeFrame(void)
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
void UpdateTypeFrame(void)
{
	VERTEX_2D *pVtx;	//設定用ポインタ

	//プレイヤー選択
	if (GetUseGamepad(0) == true)
	{
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_A) == true || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{//次のプレイヤー
			g_nSelectNum = (g_nSelectNum + 1) % MAX_USE_GAMEPAD;
		}
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{//戻る
			g_nSelectNum = (g_nSelectNum + (MAX_USE_GAMEPAD - 1)) % MAX_USE_GAMEPAD;
		}

		//タイプ選択
		if (GetStick(0, INPUTTYPE_TRIGGER).y == CONVSTICK_DOWN || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{//下に倒した
			//いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);

			//最低人数を下回るならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
		}
		else if (GetStick(0, INPUTTYPE_TRIGGER).y == CONVSTICK_UP || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_UP) == true)
		{//上に倒した
			//いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);

			//最低人数を下回るならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
		}

		//決定
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_START) == true)
		{
			if (GetSelGameMode() == SelGameMode_PVP)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_PvP(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
			else if (GetSelGameMode() == SelGameMode_HDR)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_HDR(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_LEFT) == true)
		{//左に倒した
			g_nSelectNum = (g_nSelectNum + (MAX_USE_GAMEPAD - 1)) % MAX_USE_GAMEPAD;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{//右に倒した
			g_nSelectNum = (g_nSelectNum + 1) % MAX_USE_GAMEPAD;
		}

		//タイプ選択
		if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//下に倒した
			//いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);

			//最低人数を下回るならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
		}
		else if (GetKeyboardTrigger(DIK_UP) == true)
		{//上に倒した
			 //いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);

			//最低人数を下回るならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
		}

		//決定
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			if (GetSelGameMode() == SelGameMode_PVP)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_PvP(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
			else if (GetSelGameMode() == SelGameMode_HDR)
			{
				SetFade(MODE_RaceGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_HDR(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
		}
	}

	//タイプ枠頂点設定
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD; nCntTypeFrame++, pVtx += 8)	//偶数のみ変更のため8つずらしている
	{
		//頂点カラー
		if (g_playerType[nCntTypeFrame] == PLAYERTYPE_PLAYER)
		{//プレイヤーなら色付ける
			pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame];
		}
		else
		{//COM・使用しないなら灰色
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}

	//位置可変部
	//頂点座標の設定
	pVtx[0].pos.x = c_aPosTypeFrame[g_nSelectNum].x;
	pVtx[0].pos.y = c_aPosTypeFrame[g_nSelectNum].y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_aPosTypeFrame[g_nSelectNum].x + FRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_aPosTypeFrame[g_nSelectNum].y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_aPosTypeFrame[g_nSelectNum].x;
	pVtx[2].pos.y = c_aPosTypeFrame[g_nSelectNum].y + FRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_aPosTypeFrame[g_nSelectNum].x + FRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_aPosTypeFrame[g_nSelectNum].y + FRAME_SIZE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	g_pVtxbuffTypeFrame->Unlock();
}

//========================
//描画処理
//========================
void DrawTypeFrame(void)
{
	int nCntTypeFrame;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//タイプ枠
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTypeFrame, 0, sizeof(VERTEX_2D));

	for (nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * (FRAME_USE_TEXTURE - 1); nCntTypeFrame++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTypeFrame[nCntTypeFrame % (FRAME_USE_TEXTURE - 1)]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);
	}

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureTypeFrame[2]);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);


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

//========================
//プレイヤーが最低人数を下回らないか確認する処理
//========================
bool CheckMinPlayer(void)
{
	int nUsePlayer = 0;

	//現在の使用人数確認
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_playerType[nCntPlayer] != PLAYERTYPE_NONE)
		{
			nUsePlayer++;
		}
	}

	//下回らない場合は減らしてもOK
	return nUsePlayer >= MIN_PLAYER ? true : false;
}

//========================
//プレイヤー使用取得処理
//========================
bool GetUsePlayer(int nPlayerNum)
{
	return g_playerType[nPlayerNum] != PLAYERTYPE_NONE ? true : false;
}