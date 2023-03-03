//==========================================
//
//プレイヤープログラム[hdr_player.cpp]
//Author:藤原龍輝  平澤詩苑
//
//==========================================
#include "main.h"
#include "HDRgame.h"
#include "HDR_player.h"
#include "model.h"
#include "conversioninput.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "sound.h"
#include "meshfield.h"
#include "block.h"
#include "rank.h"

//マクロ定義
#define PLAYER_HIPSPIN_SPEED	(-0.5f)				//ヒップドロップスピンの回転値
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//ヒップドロップスピンしたときの１周判定をとる値
#define PLAYER_HIPSPIN_WAIT		(20)				//ヒップドロップスピンが終わって急降下するまでの時間
#define PLAYER_MOVE_SPEED	(5.0f)				//普通に移動するときの移動量

//ヒップドロップレベルに必要な高さ（その値以上次のレベルの値以下）
#define HIPDROP_HEIGHT_LEVEL_1		(10)				//レベル1
#define HIPDROP_HEIGHT_LEVEL_2		(50)				//レベル2
#define HIPDROP_HEIGHT_LEVEL_3		(100)				//レベル3
#define HIPDROP_HEIGHT_LEVEL_MAX	(150)				//レベルMAX

//ヒップドロップレベルに応じた強さ
#define HIPDROP_POWER_LEVEL_1		(100)				//レベル1（100ダメージ）
#define HIPDROP_POWER_LEVEL_2		(200)				//レベル2（200ダメージ）
#define HIPDROP_POWER_LEVEL_3		(300)				//レベル3（300ダメージ）
#define HIPDROP_POWER_LEVEL_MAX		(500)				//レベルMAX（500ダメージ）

//向き
#define ROT_WA	(0.75f * D3DX_PI)	//左上
#define ROT_WD	(-0.75f * D3DX_PI)	//右上
#define ROT_SA	(0.25f * D3DX_PI)	//左下
#define ROT_SD	(-0.25f * D3DX_PI)	//右下
#define ROT_W	(-1.0f * D3DX_PI)	//上
#define ROT_A	(0.5f * D3DX_PI)	//左
#define ROT_S	(0.0f * D3DX_PI)	//下
#define ROT_D	(-0.5f * D3DX_PI)	//右

//グローバル変数
Player_HDR g_aPlayerHDR[MAX_USE_GAMEPAD];
int g_nIdxShadow_HDR = -1;
bool g_buse[MAX_USE_GAMEPAD] = { false };

//プロトタイプ宣言
void ControllPlayer_HDR(int nPlayerNum);
void MovePlayer(int nPadNum);

void JumpPlayer_HDR(int nJumpPlayer);			//ジャンプの設定処理
void HipDropPlayer_HDR(int nHipDropPlayer);		//ヒップドロップの設定処理
void HipSpinPlayer_HDR(int nHipSpinPlayer);		//ヒップスピンの処理

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-225.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(-75.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(75.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(225.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
};

//[デバッグ用]AI挙動させるプレイヤー指定（コントローラーが刺さっていればそれを優先）
const bool c_aAIMove[MAX_USE_GAMEPAD] = { false,true,true,true };

//========================
//初期化処理
//========================
void InitPlayer_HDR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

												//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//変数初期化
		g_aPlayerHDR[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayerHDR[nCntPlayer].pos.y = MAX_BLOCK * COLLISION_SIZE_Y;	//一番上のブロックの位置に設定
		g_aPlayerHDR[nCntPlayer].posOld = g_aPlayerHDR[nCntPlayer].pos;
		g_aPlayerHDR[nCntPlayer].move = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].moveV0 = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].rot = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].jumpTime = 0;
		g_aPlayerHDR[nCntPlayer].nHipDropWait = 0;
		g_aPlayerHDR[nCntPlayer].bJump = false;
		g_aPlayerHDR[nCntPlayer].bHipDrop = false;
		g_aPlayerHDR[nCntPlayer].bHipDropSpin = false;
		g_aPlayerHDR[nCntPlayer].bGoal = false;
		g_buse[nCntPlayer] = false;

		g_aPlayerHDR[nCntPlayer].animal = ANIMAL_WILDBOAR;

		g_aPlayerHDR[nCntPlayer].model = GetAnimal(g_aPlayerHDR[nCntPlayer].animal);
		g_aPlayerHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
	}

	//[デバッグ]コントローラーが接続されていなければ1Pのみ有効化する
	if (GetUseControllerNum_HDR() == 0)
	{
		g_aPlayerHDR[0].bUsePlayer = true;
	}
}

//========================
//終了処理
//========================
void UninitPlayer_HDR(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//プレイヤーの数だけ処理する
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//取得した方なのでNULL入れるだけでOK
			g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdatePlayer_HDR(void)
{
	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//ジャンプ時間を増やす
		g_aPlayerHDR[nCntPlayer].jumpTime++;

		if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
		{//使用時のみ行う

			//接続されているか確認して切断されていたらプレイヤーを消す（例外としてコントローラーがつながっていないときは無視）
			if (GetUseControllerNum_HDR() != 0)
			{
				g_aPlayerHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
			}

			//各プレイヤーの操作
			ControllPlayer_HDR(nCntPlayer);
		}

		//ヒップドロップスピンの硬直中
		if (0 < g_aPlayerHDR[nCntPlayer].nHipDropWait)
		{
			HipSpinPlayer_HDR(nCntPlayer);
		}
		//硬直中で無ければ落下速度を代入
		else
		{
			g_aPlayerHDR[nCntPlayer].move.y = g_aPlayerHDR[nCntPlayer].moveV0.y - (9.8f * g_aPlayerHDR[nCntPlayer].jumpTime / MAX_FPS);
		}

		//移動
		g_aPlayerHDR[nCntPlayer].pos += g_aPlayerHDR[nCntPlayer].move;

		//プレイヤーが地面を突き抜けてしまった
		if (g_aPlayerHDR[nCntPlayer].pos.y < 0.0f)
		{
			g_aPlayerHDR[nCntPlayer].pos.y = 0.0f;
			g_aPlayerHDR[nCntPlayer].move.y = 0.0f;
			g_aPlayerHDR[nCntPlayer].bJump = false;
			g_aPlayerHDR[nCntPlayer].bHipDrop = false;

			if (g_buse[nCntPlayer] == false)
			{//ゴール判定
				g_aPlayerHDR[nCntPlayer].bGoal = true;
				UpdateRank(nCntPlayer);
				g_buse[nCntPlayer] = true;
			}
		}

		//ブロックの当たり判定（一旦ゴール後は当たり判定なし）
		if (g_aPlayerHDR[nCntPlayer].bGoal == false)
		{
			CollisionBlock(nCntPlayer);
		}
	}
}

//========================
//描画処理
//========================
void DrawPlayer_HDR(void)
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
		D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerHDR[nCntPlayer].rot.y, g_aPlayerHDR[nCntPlayer].rot.x, g_aPlayerHDR[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayerHDR[nCntPlayer].pos.x, g_aPlayerHDR[nCntPlayer].pos.y, g_aPlayerHDR[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].bUse == true)
			{
				g_aPlayerHDR[nCntPlayer].posOld_Coll.y = g_aPlayerHDR[nCntPlayer].pos.y;

				D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
				D3DXMATRIX mtxParent;					//親のマトリ

				//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
				g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].posOffset;
				g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rotOffset;

				//"モデルの"ワールドマトリックス初期化
				D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

				//位置反映
				D3DXMatrixTranslation(&mtxTransModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

				//パーツの親マトリ設定
				if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
				{
					mtxParent = g_aPlayerHDR[nCntPlayer].model.aParts[g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayerHDR[nCntPlayer].mtxWorld;
				}

				//パーツのマトリと親マトリをかけ合わせる
				D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

				//"プレイヤーの"ワールドマトリックス設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//マテリアルデータへのポインタ取得
				pMat = (D3DXMATERIAL*)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					//ゴースト用
					D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

					if (nCntParts == MAX_PARTS - 1)
					{//ゼッケンの時は色変更
						matChange.Diffuse = c_aColPlayer[nCntPlayer];
					}

					//マテリアルの設定
					pDevice->SetMaterial(&matChange);

					//テクスチャ設定
					pDevice->SetTexture(0, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

					//モデル描画
					g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				/*------------------------------------------------------------------
				影の描画		Author:平澤詩苑 石原颯馬
				--------------------------------------------------------------------*/
				if (g_aPlayerHDR[nCntPlayer].pos.y >= 0.0f)
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
					D3DXMatrixMultiply(&mtxShadow, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxShadow);

					//シャドウマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

					//マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL *)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

																		//黒色に設定						//自己発光を無くす
						MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

						//マテリアル設定
						pDevice->SetMaterial(&MatCopy);

						//テクスチャ設定
						pDevice->SetTexture(0, NULL);

						//モデル描画
						g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//プレイヤーのゲームパッド操作
//========================
void ControllPlayer_HDR(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//ジャンプ・ヒップドロップ
		if (GetButton(nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump && g_aPlayerHDR[nPlayerNum].bGoal == false)
			{//ジャンプ中であり、ゴールしていない
				HipDropPlayer_HDR(nPlayerNum);		//プレイヤーのヒップドロップ処理
			}
			else if (g_aPlayerHDR[nPlayerNum].bJump == false)
			{//ジャンプしていない
				JumpPlayer_HDR(nPlayerNum);			//プレイヤーのジャンプ処理
			}
		}
	}

	//ゴール後の煽り行為
	if (g_aPlayerHDR[nPlayerNum].bGoal == true)
	{
		MovePlayer(nPlayerNum);
	}
}

//========================
//プレイヤーが普通に移動する処理
//========================
void MovePlayer(int nPadNum)
{
	//モデル移動
	//ゲームパッド部
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X方向のスティックが傾いている
		if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y方向のスティックも傾いている
			g_aPlayerHDR[nPadNum].rot.y = atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum));
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
			g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum));
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y方向のスティックだけ傾いている
		g_aPlayerHDR[nPadNum].rot.y = atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum));
		g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
	}
	//キーボード部
	else if (GetStick(nPadNum,INPUTTYPE_PRESS).y == CONVSTICK_UP)
	{
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_WA;
		}
		else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_WD;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_W;
		}
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).y == CONVSTICK_DOWN)
	{
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_SA;
		}
		else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_SD;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_S;
		}
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
	{
		g_aPlayerHDR[nPadNum].rot.y = ROT_A;
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
	{
		g_aPlayerHDR[nPadNum].rot.y = ROT_D;
	}

	//キーが押されたら移動
	if (GetStick(nPadNum, INPUTTYPE_PRESS).x != CONVSTICK_NEUTRAL || GetStick(nPadNum, INPUTTYPE_PRESS).y != CONVSTICK_NEUTRAL)
	{
		g_aPlayerHDR[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerHDR[nPadNum].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_aPlayerHDR[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerHDR[nPadNum].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else
	{//そうでないので終了
		return;
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	g_aPlayerHDR[nPadNum].pos.x += g_aPlayerHDR[nPadNum].move.x;
	g_aPlayerHDR[nPadNum].pos.z += g_aPlayerHDR[nPadNum].move.z;

	//移動量消す
	g_aPlayerHDR[nPadNum].move.x = 0.0f;
	g_aPlayerHDR[nPadNum].move.z = 0.0f;
}

//========================
//ジャンプの処理
//========================
void JumpPlayer_HDR(int nJumpPlayer)
{
	PlaySound(SOUND_LABEL_SE_JUMP);

	g_aPlayerHDR[nJumpPlayer].posOld.y = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].moveV0.y = 7.7f;		//移動量設定
	g_aPlayerHDR[nJumpPlayer].jumpTime = 0;			//ジャンプ時間リセット
	g_aPlayerHDR[nJumpPlayer].bJump = true;
}

//========================
//ヒップドロップの処理
//========================
void HipDropPlayer_HDR(int nHipDropPlayer)
{
	//ヒップドロップのパワーレベルを測定
	g_aPlayerHDR[nHipDropPlayer].HipDropPower = g_aPlayerHDR[nHipDropPlayer].pos.y - g_aPlayerHDR[nHipDropPlayer].posOld.y;

	if (g_aPlayerHDR[nHipDropPlayer].HipDropPower >= HIPDROP_HEIGHT_LEVEL_MAX)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_MAX;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > HIPDROP_HEIGHT_LEVEL_3)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_3;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > HIPDROP_HEIGHT_LEVEL_2)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_2;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > HIPDROP_HEIGHT_LEVEL_1)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_1;
	}

	g_aPlayerHDR[nHipDropPlayer].move.y = 0.0f;						//通常の落下速度を０にする
	g_aPlayerHDR[nHipDropPlayer].moveV0.y = -15.0f;					//ヒップドロップの降下速度を代入
	g_aPlayerHDR[nHipDropPlayer].jumpTime = 0;						//ジャンプ時間リセット
	g_aPlayerHDR[nHipDropPlayer].bHipDrop = true;					//ヒップドロップしている
	g_aPlayerHDR[nHipDropPlayer].bHipDropSpin = true;				//スピンしている
	g_aPlayerHDR[nHipDropPlayer].nHipDropWait = PLAYER_HIPSPIN_WAIT;//ヒップドロップスピン後の硬直時間設定
}

//========================
//ヒップスピンの処理
//========================
void HipSpinPlayer_HDR(int nHipSpinPlayer)
{
	//ヒップドロップのスピン中	
	if (g_aPlayerHDR[nHipSpinPlayer].bHipDropSpin)
	{
		//前転させる				
		g_aPlayerHDR[nHipSpinPlayer].rot.x += PLAYER_HIPSPIN_SPEED;

		//１周した				
		if (g_aPlayerHDR[nHipSpinPlayer].rot.x <= PLAYER_HIPSPIN_LAP)
		{
			g_aPlayerHDR[nHipSpinPlayer].rot.x = 0.0f;			//回転値を元に戻す
			g_aPlayerHDR[nHipSpinPlayer].bHipDropSpin = false;	//スピンし終わった		
		}
	}

	//ヒップドロップ硬直時間がある
	else
	{
		g_aPlayerHDR[nHipSpinPlayer].nHipDropWait--;		//硬直時間を減らしていく
	}
}

//========================
//取得処理
//========================
Player_HDR *GetPlayer_HDR(void)
{
	return &g_aPlayerHDR[0];
}