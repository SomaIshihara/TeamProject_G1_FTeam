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
#include "color.h"

//マクロ
#define PLAYER_MOVE_SPEED	(20.0f)		//プレイヤー移動速度
#define PLAYER_POWER_MAX	(1.0f)		//移動の強さの最大値
#define PLAYER_POWER_ADD	(0.01f)		//移動の強さの増加値
#define DUMP_COEF			(0.04f)		//減衰係数
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)	//[デバッグ用]普通に移動するときの移動量
#define DECIMAL_PLACE		(1)			//小数点第何位まで移動していることにするか

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
void MovePlayer(Player *pPlayer);

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
		g_aPlayer[nCntPlayer].moveGauge = 0;


		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = false;
	}

	//1Pのみ使用していることにする
	g_aPlayer[0].bUse = true;
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
	//デバッグ表示
	PrintDebugProc("[パラメータ]\n");

	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			//現在の位置を前回の位置にする
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

			pow(10, DECIMAL_PLACE);
			

			//移動方法（ダッシュ）押して離す
			if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
				&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
			{//もうこれ動いてるって言わないよね（ほぼ動いていない）
				if (GetKeyboardPress(DIK_SPACE) == true)
				{//スペースキーは押された
					g_aPlayer[nCntPlayer].moveGauge = fmodf(g_aPlayer[nCntPlayer].moveGauge + PLAYER_POWER_ADD, PLAYER_POWER_MAX + PLAYER_POWER_ADD);
				}
				else if (GetKeyboardRelease(DIK_SPACE) == true)
				{//SPACEキーが離された
					//進行方向の設定
					g_aPlayer[nCntPlayer].move.x = -sinf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = -cosf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;

					g_aPlayer[nCntPlayer].moveGauge = 0;
				}
			}
			else
			{
				g_aPlayer[nCntPlayer].moveGauge = 0;
			}

			//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
			g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
			g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;

			//当たり判定類
			//一旦なし

			//移動量減衰
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			//[デバッグ用]普通に移動する処理
			MovePlayer(&g_aPlayer[nCntPlayer]);

			//影位置設定
			//一旦なし

			//デバッグ表示（強さ表示）
			PrintDebugProc("[%d]:Power = %f\n", nCntPlayer, g_aPlayer[nCntPlayer].moveGauge);
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

				//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
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

				/*------------------------------------------------------------------
				影の描画		Author:平澤詩苑
				--------------------------------------------------------------------*/
				D3DXMATRIX	mtxShadow;		//シャドウマトリックス
				D3DLIGHT9	light;			//ライト情報
				D3DXVECTOR4	posLight;		//ライトの位置
				D3DXVECTOR3	pos, normal;	//平面上の任意の点、法線ベクトル
				D3DXPLANE	plane;			//平面情報

											//ライトの位置を設定
				pDevice->GetLight(0, &light);
				posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

				//平面情報を生成
				pos = ZERO_SET;
				normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXPlaneFromPointNormal(&plane, &pos, &normal);

				//シャドウマトリックスの初期化
				D3DXMatrixIdentity(&mtxShadow);

				//シャドウマトリックスの作成
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxShadow);

				//シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL *)g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

					//黒色に設定					//自己発光を無くす
					MatCopy.Diffuse = XCOL_BLACK;	MatCopy.Emissive = XCOL_BLACK;

					//マテリアル設定
					pDevice->SetMaterial(&MatCopy);

					//テクスチャ設定
					pDevice->SetTexture(0, NULL);

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
//[デバッグ用]プレイヤーの移動処理
//========================
void MovePlayer(Player *pPlayer)
{
	//モデル移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
		pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
		pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else
	{
		return;
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	pPlayer->pos.x += pPlayer->move.x;
	pPlayer->pos.z += pPlayer->move.z;

	//移動量消す
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}