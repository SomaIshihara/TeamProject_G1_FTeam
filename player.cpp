//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "player.h"
#include "model.h"
#include "input.h"
#include "wall.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>

//マクロ
#define PLAYER_MOVE_SPEED	(3.0f)	//プレイヤー移動速度
#define DUMP_COEF			(0.4f)	//減衰係数

//ダッシュ関連マクロ

//#define ADD_GAUGE			(10.0f)	//ゲージ増加量
//#define MAX_GAUGE			(25.0f)	//ゲージ最大値
//#define GAUGE_BONUS			(15.0f)	//ボーナスゲージ


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

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = true;
	}
}

//========================
//終了処理
//========================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//プレイヤーの数だけ処理する
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//取得した方なのでNULL入れるだけでOK
			g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdatePlayer(void)
{
	float nGauge[MAX_USE_GAMEPAD];

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

			//移動方法（ダッシュ）押して離す
			{
				//nGauge[nCntPlayer] = g_aPlayer[nCntPlayer].movegauge / MAX_GAUGE;

				//if (GetKeyboardPress(DIK_SPACE) == true)
				//{//スペースキーは押された

				//	g_aPlayer[nCntPlayer].movegauge += ADD_GAUGE;

				//	if (g_aPlayer[nCntPlayer].movegauge >= MAX_GAUGE)
				//	{
				//		g_aPlayer[nCntPlayer].movegauge = 0;
				//	}

				//}

				//if (GetKeyboardRelease(DIK_SPACE) == true)
				//{//SPACEキーが離された
				// //進行方向の設定

				//	if (nGauge[nCntPlayer] >= 0.9f)
				//	{
				//		g_aPlayer[nCntPlayer].movegauge += GAUGE_BONUS;
				//	}

				//	g_aPlayer[nCntPlayer].move.x += sinf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].movegauge;
				//	g_aPlayer[nCntPlayer].move.z += cosf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].movegauge;

				//	g_aPlayer[nCntPlayer].movegauge = 0;

				//}

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);


	//プレイヤーの数だけ繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//"プレイヤーの"ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_aPlayer[nCntPlayer].model.aParts[nCntParts].bUse == true)
			{
				D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
				D3DXMATRIX mtxParent;					//親のマトリ

				//仮でオフセットをそのまま使う
				g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayer[nCntPlayer].model.aParts[nCntParts].posOffset;
				g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayer[nCntPlayer].model.aParts[nCntParts].rotOffset;

				//"モデルの"ワールドマトリックス初期化
				D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

				//位置反映
				D3DXMatrixTranslation(&mtxTransModel, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

				//パーツの親マトリ設定
				if (g_aPlayer[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
				{
					mtxParent = g_aPlayer[nCntPlayer].model.aParts[g_aPlayer[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
				}

				//パーツのマトリと親マトリをかけ合わせる
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

				//"プレイヤーの"ワールドマトリックス設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//マテリアルデータへのポインタ取得
				pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					//マテリアル設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャ設定
					pDevice->SetTexture(0, g_aPlayer[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

					//モデル描画
					g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}