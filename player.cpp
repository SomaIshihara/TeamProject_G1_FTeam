//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "model.h"
#include "input.h"
#include "wall.h"
#include "score.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "Gauge.h"
#include "sound.h"
#include "effect.h"

//マクロ
#define PLAYER_MOVE_SPEED	(20.0f)		//プレイヤー移動速度
#define PLAYER_JUMP_SPEED	(7.7f)		//プレイヤージャンプ速度

#define PLAYER_HIPDROP_WAIT		(20)		//ヒップドロップの「開始・着地」硬直時間
#define PLAYER_BLOWING_POWER	(5.0f)	//ヒップドロップされた方の移動量
#define PLAYER_HIPDROP_POWER	(-10.0f)	//ヒップドロップするときの移動量
#define ACCELERATION_GRAVITY (9.8f)		//重力加速度
#define PLAYER_WEIGHT		(50)		//質量
#define PLAYER_POWER_ADD	(0.025f)		//移動の強さの増加値
#define DUMP_COEF			(0.04f)		//減衰係数
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)	//[デバッグ用]普通に移動するときの移動量
#define DECIMAL_PLACE		(1)			//小数点第何位まで移動していることにするか
#define BF_RADIUS			(353.5f)	//バトルフィールドの半径
#define DOWN_TIME			(200)		//ダウン判定とする時間

//移動量関係
#define ACCELERATION_CONS		(0.5f)		//加速定数（1.0で全部渡す）
#define ACCELERATION_ITEMMAG	(1.5f)		//アイテム所持中の強化倍率
#define DEFANCE_CONS			(0.0f)		//防御定数（1.0で完全防御）
#define DEFANCE_ITEMADD			(0.3f)		//アイテム所持中の強化量

#define TEST_SIZE_WIDTH		(30.0f)
#define TEST_SIZE_HEIGHT	(15.0f)
#define TEST_SIZE_DEPTH		(30.0f)

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
void MovePlayer(int nPadNum);
void RotPlayer(int nPadNum);		//MovePlayer のrot.y の計算式だけを残しています

void ChargePlayer(int nChargePlayer);	//チャージ処理
void DashPlayer(int nDashPlayer);		//ダッシュ処理
void JumpPlayer(int nJumpPlayer);		//ジャンプ処理
void HipDropPlayer(int nHipDropPlayer);	//ヒップドロップ処理

void CollisionPP(int nPlayerNum);	//プレイヤー同士の衝突判定
void HipDropPP(int nPlayerNum);		//ヒップドロップ時の衝突判定
void DownPlayer(int nDownPlayerNum);	//ダウンしたプレイヤーの処理
void RespawnPlayer(int nRespawnPlayer);	//リスポーン処理

//グローバル変数
Player g_aPlayer[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = ZERO_SET;
		g_aPlayer[nCntPlayer].rot = ZERO_SET;
		g_aPlayer[nCntPlayer].moveGauge = 0;
		g_aPlayer[nCntPlayer].jumpTime = 0;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].bHipDrop = false;
		g_aPlayer[nCntPlayer].nHipDropWait = 0;

		g_aPlayer[nCntPlayer].faceCollider[0] = D3DXVECTOR3(15.0f, 0.0f, 15.0f);
		g_aPlayer[nCntPlayer].faceCollider[1] = D3DXVECTOR3(-15.0f, 0.0f, 15.0f);

		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].lastAtkPlayer = -1;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;
		g_aPlayer[nCntPlayer].stat = PLAYERSTAT_WAIT;

		g_aPlayer[nCntPlayer].nATKItemTime = 0;
		g_aPlayer[nCntPlayer].nDEFItemTime = 0;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
	}

	//[デバッグ]コントローラーが接続されていなければ1Pのみ有効化する
	if (GetUseControllerNum() == 0) 
	{
		g_aPlayer[0].bUsePlayer = true;
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
	//デバッグ表示
	PrintDebugProc("[パラメータ]\n");

	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].lastAtkPlayer = -1;

		//現在の位置を前回の位置にする
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

		if (g_aPlayer[nCntPlayer].bUsePlayer == true)
		{
			//接続されているか確認して切断されていたらプレイヤーを消す（例外としてコントローラーがつながっていないときは無視）
			if (GetUseControllerNum() != 0)
			{
				g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
			}

			//ジャンプ時間を増やす
			g_aPlayer[nCntPlayer].jumpTime++;
			
			//ヒップドロップ中でなければ操作できる
			if (g_aPlayer[nCntPlayer].bHipDrop == false)
			{
				//キーボード操作時の動作
#if 1
				//移動方法（ダッシュ）押して離す
				if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
					&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
				{//もうこれ動いてるって言わないよね（ほぼ動いていない）
					if (GetKeyboardPress(DIK_SPACE) == true)
					{//スペースキーは押された

						//プレイヤーのチャージ処理
						ChargePlayer(nCntPlayer);
					}
					else if (GetKeyboardRelease(DIK_SPACE) == true)
					{//SPACEキーが離された
						//プレイヤーのダッシュ処理
						DashPlayer(nCntPlayer);
					}
				}
				else
				{
					g_aPlayer[nCntPlayer].moveGauge = 0;
				}

				//ジャンプ・ヒップドロップ
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					if (g_aPlayer[nCntPlayer].bJump)
					{
						HipDropPlayer(nCntPlayer);									//プレイヤーのヒップドロップ処理
						g_aPlayer[nCntPlayer].nHipDropWait = PLAYER_HIPDROP_WAIT;	//ヒップドロップの開始硬直を設定
					}
					else
					{
						JumpPlayer(nCntPlayer);			//プレイヤーのジャンプ処理
					}
				}
#endif
				//ゲームパッドの操作
				//移動方法（ダッシュ）押して離す
				if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
					&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
				{//もうこれ動いてるって言わないよね（ほぼ動いていない）
					if (GetGamepadPress(nCntPlayer, XINPUT_GAMEPAD_X) == true)
					{//Xボタンが押された					 
						//プレイヤーのチャージ処理
						ChargePlayer(nCntPlayer);
					}
					else if (GetGamepadRelease(nCntPlayer, XINPUT_GAMEPAD_X) == true)
					{//Xボタンが離された
						//プレイヤーのダッシュ処理
						DashPlayer(nCntPlayer);
					}
				}
				else
				{
					g_aPlayer[nCntPlayer].moveGauge = 0;
				}

				//ジャンプ・ヒップドロップ
				if (GetGamepadTrigger(nCntPlayer, XINPUT_GAMEPAD_A) == true)
				{
					if (g_aPlayer[nCntPlayer].bJump)
					{
						HipDropPlayer(nCntPlayer);									//プレイヤーのヒップドロップ処理
						g_aPlayer[nCntPlayer].nHipDropWait = PLAYER_HIPDROP_WAIT;	//ヒップドロップの開始硬直を設定
					}
					else
					{
						JumpPlayer(nCntPlayer);			//プレイヤーのジャンプ処理
					}
				}
				
				//[デバッグ用]普通に移動する処理
#ifdef _DEBUG
				MovePlayer(nCntPlayer);
#endif
			}

			//ヒップドロップ中
			else
			{
				//ヒップドロップ硬直時間がある
				if (g_aPlayer[nCntPlayer].nHipDropWait > 0)
				{
					g_aPlayer[nCntPlayer].nHipDropWait--;		//硬直時間を減らしていく
				}
				else
				{
					//落下させる
					g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].moveV0.y;
				}
			}

			//ジャンプ量設定
			g_aPlayer[nCntPlayer].move.y = g_aPlayer[nCntPlayer].moveV0.y - (ACCELERATION_GRAVITY * g_aPlayer[nCntPlayer].jumpTime / MAX_FPS);

			//移動後がy<0なら落ちるか移動量消す
			if (g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].move.y < 0.0f && g_aPlayer[nCntPlayer].stat != PLAYERSTAT_FALL)
			{
				float fLength = sqrtf(powf((g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].move.x), 2) +
					powf((g_aPlayer[nCntPlayer].pos.z + g_aPlayer[nCntPlayer].move.z), 2));

				if (fLength >= BF_RADIUS)
				{
					g_aPlayer[nCntPlayer].stat = PLAYERSTAT_FALL;
				}
				else
				{
					g_aPlayer[nCntPlayer].bJump = false;
					g_aPlayer[nCntPlayer].bHipDrop = false;
					g_aPlayer[nCntPlayer].moveV0.y = 0.0f;
					g_aPlayer[nCntPlayer].move.y = 0.0f;
					g_aPlayer[nCntPlayer].jumpTime = 0;
					g_aPlayer[nCntPlayer].pos.y = 0.0f;
				}
			}

			//向きを変える処理
			RotPlayer(nCntPlayer);

			//当たり判定類
			CollisionPP(nCntPlayer);
			HipDropPP(nCntPlayer);

			//影位置設定
			//一旦なし

			if (g_aPlayer[nCntPlayer].stat == PLAYERSTAT_FALL && g_aPlayer[nCntPlayer].jumpTime >= DOWN_TIME)
			{
				DownPlayer(nCntPlayer);
			}

			//デバッグ表示（強さ表示）
			PrintDebugProc("[%d]:Power = %f\n", nCntPlayer, g_aPlayer[nCntPlayer].moveGauge);
		}
	}

	//移動量と衝突判定をもとに移動する
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (true)
		{//判定入れたければ入れて
			if (g_aPlayer[nCntPlayer].lastAtkPlayer == -1)
			{//ぶつかってないまたは移動量交換済み		
				//ヒップドロップしておらず、ヒップドロップの硬直中でもない
				if (g_aPlayer[nCntPlayer].bHipDrop == false && g_aPlayer[nCntPlayer].nHipDropWait <= 0)
				{
					//普通に移動
					g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
					g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y;
					g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;
				}
			}
			else
			{//ぶつかった
				//移動量交換
				D3DXVECTOR3 moveTmp1 = g_aPlayer[nCntPlayer].move;
				D3DXVECTOR3 moveTmp2 = g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].move;

				//割合設定
				float fPowerConvertion1 = ACCELERATION_CONS * (g_aPlayer[nCntPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayer[nCntPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				float fPowerConvertion2 = ACCELERATION_CONS * (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				g_aPlayer[nCntPlayer].move = moveTmp2 * fPowerConvertion1;
				g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].move = moveTmp1 * fPowerConvertion2;

				//移動量交換済み扱いにする
				g_aPlayer[nCntPlayer].lastAtkPlayer = -1;
				g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].lastAtkPlayer = -1;

				//普通に移動
				g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
				g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y;
				g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;
			}

			//移動量減衰
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			////[仮]一旦Y=0.0fになったら着地
			//if (g_aPlayer[nCntPlayer].pos.y <= 0.0f)
			//{
			//	g_aPlayer[nCntPlayer].pos.y = 0.0f;
			//}
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
				if (g_aPlayer[nCntPlayer].stat != PLAYERSTAT_FALL)
				{
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

						//黒色に設定						//自己発光を無くす
						MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

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
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//プレイヤーのチャージ処理
//========================
void ChargePlayer(int nChargePlayer)
{
	g_aPlayer[nChargePlayer].moveGauge += PLAYER_POWER_ADD;

	if (g_aPlayer[nChargePlayer].moveGauge >= PLAYER_POWER_MAX)
	{
		g_aPlayer[nChargePlayer].moveGauge = PLAYER_POWER_MAX;
		StopSound(SOUND_LABEL_SE_ENERGY_00);
	}
	if (g_aPlayer[nChargePlayer].moveGauge <= PLAYER_POWER_ADD)
	{
		PlaySound(SOUND_LABEL_SE_ENERGY_00);
	}

	SetEffect(g_aPlayer[nChargePlayer].pos, nChargePlayer, EFFECTTYPE_CHARGE);
}

//========================
//プレイヤーのダッシュ処理
//========================
void DashPlayer(int nDashPlayer)
{
	//進行方向の設定
	g_aPlayer[nDashPlayer].move.x = -sinf(g_aPlayer[nDashPlayer].rot.y) * g_aPlayer[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;
	g_aPlayer[nDashPlayer].move.z = -cosf(g_aPlayer[nDashPlayer].rot.y) * g_aPlayer[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;

	g_aPlayer[nDashPlayer].moveGauge = 0;
}

//========================
//プレイヤーのヒップドロップ処理
//========================
void HipDropPlayer(int nHipDropPlayer)
{
	g_aPlayer[nHipDropPlayer].moveV0.y = PLAYER_HIPDROP_POWER;		//ヒップドロップの降下速度を代入
	g_aPlayer[nHipDropPlayer].jumpTime = 0;							//ジャンプ時間リセット
	g_aPlayer[nHipDropPlayer].bHipDrop = true;						//ヒップドロップしている
}

//========================
//プレイヤーのジャンプ処理
//========================
void JumpPlayer(int nJumpPlayer)
{
	g_aPlayer[nJumpPlayer].moveV0.y = PLAYER_JUMP_SPEED;//移動量設定
	g_aPlayer[nJumpPlayer].jumpTime = 0;	//ジャンプ時間リセット
	g_aPlayer[nJumpPlayer].bJump = true;
}

//========================
//プレイヤー同士の衝突判定処理
//========================
void CollisionPP(int nPlayerNum)
{
	//=pos0~pos3の説明==================
	// pos3		pos2
	//	・<-----・		矢印:vecLine
	//	｜		↑
	//	｜		｜
	//	↓		｜
	//	・----->・
	// pos0		pos1
	//==================================

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove[2];

	//2は2頂点の2
	float fAreaARight[2], fAreaALeft[2], fAreaBRight[2], fAreaBLeft[2];
	float fAreaAUp[2], fAreaADown[2], fAreaBUp[2], fAreaBDown[2];

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = g_aPlayer[nPlayerNum].pos + g_aPlayer[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)// && g_aPlayer[nCntOtherPlayer].bUsePlayer == true
		{
			//各頂点求める
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos0.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos1.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//PI - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos2.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//-pos3---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//-PI + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos3.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos3.y = 0.0f;
			pos3.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos3---------------------------------------------------------------------------------------------------------------------------

			//ベクトル求める
			//move
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				vecMove[nCntCollision] = (posTemp + g_aPlayer[nPlayerNum].faceCollider[nCntCollision]) - (g_aPlayer[nPlayerNum].posOld + g_aPlayer[nPlayerNum].faceCollider[nCntCollision]);
			}

			//X
			//右方向の計算
			vecLineRight = pos1 - pos0;
			vecToPosRight = posTemp - pos0;
			vecToPosOldRight = g_aPlayer[nPlayerNum].posOld - pos0;

			//左方向の計算
			vecLineLeft = pos3 - pos2;
			vecToPosLeft = posTemp - pos2;
			vecToPosOldLeft = g_aPlayer[nPlayerNum].posOld - pos2;

			//Z
			//上方向の計算
			vecLineUp = pos2 - pos1;
			vecToPosUp = posTemp - pos1;
			vecToPosOldUp = g_aPlayer[nPlayerNum].posOld - pos1;
			//下方向の計算
			vecLineDown = pos0 - pos3;
			vecToPosDown = posTemp - pos3;
			vecToPosOldDown = g_aPlayer[nPlayerNum].posOld - pos3;

			//当たり判定本番
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				//X
				//面積求める
				fAreaARight[nCntCollision] = TASUKIGAKE(vecToPosRight.x, vecToPosRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaALeft[nCntCollision] = TASUKIGAKE(vecToPosLeft.x, vecToPosLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBRight[nCntCollision] = TASUKIGAKE(vecLineRight.x, vecLineRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBLeft[nCntCollision] = TASUKIGAKE(vecLineLeft.x, vecLineLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) <= 0.0f && (vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f)
				{
					if (fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] >= 0.0f && fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							//1.0f = pushback
							float fRate = fAreaARight[nCntCollision] / fAreaBRight[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos0.x + (vecLineRight.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos0.z + (vecLineRight.z * fRate) - cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
				{
					if (fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] >= 0.0f && fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							float fRate = fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos2.x + (vecLineLeft.x * fRate) + sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos2.z + (vecLineLeft.z * fRate) + cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
			
				//Z
				//面積求める
				fAreaAUp[nCntCollision] = TASUKIGAKE(vecToPosUp.x, vecToPosUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaADown[nCntCollision] = TASUKIGAKE(vecToPosDown.x, vecToPosDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBUp[nCntCollision] = TASUKIGAKE(vecLineUp.x, vecLineUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBDown[nCntCollision] = TASUKIGAKE(vecLineDown.x, vecLineDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) <= 0.0f && (vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f)
				{
					if (fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] >= 0.0f && fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							float fRate = fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos1.x + (vecLineUp.x * fRate) + cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos1.z + (vecLineUp.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
				{
					if (fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] >= 0.0f && fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							float fRate = fAreaADown[nCntCollision] / fAreaBDown[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos3.x + (vecLineDown.x * fRate) - cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos3.z + (vecLineDown.z * fRate) + sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
			}
		}
	}
}

//========================
//ヒップドロップ時の衝突処理
//========================
void HipDropPP(int nPlayerNum)
{
	//=pos0~pos2の説明==================
	//
	//		・g_ap[nPNum].posOld
	//		↓
	//		・g_ap[nPNum].pos
	// pos1		pos0
	//	・<-----・		矢印:vecLine
	//	｜
	//	｜
	//	↓
	//　・
	// pos2
	//==================================

	D3DXVECTOR3 pos0, pos1, pos2;
	D3DXVECTOR3 vecLineX, vecToPosX, vecToPosOldX;
	D3DXVECTOR3 vecLineZ, vecToPosZ, vecToPosOldZ;
	D3DXVECTOR3 vecMove;
	float fAreaAX, fAreaBX, fAreaAZ, fAreaBZ;

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = g_aPlayer[nPlayerNum].pos + g_aPlayer[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)// && g_aPlayer[nCntOtherPlayer].bUsePlayer == true
		{
			//各頂点求める
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos0.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos1.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos2.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//ベクトル求める
			//move
			vecMove = g_aPlayer[nPlayerNum].pos - g_aPlayer[nPlayerNum].posOld;

			//X
			vecLineX = pos1 - pos0;
			vecToPosX = posTemp - pos0;
			vecToPosOldX = g_aPlayer[nPlayerNum].posOld - pos0;

			//Z
			vecLineZ = pos2 - pos1;
			vecToPosZ = posTemp - pos1;
			vecToPosOldZ = g_aPlayer[nPlayerNum].posOld - pos1;

			//当たり判定本番
			//X
			//面積求める
			fAreaAX = TASUKIGAKE(vecToPosX.x, vecToPosX.y, vecMove.x, vecMove.y);
			fAreaBX = TASUKIGAKE(vecLineX.x, vecLineX.y, vecMove.x, vecMove.y);
			fAreaAZ = TASUKIGAKE(vecToPosZ.z, vecToPosZ.y, vecMove.z, vecMove.y);
			fAreaBZ = TASUKIGAKE(vecLineZ.z, vecLineZ.y, vecMove.z, vecMove.y);
			//左側AND範囲内
			if ((vecLineX.y * vecToPosX.x) - (vecLineX.x * vecToPosX.y) <= 0.0f && (vecLineX.y * vecToPosOldX.x) - (vecLineX.x * vecToPosOldX.y) >= 0.0f)
			{
				if (fAreaAX / fAreaBX >= 0.0f && fAreaAX / fAreaBX <= 1.0f)
				{//衝突
					if (fAreaAZ / fAreaBZ >= 0.0f && fAreaAZ / fAreaBZ <= 1.0f)
					{
						//移動量計算
						float fAngleHipDrop = atan2f(g_aPlayer[nCntOtherPlayer].pos.x - g_aPlayer[nPlayerNum].pos.x,
							g_aPlayer[nCntOtherPlayer].pos.z - g_aPlayer[nPlayerNum].pos.z);
						g_aPlayer[nCntOtherPlayer].move.x = sinf(fAngleHipDrop) * PLAYER_BLOWING_POWER;
						g_aPlayer[nCntOtherPlayer].move.z = -cosf(fAngleHipDrop) * PLAYER_BLOWING_POWER;

						//攻撃された扱いにする
						g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
					}
				}
			}
		}
	}
}

//========================
//[デバッグ用]プレイヤーの移動処理
//========================
void MovePlayer(int nPadNum)
{
	//モデル移動
	//ゲームパッド部
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X方向のスティックが傾いている
		if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y方向のスティックも傾いている
			g_aPlayer[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y方向のスティックだけ傾いている
		g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
	}
	//キーボード部
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else
	{
		return;
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	g_aPlayer[nPadNum].pos.x += g_aPlayer[nPadNum].move.x;
	g_aPlayer[nPadNum].pos.z += g_aPlayer[nPadNum].move.z;

	//移動量消す
	g_aPlayer[nPadNum].move.x = 0.0f;
	g_aPlayer[nPadNum].move.z = 0.0f;
}

//========================
//プレイヤーの向き変更処理
//========================
void RotPlayer(int nPadNum)
{
	//モデル移動
	//ゲームパッド部
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X方向のスティックが傾いている
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y方向のスティックだけ傾いている
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	//キーボード部
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
	}
	else
	{
		return;
	}
}

//========================
//ダウンしたプレイヤーの処理
//========================
void DownPlayer(int nDownPlayerNum)
{
	if (g_aPlayer[nDownPlayerNum].nNumHitPlayer != -1)
	{//当てられて落ちた場合
		g_aPlayer[g_aPlayer[nDownPlayerNum].nNumHitPlayer].nScore++;
		AddScore(1, g_aPlayer[nDownPlayerNum].nNumHitPlayer);
	}
	else
	{//ただ単に自滅した場合
		for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
		{
			if (nCntOtherPlayer != nDownPlayerNum)
			{
				g_aPlayer[nCntOtherPlayer].nScore++;
				AddScore(1, nCntOtherPlayer);
			}
		}
	}

	RespawnPlayer(nDownPlayerNum);
}

//========================
//プレイヤーのリスポーン処理
//========================
void RespawnPlayer(int nRespawnPlayer)
{
	//各ステータス再設定
	g_aPlayer[nRespawnPlayer].pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	g_aPlayer[nRespawnPlayer].posOld = g_aPlayer[nRespawnPlayer].pos;
	g_aPlayer[nRespawnPlayer].move = ZERO_SET;
	g_aPlayer[nRespawnPlayer].moveV0 = ZERO_SET;
	g_aPlayer[nRespawnPlayer].rot = ZERO_SET;
	g_aPlayer[nRespawnPlayer].moveGauge = 0;
	g_aPlayer[nRespawnPlayer].jumpTime = 0;
	g_aPlayer[nRespawnPlayer].bJump = true;

	g_aPlayer[nRespawnPlayer].lastAtkPlayer = -1;
	g_aPlayer[nRespawnPlayer].nNumHitPlayer = -1;
	g_aPlayer[nRespawnPlayer].stat = PLAYERSTAT_WAIT;
}

//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}