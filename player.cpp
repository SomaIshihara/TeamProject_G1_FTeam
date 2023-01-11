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
Player g_player;
int g_nIdxShadow = -1;

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	g_player.pos = ZERO_SET;
	g_player.posOld = g_player.pos;
	g_player.move = ZERO_SET;
	g_player.rot = ZERO_SET;
	
	g_player.nNumModel = 0;

	g_nIdxShadow = -1;

	//対角線の長さ・角度
	//一旦なし
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
	//現在の位置を前回の位置にする
	g_player.posOld = g_player.pos;

	//カメラ向きに応じてプレイヤーの向き旋回
	g_player.rot.y = -(float)fmod(GetCamera()->rot.y + 1.0f * D3DX_PI + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;

	//モデル移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_W + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_S + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_A + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
		g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_D + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
		g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else
	{
		
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	g_player.pos.x += g_player.move.x;
	g_player.pos.z += g_player.move.z;

	//当たり判定類
	//一旦なし

	GetCamera()->posV.x += g_player.pos.x - g_player.posOld.x;
	GetCamera()->posV.z += g_player.pos.z - g_player.posOld.z;
	GetCamera()->posR.x += g_player.pos.x - g_player.posOld.x;
	GetCamera()->posR.z += g_player.pos.z - g_player.posOld.z;

	//移動量減衰
	g_player.move.x += (0 - g_player.move.x) * DUMP_COEF;
	g_player.move.z += (0 - g_player.move.z) * DUMP_COEF;

	//影位置設定
	//一旦なし
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
	return &g_player;
}