//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "wall.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>

//マクロ
#define PLAYER_MOVE_SPEED	(7.0f)	//プレイヤー移動速度
#define DUMP_COEF			(0.4f)	//減衰係数
#define BULLET_SPEED		(20.0f)	//弾の速さ

//向き
#define ROT_WA	(-0.75f * D3DX_PI)	//左上
#define ROT_WD	(0.75f * D3DX_PI)	//右上
#define ROT_SA	(-0.25f * D3DX_PI)	//左下
#define ROT_SD	(0.25f * D3DX_PI)	//右下
#define ROT_W	(1.0f * D3DX_PI)	//上
#define ROT_A	(-0.5f * D3DX_PI)	//左
#define ROT_S	(0.0f * D3DX_PI)	//下
#define ROT_D	(0.5f * D3DX_PI)	//右

//プロト
//なし

//グローバル変数
Player g_aPlayer[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = ZERO_SET;
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = ZERO_SET;
		g_aPlayer[nCntPlayer].rot = ZERO_SET;

		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;

		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = false;
	}
}

//========================
//終了処理
//========================
void UninitPlayer(void)
{
	
}

//========================
//更新処理
//========================
void UpdatePlayer(void)
{
	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			//現在の位置を前回の位置にする
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

			//モデル移動
			if (GetKeyboardPress(DIK_W) == true)
			{
				if (GetKeyboardPress(DIK_A) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else if (GetKeyboardPress(DIK_D) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_W + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{
				if (GetKeyboardPress(DIK_A) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else if (GetKeyboardPress(DIK_D) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_S + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
			}
			else if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_A + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
				g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_D + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
				g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			}

			//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
			g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
			g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;

			//当たり判定類
			//一旦なし

			//移動量減衰
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			//影位置設定
			//一旦なし
		}
	}
}

//========================
//描画処理
//========================
void DrawPlayer(void)
{
	
}

//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}